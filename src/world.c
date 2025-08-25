#include "world.h"

#include <stdlib.h>

World *world_create(unsigned int max_objects)
{
    World *world = (World *)malloc(sizeof(World));
    world->objects = (void **)malloc(sizeof(void *) * max_objects);
    world->objects_track = (ObjectType *)malloc(sizeof(ObjectType) * max_objects);
    world->object_index = 0;
    world->max_objects = max_objects;
    return world;
}

void world_add_object(World *world, void *object, ObjectType object_type)
{
    if (world->object_index >= world->max_objects)
    {
        return;
    }

    world->objects[world->object_index] = object;
    world->objects_track[world->object_index] = object_type;
    ++world->object_index;
}

void world_delete(World *world)
{
    if (!world)
    {
        return;
    }

    if (world->objects)
    {
        for (unsigned int i = 0; i < world->object_index; ++i)
        {
            if (!world->objects[i])
            {
                continue;
            }

            switch (world->objects_track[i])
            {
            case OBJECT_TRI:
                tri_delete(world->objects[i]);
                break;
            case OBJECT_MESH:
                mesh_delete(world->objects[i]);
                break;
            case OBJECT_SPHERE:
                sphere_delete(world->objects[i]);
                break;
            }
        }
        free(world->objects);
    }

    if (world->objects_track)
    {
        free(world->objects_track);
    }

    free(world);
}
