#include "ray.h"

#include <math.h>

#include "vec.h"
#include "math_utils.h"

Vec3 ray_at(const Ray3 *ray, float t)
{
    return (Vec3){
        ray->origin.x + t * ray->direction.x,
        ray->origin.y + t * ray->direction.y,
        ray->origin.z + t * ray->direction.z};
}

// Möller–Trumbore intersection
// thanks Sebastian Lague for the implementation: https://youtu.be/Qz0KTGYJtUk?t=1418
Ray3Hit ray_hit_tri(const Ray3 *ray, const TriObj *tri)
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

Ray3Hit ray_hit_sphere(const Ray3 *ray, const SphereObj *sphere)
{
    Ray3Hit hit;
    hit.hit = 0;

    Vec3 oc = vec_sub(sphere->position, ray->origin);
    float a = vec_length_squared(ray->direction);
    float h = vec_dot(ray->direction, oc);
    float c = vec_length_squared(oc) - sphere->radius * sphere->radius;
    float discriminant = h * h - a * c;
    if (discriminant >= 0)
    {
        float t = (h - sqrtf(discriminant)) / a;
        if (t >= 0.0f)
        {
            hit.hit = 1;
            hit.t = t;
            hit.pos = ray_at(ray, hit.t);
            hit.normal = vec_normalize(vec_sub(hit.pos, sphere->position));
        }
    }

    return hit;
}

void ray_bounce(Ray3 *ray, Ray3Hit *hit, Mat *mat)
{
    // apply color
    ray->color = vec_mult(ray->color, mat->color);

    Vec3 emittedLight = vec_mult_v(mat->emission, mat->emission_strength); 
    ray->radiance = vec_add(ray->radiance, vec_mult(emittedLight, ray->color));

    // reflective bounce
    Vec3 i_n = vec_normalize(ray->direction);
    float d = vec_dot(i_n, hit->normal);
    ray->origin = hit->pos;
    ray->direction = vec_sub(i_n, vec_mult_v(hit->normal, 2.0f * d));

    // scatter from roughness
    ray->direction = vec_lerp(ray->direction, random_hemi(hit->normal), mat->roughness);
}