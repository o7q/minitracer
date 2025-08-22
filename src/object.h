#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"
#include "material.h"

typedef enum ObjectType
{
    OBJECT_TRI,
    OBJECT_MESH,
    OBJECT_SPHERE
} ObjectType;

typedef struct TriObj
{
    Vec3 p1, p2, p3;
    Vec3 p1_n, p2_n, p3_n;
    Vec3 normal;

    Mat mat;
} TriObj;

typedef struct MeshObj
{
    TriObj *tris;
    Vec3 position;

    unsigned int max_tris;
    unsigned int tri_index;
} MeshObj;

typedef struct SphereObj
{
    Vec3 position;
    float radius;

    Mat mat;
} SphereObj;

MeshObj *mesh_create(unsigned int max_tris);
void mesh_add_tri(MeshObj *mesh, TriObj tri);
void mesh_delete(MeshObj *mesh);

void tri_init_normals(TriObj *tri);
TriObj tri_create(Vec3 p1, Vec3 p2, Vec3 p3);

#endif