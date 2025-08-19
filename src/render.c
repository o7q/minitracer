#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "ray.h"
#include "camera.h"

void render(const Cam3 *camera, const World *world)
{
    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)camera->width / camera->height);

    float pixel_delta_u = viewport_width / camera->width;
    float pixel_delta_v = viewport_height / camera->height;

    Vec3 viewport_top_left = vec_sub(camera->pos, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, camera->fov});
    Vec3 pixel00_loc = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    int max_bounce = 1;

    for (int y = 0; y < camera->height; ++y)
    {
        for (int x = 0; x < camera->width; ++x)
        {
            Vec3 pixel_center = vec_add(pixel00_loc, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});
            Vec3 ray_direction = vec_sub(pixel_center, camera->pos);

            Ray3 ray;
            ray.origin = camera->pos;
            ray.direction = ray_direction;

            float t_lowest = 100000.0f;
            int intersection = 0;

            Vec3 unit_direction = vec_unit(ray.direction);
            float a = 0.5 * (unit_direction.y + 1.0);
            Vec3 color  = vec_add((Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (Vec3){0.5 * a, 0.7 * a, 1.0 * a});

            for (int i = 0; i < world->mesh_index; ++i)
            {
                for (int j = 0; j < world->meshes[i]->tri_index; ++j)
                {
                    Tri3 tri = world->meshes[i]->tris[j];
                    tri.p1 = vec_add(tri.p1, tri.offset);
                    tri.p2 = vec_add(tri.p2, tri.offset);
                    tri.p3 = vec_add(tri.p3, tri.offset);

                    float t = ray_hit_tri(&ray, &tri);
                    if (t >= 0.0f && t < t_lowest)
                    {
                        t_lowest = t;

                        color = tri.mat.color;
                    }
                }
            }

            DrawPixel(x, y, (Color){color.x * 255, color.y * 255, color.z * 255, 255});
        }
    }
}