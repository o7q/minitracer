#include "input.h"

#include <raylib.h>
#include <math.h>

void raylib_handle_debug_input(RaylibInstance instance, MT_Camera *camera)
{
    if (IsKeyPressed(KEY_ONE))
    {
        printf("Camera Position: %.3f %.3f %.3f\nCamera Rotation: %.3f %.3f %.3f\nCamera FOV: %.3f\n\n",
               camera->position.x,
               camera->position.y,
               camera->position.z,
               camera->rotation.x,
               camera->rotation.y,
               camera->rotation.z,
               camera->fov);
        fflush(stdout);
    }
    if (IsKeyPressed(KEY_TWO))
    {
        mt_bmp_write("screenshot.bmp", instance.render_pixels, instance.display_width, instance.display_height);
    }
}

int raylib_handle_movement(MT_Camera *camera, float speed)
{
    int movement_occurred = 0;

    if (IsKeyDown(KEY_SPACE))
    {
        camera->position.y -= speed;
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        camera->position.y += speed;
        movement_occurred = 1;
    }

    if (IsKeyDown(KEY_W))
    {
        camera->position.z -= speed * cosf(camera->rotation.y);
        camera->position.x -= speed * sinf(camera->rotation.y);
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_S))
    {
        camera->position.z += speed * cosf(camera->rotation.y);
        camera->position.x += speed * sinf(camera->rotation.y);
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_A))
    {
        camera->position.x -= speed * cosf(camera->rotation.y);
        camera->position.z += speed * sinf(camera->rotation.y);
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_D))
    {
        camera->position.x += speed * cosf(camera->rotation.y);
        camera->position.z -= speed * sinf(camera->rotation.y);
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_Z))
    {
        camera->fov += 0.25f;
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_X))
    {
        if (camera->fov > 0.25f)
        {
            camera->fov -= 0.25f;
            movement_occurred = 1;
        }
    }
    if (IsKeyDown(KEY_UP))
    {
        camera->rotation.x -= speed / camera->fov / 2.0f;
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        camera->rotation.x += speed / camera->fov / 2.0f;
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        camera->rotation.y += speed / camera->fov / 2.0f;
        movement_occurred = 1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        camera->rotation.y -= speed / camera->fov / 2.0f;
        movement_occurred = 1;
    }

    return movement_occurred;
}
