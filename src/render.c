#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "ray.h"
#include "camera.h"

#include <math.h>

#include "random.h"

#include <float.h>

void render_handle_mesh(Ray3 *ray, Mesh3 *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        Tri3 tri = mesh->tris[i];

        Ray3Hit ray_hit = ray_hit_tri(ray, &tri);
        if (ray_hit.hit && ray_hit.t < *t_lowest)
        {
            hit_info->hit = 1;
            hit_info->pos = ray_hit.pos;
            hit_info->normal = ray_hit.normal;
            *t_lowest = ray_hit.t;

            *hit_mat = mesh->tris[i].mat;
        }
    }
}

void render_handle_sphere(Ray3 *ray, Sphere3 *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_sphere(ray, sphere);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = sphere->mat; // (Vec3){0.5f * (hit_info->normal.x + 1), 0.5f * (hit_info->normal.y + 1), 0.5f * (hit_info->normal.z + 1)};
    }
}

void render(const Cam3 *camera, const World *world)
{
    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)camera->width / camera->height);

    float pixel_delta_u = viewport_width / camera->width;
    float pixel_delta_v = viewport_height / camera->height;

    Vec3 viewport_top_left = vec_sub(camera->pos, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, camera->fov});
    Vec3 pixel00_pos = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    int max_bounces = 4;
    int samples = 1;

    for (int y = 0; y < camera->height; ++y)
    {
        for (int x = 0; x < camera->width; ++x)
        {
            Vec3 pixel_center = vec_add(pixel00_pos, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});

            Vec3 ray_direction = vec_sub(pixel_center, camera->pos);

            Vec3 ray_dir_normal = vec_normalize(ray_direction);

            Vec3 ray_rotated_normal = ray_dir_normal;

            float temp_y = ray_dir_normal.y * cosf(camera->rot.x) - ray_dir_normal.z * sinf(camera->rot.x);
            float temp_z = ray_dir_normal.y * sinf(camera->rot.x) + ray_dir_normal.z * cosf(camera->rot.x);

            ray_rotated_normal.x = ray_dir_normal.x * cosf(camera->rot.y) + temp_z * sinf(camera->rot.y);
            ray_rotated_normal.y = temp_y;
            ray_rotated_normal.z = temp_z * cosf(camera->rot.y) - ray_dir_normal.x * sinf(camera->rot.y);

            ray_direction = vec_mult_v(ray_rotated_normal, vec_length(ray_direction));

            Vec3 render_color = (Vec3){0, 0, 0};

            for (int i = 0; i < samples; ++i)
            {
                Ray3 ray;
                ray.origin = camera->pos;
                ray.direction = ray_direction;
                ray.color = (Vec3){1, 1, 1};

                for (int j = 0; j < max_bounces; ++j)
                {
                    float t_lowest = FLT_MAX;

                    Ray3Hit hit_info;
                    hit_info.hit = 0;
                    Mat hit_mat;

                    for (int k = 0; k < world->object_index; ++k)
                    {
                        switch (world->objects_track[k])
                        {
                        case OBJECT_MESH:
                            render_handle_mesh(&ray, (Mesh3 *)world->objects[k], &hit_info, &hit_mat, &t_lowest);
                            break;
                        case OBJECT_SPHERE:
                            render_handle_sphere(&ray, (Sphere3 *)world->objects[k], &hit_info, &hit_mat, &t_lowest);
                            break;
                        }
                    }

                    if (hit_info.hit)
                    {
                        ray.color = vec_mult(ray.color, hit_mat.color);
                        Vec3 i_n = vec_normalize(ray.direction);
                        float d = vec_dot(i_n, hit_info.normal);
                        ray.origin = hit_info.pos;
                        ray.direction = vec_sub(i_n, vec_mult_v(hit_info.normal, 2.0f * d)); //(Vec3){hit_normal.x + random_float() / 10.0f, hit_normal.y + random_float() / 10.0f, hit_normal.z + random_float() / 10.0f};
                        // ray.direction = (Vec3){ray.direction.x + random_float() / 10.0f, ray.direction.y + random_float() / 10.0f, ray.direction.z + random_float() / 10.0f};
                    }
                    else
                    {
                        Vec3 unit_direction = vec_normalize(ray.direction);
                        float a = 0.5 * (unit_direction.y + 1.0);
                        ray.color = vec_mult(ray.color, vec_add((Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (Vec3){0.5 * a, 0.7 * a, 1.0 * a}));

                        break;
                    }
                }

                render_color = vec_add(render_color, ray.color);
            }

            render_color = vec_div_v(render_color, (float)samples);

            DrawPixel(x, y, (Color){render_color.x * 255, render_color.y * 255, render_color.z * 255, 255});
        }
    }
}
