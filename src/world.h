#ifndef WORLD_H
#define WORLD_H

#include "object.h"

typedef struct World
{
    void **objects;
    ObjectType *objects_track;

    unsigned int object_index;
    unsigned int max_objects;

} World;

World *world_create(unsigned int max_objects);
void world_add_object(World *world, void *object, ObjectType object_type);
void world_delete(World *world);

#endif