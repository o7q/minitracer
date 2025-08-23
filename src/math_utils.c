
#include "math_utils.h"

#include <stdlib.h>
#include <time.h>
#include <limits.h>

static __thread unsigned int seed = 0;

void random_init()
{
    srand((unsigned int)time(NULL));
}

void random_thread_init(int thread_id)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    seed = ts.tv_nsec + thread_id;
}

float random_float()
{
    return 2 * (rand() / (float)RAND_MAX) - 1;
}

float random_float_thread()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return 2.0f * (seed / (float)UINT_MAX) - 1.0f;
}

Vec3 random_hemi(Vec3 normal)
{
    Vec3 dir = (Vec3){random_float_thread(), random_float_thread(), random_float_thread()};
    return vec_mult_v(dir, (vec_dot(normal, dir) < 0 ? -1 : 1));
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

unsigned int index_2d_to_1d(unsigned int x, unsigned int y, unsigned int width)
{
    return y * width + x;
}