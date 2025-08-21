
#include "math_utils.h"

#include <stdlib.h>
#include <time.h>

void random_init()
{
    srand((unsigned int)time(NULL));
}

float random_float()
{
    return 2* (rand() / (float)RAND_MAX) - 1;
}

Vec3 random_hemi(Vec3 normal)
{
    Vec3 dir = (Vec3){random_float(), random_float(), random_float()};
    return vec_mult_v(dir, (vec_dot(normal, dir) < 0 ? -1 : 1));
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}