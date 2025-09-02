#include "instance.h"

#include <stdlib.h>

RaylibInstance raylib_instance_create(int width, int height, int render_scale)
{
    int window_scale = 4 / render_scale;
    if (window_scale < 1)
    {
        window_scale = 1;
    }

    InitWindow(width * render_scale * window_scale, height * render_scale * window_scale, "minitracer");
    SetWindowPosition(2500, 200);

    int texture_width = width * render_scale;
    int texture_height = height * render_scale;

    RaylibInstance instance;
    instance.window_scale = window_scale;
    instance.pixels = (Color *)malloc(sizeof(Color) * texture_width * texture_height);
    instance.target = LoadRenderTexture(texture_width, texture_height);

    return instance;
}

void raylib_instance_delete(RaylibInstance instance)
{
    if (instance.pixels)
    {
        free(instance.pixels);
    }

    UnloadRenderTexture(instance.target);
    CloseWindow();
}
