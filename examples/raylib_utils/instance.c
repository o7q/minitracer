#include "instance.h"

#include <stdlib.h>

RaylibInstance raylib_instance_create(MT_Vec3 *pixels_ptr, int render_width, int render_height, int render_scale, int position_x, int position_y)
{
    int window_scale = 4 / render_scale;
    if (window_scale < 1)
    {
        window_scale = 1;
    }

    InitWindow(render_width * window_scale, render_height * window_scale, "minitracer");
    SetWindowPosition(position_x, position_y);

    RaylibInstance instance;
    instance.display_pixels = (Color *)malloc(sizeof(Color) * render_width * render_height);
    instance.render_pixels = pixels_ptr;
    instance.target = LoadRenderTexture(render_width, render_height);
    instance.display_width = render_width;
    instance.display_height = render_height;
    instance.window_scale = window_scale;

    return instance;
}

void raylib_instance_delete(RaylibInstance instance)
{
    if (instance.display_pixels)
    {
        free(instance.display_pixels);
    }

    if (instance.render_pixels)
    {
        free(instance.render_pixels);
    }

    UnloadRenderTexture(instance.target);
    CloseWindow();
}
