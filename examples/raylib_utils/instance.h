#ifndef UTILS_INIT_H
#define UTILS_INIT_H

#include <raylib.h>
#include "../../minitracer.h"

typedef struct RaylibInstance
{
    Color *display_pixels;
    MT_Vec3 *render_pixels;
    RenderTexture2D target;
    int display_width, display_height;
    int window_scale;
} RaylibInstance;

RaylibInstance raylib_instance_create(MT_Vec3 *pixels_ptr, int render_width, int render_height, int render_scale, int position_x, int position_y);
void raylib_instance_delete(RaylibInstance instance);

#endif
