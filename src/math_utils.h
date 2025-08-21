#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "vec.h"

void random_init();
float random_float();

Vec3 random_hemi(Vec3 normal);

float lerp(float a, float b, float t);

#endif
