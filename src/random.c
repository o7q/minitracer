
#include "random.h"

#include <stdlib.h>
#include <time.h>

void random_init()
{
    srand((unsigned int)time(NULL));
}

float random_float()
{
    return rand() / (float)RAND_MAX;
}

Vec3 randomHemi(Vec3 normal)
{
    Vec3 dir = (Vec3){random_float(), random_float(), random_float()};
    return vec_mult_v(dir, (vec_dot(normal, dir) < 0 ? -1 : 1));
}