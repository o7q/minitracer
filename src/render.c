#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <float.h>

#include "ray.h"
#include "camera.h"
#include "math_utils.h"

Renderer *renderer_create(unsigned int width, unsigned int height, unsigned int thread_count)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    *renderer = (Renderer){(RenderSettings){NULL, NULL, width, height, 5, 20}};

    renderer->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    renderer->render_chunks = (RenderChunk **)malloc(sizeof(RenderChunk *) * thread_count);

    renderer->thread_station.thread_count = thread_count;

    renderer->thread_station.pixels = (Vec3 *)malloc(sizeof(Vec3) * width * height);

    pthread_mutex_init(&renderer->thread_station.finished_mutex, NULL);
    pthread_cond_init(&renderer->thread_station.thread_done_cond, NULL);

    int px_inc = (width * height) / thread_count;
    for (int i = 0; i < thread_count; ++i)
    {
        int px_start = i * px_inc;
        int px_end = i * px_inc + px_inc;
        if (px_end > width * height)
        {
            px_end = width * height;
        }

        if (i == thread_count - 1)
        {
            int offset = width * height - px_inc * thread_count;
            px_end += offset;
        }

        RenderChunk *rc = (RenderChunk *)malloc(sizeof(RenderChunk));
        *rc = (RenderChunk){&renderer->settings, px_start, px_end, i, 0, 0};

        rc->thread_station = &renderer->thread_station;

        pthread_mutex_init(&rc->wake_mutex, NULL);
        pthread_mutex_init(&rc->terminate_mutex, NULL);
        pthread_cond_init(&rc->wake_cond, NULL);

        renderer->render_chunks[i] = rc;
        pthread_create(&renderer->threads[i], NULL, worker_thread, rc);
    }

    return renderer;
}

void renderer_set_world(Renderer *renderer, World *world)
{
    renderer->settings.world = world;
}

void renderer_set_camera(Renderer *renderer, Cam *camera)
{
    renderer->settings.camera = camera;
}

void renderer_set_samples(Renderer *renderer, unsigned int samples)
{
    renderer->settings.samples = samples;
}
void renderer_set_bounces(Renderer *renderer, unsigned int bounces)
{
    renderer->settings.bounces = bounces;
}

void renderer_delete(Renderer *renderer)
{
    if (!renderer)
    {
        return;
    }

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        RenderChunk *rc = renderer->render_chunks[i];

        if (!rc)
        {
            continue;
        }

        pthread_mutex_lock(&rc->terminate_mutex);
        rc->terminate = 1;
        pthread_cond_signal(&rc->wake_cond);
        pthread_mutex_unlock(&rc->terminate_mutex);

        printf("Freeing: %d\n", i);
        fflush(stdout);

        pthread_join(renderer->threads[i], NULL);

        pthread_mutex_destroy(&rc->wake_mutex);
        pthread_mutex_destroy(&rc->terminate_mutex);
        pthread_cond_destroy(&rc->wake_cond);

        free(rc);
    }

    if (renderer->render_chunks)
    {
        free(renderer->render_chunks);
    }

    pthread_mutex_destroy(&renderer->thread_station.finished_mutex);
    pthread_cond_destroy(&renderer->thread_station.thread_done_cond);

    if (renderer->threads)
    {
        free(renderer->threads);
    }

    if (renderer->thread_station.pixels)
    {
        free(renderer->thread_station.pixels);
    }

    free(renderer);
}

void *worker_thread(void *data)
{
    RenderChunk *rc = (RenderChunk *)data;

    random_thread_init(rc->index);

    while (1)
    {
        pthread_mutex_lock(&rc->wake_mutex);
        if (!rc->ready)
        {
            pthread_cond_wait(&rc->wake_cond, &rc->wake_mutex);

            if (rc->terminate)
            {
                break;
            }
        }

        rc->ready = 0;

        pthread_mutex_unlock(&rc->wake_mutex);

        render_chunk(rc);

        pthread_mutex_lock(&rc->thread_station->finished_mutex);
        rc->thread_station->finished_count++;
        if (rc->thread_station->finished_count == rc->thread_station->thread_count)
        {
            pthread_cond_signal(&rc->thread_station->thread_done_cond); // wake main
        }
        pthread_mutex_unlock(&rc->thread_station->finished_mutex);
    }

    return NULL;
}

void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_tri(ray, tri);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *tri->mat;
    }
}

void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        render_handle_tri(ray, mesh->tris[i], hit_info, hit_mat, t_lowest);
    }
}

void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_sphere(ray, sphere);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *sphere->mat;
    }
}

