#include "render.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <float.h>

#include "ray.h"
#include "camera.h"


#include "math_utils.h"


void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_tri(ray, tri);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = tri->mat;
    }
}

void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        render_handle_tri(ray, &mesh->tris[i], hit_info, hit_mat, t_lowest);
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

        *hit_mat = sphere->mat;
    }
}

void render(const Cam *camera, const World *world)
{
    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)camera->width / camera->height);

    float pixel_delta_u = viewport_width / camera->width;
    float pixel_delta_v = viewport_height / camera->height;

    Vec3 viewport_top_left = vec_sub(camera->position, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, camera->fov});
    Vec3 pixel00_pos = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    int max_bounces = 4;
    int samples = 20;

    for (int y = 0; y < camera->height; ++y)
    {
        for (int x = 0; x < camera->width; ++x)
        {
            Vec3 pixel_center = vec_add(pixel00_pos, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});

            Vec3 ray_direction = vec_sub(pixel_center, camera->position);

            Vec3 ray_dir_normal = vec_normalize(ray_direction);

            Vec3 ray_rotated_normal = ray_dir_normal;

            float temp_y = ray_dir_normal.y * cosf(camera->rotation.x) - ray_dir_normal.z * sinf(camera->rotation.x);
            float temp_z = ray_dir_normal.y * sinf(camera->rotation.x) + ray_dir_normal.z * cosf(camera->rotation.x);

            ray_rotated_normal.x = ray_dir_normal.x * cosf(camera->rotation.y) + temp_z * sinf(camera->rotation.y);
            ray_rotated_normal.y = temp_y;
            ray_rotated_normal.z = temp_z * cosf(camera->rotation.y) - ray_dir_normal.x * sinf(camera->rotation.y);

            ray_direction = vec_mult_v(ray_rotated_normal, vec_length(ray_direction));

            Vec3 render_color = (Vec3){0, 0, 0};

            for (int i = 0; i < samples; ++i)
            {
                Ray3 ray;
                ray.origin = camera->position;
                ray.direction = ray_direction;
                ray.color = (Vec3){1, 1, 1};
                ray.radiance = (Vec3){0, 0, 0};

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
                        case OBJECT_TRI:
                            render_handle_tri(&ray, (TriObj *)world->objects[k], &hit_info, &hit_mat, &t_lowest);
                            break;
                        case OBJECT_MESH:
                            render_handle_mesh(&ray, (MeshObj *)world->objects[k], &hit_info, &hit_mat, &t_lowest);
                            break;
                        case OBJECT_SPHERE:
                            render_handle_sphere(&ray, (SphereObj *)world->objects[k], &hit_info, &hit_mat, &t_lowest);
                            break;
                        }
                    }

                    if (hit_info.hit)
                    {
                        ray_bounce(&ray, &hit_info, &hit_mat);
                    }
                    else
                    {
                        break;
                    }
                }

                render_color = vec_add(render_color, ray.radiance);
            }

            render_color = vec_div_v(render_color, (float)samples);

            if (render_color.x > 1.0f)
                render_color.x = 1.0f;
            if (render_color.y > 1.0f)
                render_color.y = 1.0f;
            if (render_color.z > 1.0f)
                render_color.z = 1.0f;

            DrawPixel(x, y, (Color){render_color.x * 255, render_color.y * 255, render_color.z * 255, 255});
        }
    }
}
