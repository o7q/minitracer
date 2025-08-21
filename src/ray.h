#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include "object.h"
#include "world.h"

typedef struct Ray3
{
    Vec3 origin;
    Vec3 direction;
    Vec3 color;
} Ray3;

typedef struct Ray3Hit
{
    Vec3 pos;
    Vec3 normal;
    int hit;
    float t;
} Ray3Hit;

Vec3 ray_at(const Ray3 *ray, float t);
Ray3Hit ray_hit_tri(const Ray3 *ray, const Tri3 *tri);

#endif