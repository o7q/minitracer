#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec.h"

typedef struct Mat
{
    Vec3 color;
    Vec3 emission;
    float emission_strength;
    float roughness;
} Mat;

Mat *material_create();
void material_delete(Mat *material);

#endif