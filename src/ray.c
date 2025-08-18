#include "ray.h"

#include "vec.h"

Vec3 ray_at(struct Ray3 ray, float t)
{
    return (Vec3){
        ray.origin.x + t * ray.direction.x,
        ray.origin.y + t * ray.direction.y,
        ray.origin.z + t * ray.direction.z};
}
