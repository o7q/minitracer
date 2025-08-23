#include "material.h"

Mat material_create()
{
    Mat mat;
    mat.color = (Vec3){1.0f, 1.0f, 1.0f};
    mat.emission = (Vec3){1.0f, 1.0f, 1.0f};
    mat.emission_strength = 0.0f;
    mat.roughness = 1.0f;
    return mat;
}
