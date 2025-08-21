#include "ray.h"

#include "vec.h"

#include <math.h>

Vec3 ray_at(const Ray3 *ray, float t)
{
    return (Vec3){
        ray->origin.x + t * ray->direction.x,
        ray->origin.y + t * ray->direction.y,
        ray->origin.z + t * ray->direction.z};
}

// Möller–Trumbore intersection
// thanks Sebastian Lague for the implementation: https://youtu.be/Qz0KTGYJtUk?t=1418
Ray3Hit ray_hit_tri(const Ray3 *ray, const Tri3 *tri)
{
    Ray3Hit hit;

    Vec3 edge1 = vec_sub(tri->p2, tri->p1);
    Vec3 edge2 = vec_sub(tri->p3, tri->p1);
    Vec3 normal = vec_cross(edge1, edge2);

    float det = -vec_dot(ray->direction, normal);
    float invdet = 1.0f / det;

    Vec3 ao = vec_sub(ray->origin, tri->p1);
    Vec3 dao = vec_cross(ao, ray->direction);

    float dst = vec_dot(ao, normal) * invdet;
    float u = vec_dot(edge2, dao) * invdet;
    float v = -vec_dot(edge1, dao) * invdet;
    float w = 1.0f - u - v;

    hit.hit = det >= 1e-6 && dst >= 0.0f && u >= 0.0f && v >= 0.0f && w >= 0.0f ? 1 : 0;
    hit.pos = ray_at(ray, dst);
    hit.normal = vec_normalize((Vec3){tri->p1_n.x * w + tri->p2_n.x * u + tri->p3_n.x * v,
                                      tri->p1_n.y * w + tri->p2_n.y * u + tri->p3_n.y * v,
                                      tri->p1_n.z * w + tri->p2_n.z * u + tri->p3_n.z * v});
    hit.t = dst;

    return hit;
}

Ray3Hit ray_hit_sphere(const Ray3 *ray, const Sphere3 *sphere)
{
    Ray3Hit hit;

    Vec3 oc = vec_sub(sphere->position, ray->origin);
    float a = vec_length_squared(ray->direction);
    float h = vec_dot(ray->direction, oc);
    float c = vec_length_squared(oc) - sphere->radius * sphere->radius;
    float discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        hit.hit = 0;
    }
    else
    {
        hit.t = (h - sqrtf(discriminant)) / a;
        if (hit.t < 0.0f)
        {
            hit.hit = 0;
        }
        else
        {
            hit.hit = 1;
            hit.pos = ray_at(ray, hit.t);
            hit.normal = vec_sub(hit.pos, sphere->position);
            hit.normal = vec_normalize(hit.normal);
        }
    }

    return hit;
}