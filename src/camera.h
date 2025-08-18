#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

typedef struct Camera3 {
    Vec3 pos;
    float fov;
    int width;
    int height;
    float viewport_width;
} Camera3;

#endif