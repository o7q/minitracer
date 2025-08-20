#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec.h"

typedef struct Mat
{
    Vec3 color;
    float roughness;
} Mat;

#endif