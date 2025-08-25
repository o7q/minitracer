
#include "math_utils.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

static __thread unsigned int seed = 0;

void random_init()
{
    srand((unsigned int)time(NULL));
}

void random_thread_init(int thread_id)
{
    seed = (unsigned int)time(NULL) + thread_id;
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

Vec3 random_hemi_normal_distribution(Vec3 normal)
{
    float u1 = (random_float_thread() + 1.0f) * 0.5f;
    float u2 = (random_float_thread() + 1.0f) * 0.5f;

    float cos_theta = u1;
    float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    float phi = 2.0f * 3.14159265f * u2;

    Vec3 local_dir = {
        sin_theta * cosf(phi),
        sin_theta * sinf(phi),
        cos_theta};

    return vec_mult_v(local_dir, (vec_dot(normal, local_dir) < 0 ? -1 : 1));
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