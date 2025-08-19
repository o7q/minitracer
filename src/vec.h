#ifndef VEC3_H
#define VEC3_H

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vec_add(Vec3 a, Vec3 b);
Vec3 vec_sub(Vec3 a, Vec3 b);
Vec3 vec_mult(Vec3 a, Vec3 b);
Vec3 vec_div(Vec3 a, Vec3 b);

float vec_dot(Vec3 a, Vec3 b);
Vec3 vec_cross(Vec3 a, Vec3 b);

Vec3 vec_unit(Vec3 a);
float vec_length(Vec3 a);

#endif