#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "world.h"
#include "object.h"
#include "ray.h"

void render(const Cam *camera, const World *world);
void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);

#endif