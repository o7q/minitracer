#ifndef HIT_H
#define HIT_H

#include "ray.h"
#include "object.h"

float hit_sphere(const Sphere* sphere, const Ray3* ray);

#endif