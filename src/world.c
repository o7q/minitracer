#include "world.h"

#include <stdlib.h>

World *world_create(unsigned int max_meshes)
{
    World *world = malloc(sizeof(World));
    world->meshes = (Mesh3 **)malloc(sizeof(Mesh3 *) * max_meshes);
    world->mesh_index = 0;
    world->max_meshes = max_meshes;
    return world;
}

void world_add_mesh(World *world, Mesh3 *mesh)
{
    if (world->mesh_index >= world->max_meshes)
    {
        return;
    }

    world->meshes[world->mesh_index] = mesh;
    ++world->mesh_index;
}

void world_delete(World *world)
{
    if (!world)
    {
        return;
    }

    if (world->meshes)
    {
        for (unsigned int i = 0; i < world->mesh_index; ++i)
        {
            if (world->meshes[i])
            {
                free(world->meshes[i]);
            }
        }
        free(world->meshes);
    }

    free(world);
}