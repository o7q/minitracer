#include "mesh.h"

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

Vec3 tri_normal(const Tri3 *tri)
{
    Vec3 u = vec_sub(tri->p2, tri->p1);
    Vec3 v = vec_sub(tri->p3, tri->p1);

    Vec3 n = (Vec3){u.y * v.z - u.z * v.y,
                    u.z * v.x - u.x * v.z,
                    u.x * v.y - u.y * v.x};

    return vec_unit(n);
}