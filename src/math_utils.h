#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "vec.h"

void random_init();
void random_thread_init(int thread_id);
float random_float();
float random_float_thread();

Vec3 random_hemi(Vec3 normal);

float lerp(float a, float b, float t);
unsigned int index_2d_to_1d(unsigned int x, unsigned int y, unsigned int width);

#endif
