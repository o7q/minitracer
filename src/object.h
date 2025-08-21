#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"
#include "material.h"

typedef enum ObjectType
{
    OBJECT_MESH,
    OBJECT_SPHERE
} ObjectType;

typedef struct Tri3
{
    Vec3 p1, p2, p3;
    Vec3 p1_n, p2_n, p3_n;
    Vec3 normal;

    Mat mat;
} Tri3;

typedef struct Mesh3
{
    Tri3 *tris;
    Vec3 position;

    unsigned int max_tris;
    unsigned int tri_index;
} Mesh3;

typedef struct Sphere3
{
    Vec3 position;
    float radius;

    Mat mat;
} Sphere3;

Mesh3 *mesh_create(unsigned int max_tris);
void mesh_add_tri(Mesh3 *mesh, Tri3 tri);
void mesh_delete(Mesh3 *mesh);

void tri_init_normals(Tri3 *tri);
Tri3 tri_create(Vec3 p1, Vec3 p2, Vec3 p3);

#endif