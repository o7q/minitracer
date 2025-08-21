#ifndef VEC3_H
#define VEC3_H

typedef struct Vec3
{
    float x, y, z;
} Vec3;

Vec3 vec_add(Vec3 a, Vec3 b);
Vec3 vec_sub(Vec3 a, Vec3 b);
Vec3 vec_mult(Vec3 a, Vec3 b);
Vec3 vec_div(Vec3 a, Vec3 b);

Vec3 vec_add_v(Vec3 a, float v);
Vec3 vec_sub_v(Vec3 a, float v);
Vec3 vec_mult_v(Vec3 a, float v);
Vec3 vec_div_v(Vec3 a, float v);

float vec_dot(Vec3 a, Vec3 b);
Vec3 vec_cross(Vec3 a, Vec3 b);

float vec_length_squared(Vec3 a);
float vec_length(Vec3 a);
Vec3 vec_normalize(Vec3 a);

#endif