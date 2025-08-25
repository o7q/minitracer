#include "camera.h"

#include <stdlib.h>

Cam* camera_create()
{
    Cam *cam = (Cam*)malloc(sizeof(Cam));
    cam->position = (Vec3){0, 0, 0};
    cam->rotation = (Vec3){0, 0, 0};
    cam->fov = 1.0f;
    return cam;
}

void camera_delete(Cam* cam)
{
    if (cam)
    {
        free(cam);
    }
}
