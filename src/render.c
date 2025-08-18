#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "ray.h"
#include "hit.h"
#include "camera.h"

void render(const Camera3 *camera, const Sphere *sphere)
{
    // printf("\033[H"); // clear screen

    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)camera->width / camera->height);

    float pixel_delta_u = viewport_width / camera->width;
    float pixel_delta_v = viewport_height / camera->height;

    Vec3 viewport_top_left = vec_sub(camera->pos, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, camera->fov});
    Vec3 pixel00_loc = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    int size = (camera->width * 2 + 1) * camera->height;
    char *screen = malloc(size);
    if (!screen)
    {
        printf("%s", "Error!\n");
    }

    for (int y = 0; y < camera->height; ++y)
    {
        for (int x = 0; x < camera->width; ++x)
        {
            Vec3 pixel_center = vec_add(pixel00_loc, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});
            Vec3 ray_direction = vec_sub(pixel_center, camera->pos);

            Ray3 ray;
            ray.origin = camera->pos;
            ray.direction = ray_direction;

            int index = y * (camera->width * 2 + 1) + x * 2;

            if (hit_sphere(sphere, &ray))
            {
                // screen[index] = '[';
                // screen[index + 1] = ']';
                DrawPixel(x, y, RAYWHITE);
            }
            else
            {
                // screen[index] = ' ';
                // screen[index + 1] = ' ';
                DrawPixel(x, y, BLACK);
            }
        }
        // screen[y * (camera->width * 2 + 1) + camera->width * 2] = '\n';
    }

    fwrite(screen, 1, size, stdout);

    free(screen);
}