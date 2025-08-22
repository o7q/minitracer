#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

typedef struct Cam
{
    Vec3 position;
    Vec3 rotation;

    float fov;
} Cam;

Cam camera_create();

#endif