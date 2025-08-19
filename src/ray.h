#ifndef RAY_H
#define RAY_H

#include "vec.h"
#include "mesh.h"
#include "world.h"

typedef struct
{
    Vec3 origin;
    Vec3 direction;
} Ray3;

Vec3 ray_at(const Ray3 *ray, float t);
float ray_hit_tri(const Ray3* ray, const Tri3* tri);
Vec3 ray_color(const Ray3* ray, const World* world);

#endif