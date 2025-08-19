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