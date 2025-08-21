#include "object.h"

#include <stdlib.h>

Mesh3 *mesh_create(unsigned int max_tris)
{
    Mesh3 *mesh = malloc(sizeof(Mesh3));
    mesh->tris = malloc(sizeof(Tri3) * max_tris);
    mesh->tri_index = 0;
    mesh->max_tris = max_tris;
    return mesh;
}

void mesh_add_tri(Mesh3 *mesh, Tri3 tri)
{
    if (mesh->tri_index >= mesh->max_tris)
    {
        return;
    }

    mesh->tris[mesh->tri_index] = tri;
    ++mesh->tri_index;
}

void mesh_delete(Mesh3 *mesh)
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

void tri_init_normals(Tri3 *tri)
{
    Vec3 u = vec_sub(tri->p2, tri->p1);
    Vec3 v = vec_sub(tri->p3, tri->p1);

    Vec3 normal = vec_normalize(vec_cross(u, v));
    tri->normal = normal;
    tri->p1_n = normal;
    tri->p2_n = normal;
    tri->p3_n = normal;
}

Tri3 tri_create(Vec3 p1, Vec3 p2, Vec3 p3)
{
    Tri3 tri;
    tri.p1 = p1;
    tri.p2 = p2;
    tri.p3 = p3;

    tri_init_normals(&tri);

    return tri;
}