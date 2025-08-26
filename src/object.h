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

    Mat *mat;
} TriObj;

typedef struct MeshObj
{
    TriObj **tris;

    Vec3 offset_history;

    unsigned int max_tris;
    unsigned int tri_index;
} MeshObj;

typedef struct SphereObj
{
    Vec3 position;
    float radius;

    Mat *mat;
} SphereObj;

void tri_init_normals(TriObj *tri);
TriObj *tri_create(Vec3 p1, Vec3 p2, Vec3 p3, Mat *mat);
void tri_delete(TriObj *tri);

MeshObj *mesh_create(unsigned int max_tris);
MeshObj *mesh_create_plane(Mat *material);
MeshObj *mesh_create_cube(Mat *material);
void mesh_add_tri(MeshObj *mesh, TriObj *tri);
void mesh_init_normals(MeshObj *mesh);
void mesh_move(MeshObj *mesh, Vec3 position);
void mesh_rotate(MeshObj *mesh, Vec3 rotation);
void mesh_scale(MeshObj *mesh, Vec3 scale);
void mesh_delete(MeshObj *mesh);

SphereObj *sphere_create(Vec3 position, float radius, Mat *mat);
void sphere_delete(SphereObj *sphere);

#endif