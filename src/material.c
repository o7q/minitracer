#include "material.h"

Mat material_create()
{
    Mat mat;
    mat.color = (Vec3){1, 1, 1};
    mat.emission = (Vec3){1, 1, 1};
    mat.emission_strength = 0;
    mat.roughness = 1.0f;
    return mat;
}