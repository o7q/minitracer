#include "ray.h"

#include "vec.h"

Vec3 ray_at(const Ray3 *ray, float t)
{
    return (Vec3){
        ray->origin.x + t * ray->direction.x,
        ray->origin.y + t * ray->direction.y,
        ray->origin.z + t * ray->direction.z};
}

Vec3 ray_color(const Ray3 *ray, const World *world)
{

}

float ray_hit_tri(const Ray3 *ray, const Tri3 *tri)
{
    const float epsilon = 0.000001;

    Vec3 edge1 = vec_sub(tri->p2, tri->p1);
    Vec3 edge2 = vec_sub(tri->p3, tri->p1);

    Vec3 cross_rayDir_edge2 = vec_cross(ray->direction, edge2);

    float det = vec_dot(edge1, cross_rayDir_edge2);

    if (det > -epsilon && det < epsilon)
    {
        return -1;
    }

    float inv_det = 1.0f / det;

    Vec3 orig_minus_vert0 = vec_sub(ray->origin, tri->p1);

    float baryU = vec_dot(orig_minus_vert0, cross_rayDir_edge2) * inv_det;

    if (baryU < 0.0 || baryU > 1.0f)
    {
        return -1;
    }

    Vec3 cross_oriMinusVert0_edge1 = vec_cross(orig_minus_vert0, edge1);

    float baryV = vec_dot(ray->direction, cross_oriMinusVert0_edge1) * inv_det;
    if (baryV < 0.0f || baryU + baryV > 1.0f)
    {
        return -1;
    }

    float t = vec_dot(edge2, cross_oriMinusVert0_edge1) * inv_det;

    if (t < 0.0f)
    {
        return -1;
    }

    return t;
}
