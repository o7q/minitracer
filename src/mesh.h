#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"
#include "material.h"

typedef struct
{
    Vec3 p1, p2, p3;
    Vec3 offset;
    Vec3 normal;
} Tri3;

typedef struct
{
    Tri3 *tris;
    Vec3 position;
    Mat mat;

    unsigned int max_tris;
    unsigned int tri_index;
} Mesh3;

Mesh3 *mesh_create(unsigned int max_tris);
void mesh_add_tri(Mesh3 *mesh, Tri3 tri);
void mesh_delete(Mesh3 *mesh);

#endif