#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

typedef struct Cam3 {
    Vec3 pos;
    Vec3 rot;
    float fov;
    int width;
    int height;
    float viewport_width;
} Cam3;

#endif