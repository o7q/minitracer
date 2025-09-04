
#ifndef UTILS_INPUT_H
#define UTILS_INPUT_H

#include "instance.h"
#include "../../minitracer.h"

void raylib_handle_debug_input(RaylibInstance instance, MT_Camera* camera);
int raylib_handle_movement(MT_Camera *camera, float speed);

#endif
