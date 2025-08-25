#include "material.h"

#include <stdlib.h>

Mat *material_create()
{
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    mat->color = (Vec3){1.0f, 1.0f, 1.0f};
    mat->emission = (Vec3){1.0f, 1.0f, 1.0f};
    mat->emission_strength = 0.0f;
    mat->roughness = 1.0f;
    return mat;
}

void material_delete(Mat *material)
{
    if (material)
    {
        free(material);
    }
}
