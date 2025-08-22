#include "camera.h"

Cam camera_create()
{
    Cam cam;
    cam.position = (Vec3){0, 0, 0};
    cam.rotation = (Vec3){0, 0, 0};
    cam.fov = 1.0f;
    return cam;
}