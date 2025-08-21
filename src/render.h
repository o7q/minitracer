#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "world.h"
#include "object.h"
#include "ray.h"

void render(const Cam3 *camera, const World *world);
void render_handle_mesh(Ray3 *ray, Mesh3 *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);

#endif