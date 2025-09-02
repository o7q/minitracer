#ifndef UTILS_INIT_H
#define UTILS_INIT_H

#include <raylib.h>

typedef struct RaylibInstance
{
    Color *pixels;
    RenderTexture2D target;
    int window_scale;
} RaylibInstance;

RaylibInstance raylib_instance_create(int width, int height, int render_scale, int position_x, int position_y);
void raylib_instance_delete(RaylibInstance instance);

#endif
