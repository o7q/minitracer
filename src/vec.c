#include "vec.h"

#include <math.h>

#include "math_utils.h"

Vec3 vec_add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 vec_sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 vec_mult(Vec3 a, Vec3 b) { return (Vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }
Vec3 vec_div(Vec3 a, Vec3 b) { return (Vec3){a.x / b.x, a.y / b.y, a.z / b.z}; }

Vec3 vec_add_v(Vec3 a, float v) { return (Vec3){a.x + v, a.y + v, a.z + v}; }
Vec3 vec_sub_v(Vec3 a, float v) { return (Vec3){a.x - v, a.y - v, a.z - v}; }
Vec3 vec_mult_v(Vec3 a, float v) { return (Vec3){a.x * v, a.y * v, a.z * v}; }
Vec3 vec_div_v(Vec3 a, float v) { return (Vec3){a.x / v, a.y / v, a.z / v}; }

float vec_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec_cross(Vec3 a, Vec3 b)
{
    return (Vec3){a.y * b.z - a.z * b.y,
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x};
}

float vec_length_squared(Vec3 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float vec_length(Vec3 a)
{
    return sqrtf(vec_length_squared(a));
}

Vec3 vec_normalize(Vec3 a)
{
    float m = vec_length(a);
    return (Vec3){a.x / m, a.y / m, a.z / m};
}

Vec3 vec_lerp(Vec3 a, Vec3 b, float t)
{
    return (Vec3){lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)};
}

void vec_rotate(Vec3 *point, Vec3 rotation)
{
    // x
    float p1_rotated_y = point->y * cosf(rotation.x) - point->z * sinf(rotation.x);
    float p1_rotated_z = point->y * sinf(rotation.x) + point->z * cosf(rotation.x);

    float p1_rotated_x = point->x * cosf(rotation.y) + p1_rotated_z * sinf(rotation.y);
    // y
    float p1_rotated_z_2 = p1_rotated_z * cosf(rotation.y) - point->x * sinf(rotation.y);

    float p1_rotated_x_2 = p1_rotated_x * cosf(rotation.z) - p1_rotated_y * sinf(rotation.z);
    float p1_rotated_y_2 = p1_rotated_x * sinf(rotation.z) + p1_rotated_y * cosf(rotation.z);
    // z

    point->x = p1_rotated_x_2;
    point->y = p1_rotated_y_2;
    point->z = p1_rotated_z_2;
}