#include "hit.h"

#include <math.h>

float hit_sphere(const Sphere *sphere, const Ray3 *ray)
{
    Vec3 oc = vec_sub(sphere->pos, ray->origin);
    float a = vec_dot(ray->direction, ray->direction);
    float b = -2.0 * vec_dot(ray->direction, oc);
    float c = vec_dot(oc, oc) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrtf(discriminant)) / (2.0f * a);
    }
}