#ifndef RENDER_H
#define RENDER_H

#include <pthread.h>

#include "camera.h"
#include "world.h"
#include "object.h"
#include "ray.h"

typedef struct RenderSettings
{
    Cam *camera;
    World *world;

    int width, height;

    int max_bounces;
    int samples;

} RenderSettings;

typedef struct RenderThreadStation
{
    unsigned int finished_count;
    unsigned int thread_count;
    pthread_mutex_t finished_mutex;
    pthread_cond_t thread_done_cond;

    Vec3 *pixels;
} RenderThreadStation;

typedef struct RenderChunk
{
    RenderSettings* settings;
    unsigned int px_start;
    unsigned int px_end;
    unsigned int index;
    int ready;

    RenderThreadStation *thread_station;
    pthread_mutex_t wake_mutex;
    pthread_cond_t wake_cond;
} RenderChunk;

typedef struct Renderer
{
    RenderSettings settings;

    pthread_t *threads;
    RenderChunk **render_chunks;

    RenderThreadStation thread_station;
} Renderer;

Renderer *renderer_create(Cam *camera, World *world, unsigned int width, unsigned int height, unsigned int thread_count);

void render_chunk(void *data);
void *worker_thread(void *data);

void render(Renderer *renderer);
void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);

#endif
