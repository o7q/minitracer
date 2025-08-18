#ifndef RAY_H
#define RAY_H

#include "vec.h"

typedef struct Ray3
{
    Vec3 origin;
    Vec3 direction;
} Ray3;

Vec3 ray_at(Ray3 ray, float t);

#endif