void render_chunk(void *data)
{
    RenderChunk *rc = (RenderChunk *)data;

    RenderSettings *rs = rc->settings;

    if (!rs->camera || !rs->world)
    {
        return;
    }

    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)rs->width / rs->height);

    float pixel_delta_u = viewport_width / rs->width;
    float pixel_delta_v = viewport_height / rs->height;

    Vec3 viewport_top_left = vec_sub(rs->camera->position, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, rs->camera->fov});
    Vec3 pixel00_pos = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    for (int i = rc->px_start; i < rc->px_end; ++i)
    {
        int x = i % rs->width;
        int y = i / rs->width;

        Vec3 pixel_center = vec_add(pixel00_pos, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});
        Vec3 ray_direction = vec_sub(pixel_center, rs->camera->position);
        Vec3 ray_dir_normal = vec_normalize(ray_direction);
        Vec3 ray_rotated_normal = ray_dir_normal;

        float temp_y = ray_dir_normal.y * cosf(rs->camera->rotation.x) - ray_dir_normal.z * sinf(rs->camera->rotation.x);
        float temp_z = ray_dir_normal.y * sinf(rs->camera->rotation.x) + ray_dir_normal.z * cosf(rs->camera->rotation.x);

        ray_rotated_normal.x = ray_dir_normal.x * cosf(rs->camera->rotation.y) + temp_z * sinf(rs->camera->rotation.y);
        ray_rotated_normal.y = temp_y;
        ray_rotated_normal.z = temp_z * cosf(rs->camera->rotation.y) - ray_dir_normal.x * sinf(rs->camera->rotation.y);

        ray_direction = vec_mult_v(ray_rotated_normal, vec_length(ray_direction));

        Vec3 render_color = (Vec3){0, 0, 0};

        for (int i = 0; i < rs->samples; ++i)
        {
            Ray3 ray;
            ray.origin = rs->camera->position;
            ray.direction = ray_direction;
            ray.color = (Vec3){1, 1, 1};
            ray.radiance = (Vec3){0, 0, 0};

            for (int j = 0; j < rs->bounces; ++j)
            {
                float t_lowest = FLT_MAX;

                Ray3Hit hit_info;
                hit_info.hit = 0;
                Mat hit_mat;

                for (int k = 0; k < rs->world->object_index; ++k)
                {
                    switch (rs->world->objects_track[k])
                    {
                    case OBJECT_TRI:
                        render_handle_tri(&ray, (TriObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_MESH:
                        render_handle_mesh(&ray, (MeshObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_SPHERE:
                        render_handle_sphere(&ray, (SphereObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    }
                }

                if (hit_info.hit)
                {
                    ray_bounce(&ray, &hit_info, &hit_mat);
                }
                else
                {
                    Vec3 unit_direction = vec_mult_v(vec_normalize(ray.direction), -1.0f);
                    float a = 0.5f * (unit_direction.y + 1.0f);
                    Vec3 color = vec_add((Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (Vec3){a * 0.5f, a * 0.7f, a * 1.0f});
                    // color = vec_div_v(color, 1.5f);
                    ray.radiance = vec_add(ray.radiance, vec_mult(ray.color, color));
                    break;
                }
            }

            render_color = vec_add(render_color, ray.radiance);
        }

        render_color = vec_div_v(render_color, (float)rs->samples);

        float gamma = 0.9f;

        render_color.x = powf(render_color.x, 1.0f / gamma);
        render_color.y = powf(render_color.y, 1.0f / gamma);
        render_color.z = powf(render_color.z, 1.0f / gamma);

        // clamp color
        if (render_color.x > 1.0f)
        {
            render_color.x = 1.0f;
        }
        if (render_color.y > 1.0f)
        {
            render_color.y = 1.0f;
        }
        if (render_color.z > 1.0f)
        {
            render_color.z = 1.0f;
        }

        rc->thread_station->pixels[index_2d_to_1d(x, y, rc->settings->width)] = render_color;

        if (i % 100 == 0 && rc->index == 3)
        {
            printf("[%d] %d / %d\n", rc->index, i - rc->px_start, rc->px_end - rc->px_start);
            fflush(stdout);
        }
    }
}

void render(Renderer *renderer)
{
    pthread_mutex_lock(&renderer->thread_station.finished_mutex);
    renderer->thread_station.finished_count = 0;
    pthread_mutex_unlock(&renderer->thread_station.finished_mutex);

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        RenderChunk *rc = renderer->render_chunks[i];
        pthread_mutex_lock(&rc->wake_mutex);
        rc->ready = 1;
        pthread_cond_signal(&rc->wake_cond);
        pthread_mutex_unlock(&rc->wake_mutex);
    }

    pthread_mutex_lock(&renderer->thread_station.finished_mutex);
    while (renderer->thread_station.finished_count < renderer->thread_station.thread_count)
    {
        pthread_cond_wait(&renderer->thread_station.thread_done_cond, &renderer->thread_station.finished_mutex);
    }
    pthread_mutex_unlock(&renderer->thread_station.finished_mutex);
}
