#include "object.h"

#include <stdlib.h>

MeshObj *mesh_create(unsigned int max_tris)
{
    MeshObj *mesh = (MeshObj *)malloc(sizeof(MeshObj));
    mesh->tris = malloc(sizeof(TriObj) * max_tris);
    mesh->tri_index = 0;
    mesh->max_tris = max_tris;
    return mesh;
}

void mesh_add_tri(MeshObj *mesh, TriObj tri)
{
    if (mesh->tri_index >= mesh->max_tris)
    {
        return;
    }

    mesh->tris[mesh->tri_index] = tri;
    ++mesh->tri_index;
}

void mesh_delete(MeshObj *mesh)
{
    if (!mesh)
    {
        return;
    }

    if (mesh->tris)
    {
        free(mesh->tris);
    }

    free(mesh);
}

void tri_init_normals(TriObj *tri)
{
    Vec3 u = vec_sub(tri->p2, tri->p1);
    Vec3 v = vec_sub(tri->p3, tri->p1);

    Vec3 normal = vec_normalize(vec_cross(u, v));
    tri->normal = normal;
    tri->p1_n = normal;
    tri->p2_n = normal;
    tri->p3_n = normal;
}

TriObj tri_create(Vec3 p1, Vec3 p2, Vec3 p3)
{
    TriObj tri;
    tri.p1 = p1;
    tri.p2 = p2;
    tri.p3 = p3;
    tri.mat = material_create();

    tri_init_normals(&tri);

    return tri;
}