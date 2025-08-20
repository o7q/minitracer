#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"

typedef struct World
{
    Mesh3 **meshes;
    unsigned int mesh_index;
    unsigned int max_meshes;

} World;

World *world_create(unsigned int max_meshes);
void world_add_mesh(World *world, Mesh3 *mesh);
void world_delete(World *world);

#endif