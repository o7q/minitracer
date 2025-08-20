#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "ray.h"
#include "camera.h"

#include <math.h>

#include "random.h"

void render(const Cam3 *camera, const World *world)
{
    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)camera->width / camera->height);

    float pixel_delta_u = viewport_width / camera->width;
    float pixel_delta_v = viewport_height / camera->height;

    Vec3 viewport_top_left = vec_sub(camera->pos, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, camera->fov});
    Vec3 pixel00_loc = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    int max_bounces = 5;
    int samples = 1;

    for (int y = 0; y < camera->height; ++y)
    {
        for (int x = 0; x < camera->width; ++x)
        {
            Vec3 pixel_center = vec_add(pixel00_loc, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});
            Vec3 ray_direction = vec_sub(pixel_center, camera->pos);

            Ray3 ray;
            ray.origin = camera->pos;
            ray.direction = ray_direction;

            Vec3 final_color = (Vec3){0, 0, 0};

            for (int i = 0; i < samples; ++i)
            {
                Vec3 color = (Vec3){1, 1, 1};

                for (int j = 0; j < max_bounces; ++j)
                {
                    float t_lowest = 100000.0f;
                    Tri3 nearest_tri;

                    int hit = 0;
                    Vec3 hit_normal;
                    Vec3 hit_position;

                    for (int k = 0; k < world->mesh_index; ++k)
                    {
                        for (int l = 0; l < world->meshes[k]->tri_index; ++l)
                        {
                            Tri3 tri = world->meshes[k]->tris[l];

                            Ray3Hit ray_hit = ray_hit_tri(&ray, &tri);
                            if (ray_hit.hit && ray_hit.t < t_lowest)
                            {
                                t_lowest = ray_hit.t;
                                nearest_tri = tri;

                                hit = 1;
                                hit_normal = ray_hit.normal;
                                hit_position = ray_hit.pos;
                            }
                        }
                    }

                    if (hit)
                    {
                        color = vec_mult(color, nearest_tri.mat.color);
                        Vec3 i_n = vec_unit(ray.direction);
                        float d = vec_dot(i_n, hit_normal);
                        ray.origin = hit_position;
                        ray.direction = vec_sub(i_n, vec_mult_v(hit_normal, 2.0f * d)); //(Vec3){hit_normal.x + random_float() / 10.0f, hit_normal.y + random_float() / 10.0f, hit_normal.z + random_float() / 10.0f};
                        // ray.direction = (Vec3){ray.direction.x + random_float() / 10.0f, ray.direction.y + random_float() / 10.0f, ray.direction.z + random_float() / 10.0f};
                    }
                    else
                    {
                        Vec3 unit_direction = vec_unit(ray.direction);
                        float a = 0.5 * (unit_direction.y + 1.0);
                        color = vec_mult(color, vec_add((Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (Vec3){0.5 * a, 0.7 * a, 1.0 * a}));

                        break;
                    }
                }

                final_color = vec_add(final_color, color);
            }

            final_color = vec_div_v(final_color, samples);

            DrawPixel(x, y, (Color){final_color.x * 255, final_color.y * 255, final_color.z * 255, 255});
        }
    }
}