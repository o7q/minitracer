#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "world.h"
#include "object.h"
#include "ray.h"

#include <raylib.h>
#include <pthread.h>

typedef struct Renderer
{
    const unsigned int width, height;

    unsigned int max_bounces;
    unsigned int samples;

    const unsigned int thread_count;
    pthread_t *threads;
    pthread_mutex_t work_mutex;
    pthread_cond_t work_available;
    pthread_cond_t work_complete;

    RenderThreadData** work_queue;
    int work_count;
    int work_index;
    int threads_working;

    Color *pixels;
} Renderer;

typedef struct RenderThreadData
{
    Renderer *renderer;
    Cam *camera;
    World *world;
    unsigned int px_start;
    unsigned int px_end
} RenderThreadData;

Renderer renderer_create(unsigned int width, unsigned int height, unsigned int threads);

void *render_thread(void *data);

void render(Renderer *renderer, const Cam *camera, const World *world);
void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);

#endif
