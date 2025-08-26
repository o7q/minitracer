#ifndef MINITRACER_H
#define MINITRACER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <pthread.h>
#include <float.h>

///////////////////////////////
// ========== VEC ========== //
///////////////////////////////
typedef struct Vec3
{
    float x, y, z;
} Vec3;

Vec3 vec_add(Vec3 a, Vec3 b);
Vec3 vec_sub(Vec3 a, Vec3 b);
Vec3 vec_mult(Vec3 a, Vec3 b);
Vec3 vec_div(Vec3 a, Vec3 b);

Vec3 vec_add_v(Vec3 a, float v);
Vec3 vec_sub_v(Vec3 a, float v);
Vec3 vec_mult_v(Vec3 a, float v);
Vec3 vec_div_v(Vec3 a, float v);

float vec_dot(Vec3 a, Vec3 b);
Vec3 vec_cross(Vec3 a, Vec3 b);

float vec_length_squared(Vec3 a);
float vec_length(Vec3 a);
Vec3 vec_normalize(Vec3 a);
Vec3 vec_lerp(Vec3 a, Vec3 b, float t);

void vec_rotate(Vec3 *point, Vec3 rotation);

//////////////////////////////////////
// ========== MATH_UTILS ========== //
//////////////////////////////////////
static const double PI = 3.14159265358979323846;

void random_thread_init(int thread_id);
float random_float_thread();

Vec3 random_hemi_normal_distribution(Vec3 normal);
Vec3 random_hemi(Vec3 normal);

float lerp(float a, float b, float t);
unsigned int index_2d_to_1d(unsigned int x, unsigned int y, unsigned int width);

///////////////////////////////
// ========== MAT ========== //
///////////////////////////////
typedef struct Mat
{
    Vec3 color;
    Vec3 emission;
    float emission_strength;
    float roughness;
} Mat;

Mat *material_create();
void material_delete(Mat *material);

//////////////////////////////////
// ========== OBJECT ========== //
//////////////////////////////////
typedef enum ObjectType
{
    OBJECT_TRI,
    OBJECT_MESH,
    OBJECT_SPHERE
} ObjectType;

typedef struct TriObj
{
    Vec3 p1, p2, p3;
    Vec3 p1_n, p2_n, p3_n;
    Vec3 normal;

    Mat *mat;
} TriObj;

typedef struct MeshObj
{
    TriObj **tris;

    Vec3 offset_history;

    unsigned int max_tris;
    unsigned int tri_index;
} MeshObj;

typedef struct SphereObj
{
    Vec3 position;
    float radius;

    Mat *mat;
} SphereObj;

void tri_init_normals(TriObj *tri);
TriObj *tri_create(Vec3 p1, Vec3 p2, Vec3 p3, Mat *mat);
void tri_delete(TriObj *tri);

MeshObj *mesh_create(unsigned int max_tris);
MeshObj *mesh_create_plane(Mat *material);
MeshObj *mesh_create_cube(Mat *material);
void mesh_add_tri(MeshObj *mesh, TriObj *tri);
void mesh_init_normals(MeshObj *mesh);
void mesh_move(MeshObj *mesh, Vec3 position);
void mesh_rotate(MeshObj *mesh, Vec3 rotation);
void mesh_scale(MeshObj *mesh, Vec3 scale);
void mesh_delete(MeshObj *mesh);

SphereObj *sphere_create(Vec3 position, float radius, Mat *mat);
void sphere_delete(SphereObj *sphere);

///////////////////////////////
// ========== RAY ========== //
///////////////////////////////
typedef struct Ray3
{
    Vec3 origin;
    Vec3 direction;
    Vec3 radiance;
    Vec3 color;
} Ray3;

typedef struct Ray3Hit
{
    Vec3 pos;
    Vec3 normal;
    int hit;
    float t;
} Ray3Hit;

Vec3 ray_at(const Ray3 *ray, float t);
Ray3Hit ray_hit_tri(const Ray3 *ray, const TriObj *tri);
Ray3Hit ray_hit_sphere(const Ray3 *ray, const SphereObj *sphere);

void ray_bounce(Ray3 *ray, Ray3Hit *hit, Mat *mat);

/////////////////////////////////
// ========== WORLD ========== //
/////////////////////////////////
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

///////////////////////////////
// ========== CAM ========== //
///////////////////////////////
typedef struct Cam
{
    Vec3 position;
    Vec3 rotation;

    float fov;
} Cam;

Cam *camera_create();
void camera_delete(Cam *cam);

//////////////////////////////////
// ========== RENDER ========== //
//////////////////////////////////
typedef struct RenderSettings
{
    World *world;
    Cam *camera;

    int width, height;

    int bounces;
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
    RenderSettings *settings;
    unsigned int px_start;
    unsigned int px_end;
    unsigned int index;
    int ready;
    int terminate;

    RenderThreadStation *thread_station;
    pthread_mutex_t wake_mutex;
    pthread_mutex_t terminate_mutex;
    pthread_cond_t wake_cond;
} RenderChunk;

typedef struct Renderer
{
    RenderSettings settings;

    pthread_t *threads;
    RenderChunk **render_chunks;

    RenderThreadStation thread_station;
} Renderer;

Renderer *renderer_create(unsigned int width, unsigned int height, unsigned int thread_count);
void renderer_set_samples(Renderer *renderer, unsigned int samples);
void renderer_set_bounces(Renderer *renderer, unsigned int bounces);
void renderer_set_world(Renderer *renderer, World *world);
void renderer_set_camera(Renderer *renderer, Cam *camera);
void renderer_delete(Renderer *renderer);

void *worker_thread(void *data);
void render_chunk(void *data);

void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest);
void render(Renderer *renderer);

#endif // MINITRACER_H

#ifdef MINITRACER_IMPLEMENTATION

///////////////////////////////
// ========== VEC ========== //
///////////////////////////////
Vec3 vec_add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 vec_sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 vec_mult(Vec3 a, Vec3 b) { return (Vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }
Vec3 vec_div(Vec3 a, Vec3 b) { return (Vec3){a.x / b.x, a.y / b.y, a.z / b.z}; }

Vec3 vec_add_v(Vec3 a, float v) { return (Vec3){a.x + v, a.y + v, a.z + v}; }
Vec3 vec_sub_v(Vec3 a, float v) { return (Vec3){a.x - v, a.y - v, a.z - v}; }
Vec3 vec_mult_v(Vec3 a, float v) { return (Vec3){a.x * v, a.y * v, a.z * v}; }
Vec3 vec_div_v(Vec3 a, float v) { return (Vec3){a.x / v, a.y / v, a.z / v}; }

float vec_dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec_cross(Vec3 a, Vec3 b)
{
    return (Vec3){a.y * b.z - a.z * b.y,
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x};
}

float vec_length_squared(Vec3 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float vec_length(Vec3 a)
{
    return sqrtf(vec_length_squared(a));
}

Vec3 vec_normalize(Vec3 a)
{
    float m = vec_length(a);
    return (Vec3){a.x / m, a.y / m, a.z / m};
}

Vec3 vec_lerp(Vec3 a, Vec3 b, float t)
{
    return (Vec3){lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)};
}

void vec_rotate(Vec3 *point, Vec3 rotation)
{
    // x
    float p1_rotated_y = point->y * cosf(rotation.x) - point->z * sinf(rotation.x);
    float p1_rotated_z = point->y * sinf(rotation.x) + point->z * cosf(rotation.x);

    float p1_rotated_x = point->x * cosf(rotation.y) + p1_rotated_z * sinf(rotation.y);
    // y
    float p1_rotated_z_2 = p1_rotated_z * cosf(rotation.y) - point->x * sinf(rotation.y);

    float p1_rotated_x_2 = p1_rotated_x * cosf(rotation.z) - p1_rotated_y * sinf(rotation.z);
    float p1_rotated_y_2 = p1_rotated_x * sinf(rotation.z) + p1_rotated_y * cosf(rotation.z);
    // z

    point->x = p1_rotated_x_2;
    point->y = p1_rotated_y_2;
    point->z = p1_rotated_z_2;
}

//////////////////////////////////////
// ========== MATH_UTILS ========== //
//////////////////////////////////////
static __thread unsigned int seed = 0;

void random_thread_init(int thread_id)
{
    seed = (unsigned int)time(NULL) + thread_id;
}

float random_float_thread()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return 2.0f * (seed / (float)UINT_MAX) - 1.0f;
}

Vec3 random_hemi_normal_distribution(Vec3 normal)
{
    float u1 = (random_float_thread() + 1.0f) * 0.5f;
    float u2 = (random_float_thread() + 1.0f) * 0.5f;

    float cos_theta = u1;
    float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    float phi = 2.0f * PI * u2;

    Vec3 local_dir = {
        sin_theta * cosf(phi),
        sin_theta * sinf(phi),
        cos_theta};

    return vec_mult_v(local_dir, (vec_dot(normal, local_dir) < 0 ? -1 : 1));
}

Vec3 random_hemi(Vec3 normal)
{
    Vec3 dir = (Vec3){random_float_thread(), random_float_thread(), random_float_thread()};
    return vec_mult_v(dir, (vec_dot(normal, dir) < 0 ? -1 : 1));
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

unsigned int index_2d_to_1d(unsigned int x, unsigned int y, unsigned int width)
{
    return y * width + x;
}

///////////////////////////////
// ========== MAT ========== //
///////////////////////////////
Mat *material_create()
{
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    mat->color = (Vec3){1.0f, 1.0f, 1.0f};
    mat->emission = (Vec3){1.0f, 1.0f, 1.0f};
    mat->emission_strength = 0.0f;
    mat->roughness = 1.0f;
    return mat;
}

void material_delete(Mat *material)
{
    if (material)
    {
        free(material);
    }
}

//////////////////////////////////
// ========== OBJECT ========== //
//////////////////////////////////
void tri_init_normals(TriObj *tri)
{
    Vec3 u = vec_sub(tri->p2, tri->p1);
    Vec3 v = vec_sub(tri->p3, tri->p1);

    Vec3 normal = vec_normalize(vec_cross(u, v));
    tri->normal = normal;
    tri->p1_n = normal;
    tri->p2_n = normal;
    tri->p3_n = normal;
}

TriObj *tri_create(Vec3 p1, Vec3 p2, Vec3 p3, Mat *mat)
{
    TriObj *tri = (TriObj *)malloc(sizeof(TriObj));
    tri->p1 = p1;
    tri->p2 = p2;
    tri->p3 = p3;
    tri->mat = mat;

    tri_init_normals(tri);

    return tri;
}

void tri_delete(TriObj *tri)
{
    if (tri)
    {
        free(tri);
    }
}

MeshObj *mesh_create(unsigned int max_tris)
{
    MeshObj *mesh = (MeshObj *)malloc(sizeof(MeshObj));
    mesh->tris = (TriObj **)malloc(sizeof(TriObj *) * max_tris);
    mesh->tri_index = 0;
    mesh->max_tris = max_tris;
    return mesh;
}

MeshObj *mesh_create_plane(Mat *material)
{
    MeshObj *plane = mesh_create(2);

    TriObj *t1 = tri_create((Vec3){-0.5f, 0, 0.5f}, (Vec3){-0.5f, 0, -0.5f}, (Vec3){0.5f, 0, 0.5f}, material);
    TriObj *t2 = tri_create((Vec3){0.5f, 0, 0.5f}, (Vec3){-0.5f, 0, -0.5f}, (Vec3){0.5f, 0, -0.5f}, material);
    mesh_add_tri(plane, t1);
    mesh_add_tri(plane, t2);

    return plane;
}

MeshObj *mesh_create_cube(Mat *material)
{
    MeshObj *cube = mesh_create(12);

    MeshObj **planes = (MeshObj **)malloc(sizeof(MeshObj *) * 6);

    for (int i = 0; i < 6; ++i)
    {
        planes[i] = mesh_create_plane(material);
    }

    // top
    mesh_move(planes[0], (Vec3){0, -0.5, 0});

    // bottom
    mesh_move(planes[1], (Vec3){0, 0.5, 0});
    mesh_rotate(planes[1], (Vec3){PI, 0, 0});

    // front
    mesh_move(planes[2], (Vec3){0, 0, -0.5});
    mesh_rotate(planes[2], (Vec3){PI / 2.0f, 0, 0});

    // back
    mesh_move(planes[3], (Vec3){0, 0, 0.5});
    mesh_rotate(planes[3], (Vec3){-PI / 2.0f, 0, 0});

    // left
    mesh_move(planes[4], (Vec3){-0.5, 0, 0});
    mesh_rotate(planes[4], (Vec3){0, 0, -PI / 2.0f});

    // right
    mesh_move(planes[5], (Vec3){0.5, 0, 0});
    mesh_rotate(planes[5], (Vec3){0, 0, PI / 2.0f});

    for (int i = 0; i < 6; ++i)
    {
        mesh_add_tri(cube, planes[i]->tris[0]);
        mesh_add_tri(cube, planes[i]->tris[1]);
    }

    free(planes);

    return cube;
}

// moves a tri into the mesh, deleting the mesh will delete all children tris
void mesh_add_tri(MeshObj *mesh, TriObj *tri)
{
    if (mesh->tri_index >= mesh->max_tris)
    {
        return;
    }

    mesh->tris[mesh->tri_index] = tri;
    ++mesh->tri_index;
}

void mesh_init_normals(MeshObj *mesh)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        tri_init_normals(mesh->tris[i]);
    }
}

void mesh_move(MeshObj *mesh, Vec3 position)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        TriObj *tri = mesh->tris[i];

        tri->p1 = vec_add(tri->p1, position);
        tri->p2 = vec_add(tri->p2, position);
        tri->p3 = vec_add(tri->p3, position);
    }

    mesh->offset_history = vec_add(mesh->offset_history, position);
}

void mesh_rotate(MeshObj *mesh, Vec3 rotation)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        TriObj *tri = mesh->tris[i];
        tri->p1 = vec_sub(tri->p1, mesh->offset_history);
        tri->p2 = vec_sub(tri->p2, mesh->offset_history);
        tri->p3 = vec_sub(tri->p3, mesh->offset_history);

        vec_rotate(&tri->p1, rotation);
        vec_rotate(&tri->p2, rotation);
        vec_rotate(&tri->p3, rotation);
        vec_rotate(&tri->p1_n, rotation);
        vec_rotate(&tri->p2_n, rotation);
        vec_rotate(&tri->p3_n, rotation);

        tri->p1 = vec_add(tri->p1, mesh->offset_history);
        tri->p2 = vec_add(tri->p2, mesh->offset_history);
        tri->p3 = vec_add(tri->p3, mesh->offset_history);
    }
}

void mesh_scale(MeshObj *mesh, Vec3 scale)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        TriObj *tri = mesh->tris[i];

        tri->p1 = vec_mult(tri->p1, scale);
        tri->p2 = vec_mult(tri->p2, scale);
        tri->p3 = vec_mult(tri->p3, scale);
    }
}

void mesh_delete(MeshObj *mesh)
{
    if (!mesh)
    {
        return;
    }

    if (mesh->tris)
    {
        for (int i = 0; i < mesh->tri_index; ++i)
        {
            if (mesh->tris[i])
            {
                free(mesh->tris[i]);
            }
        }
        free(mesh->tris);
    }

    free(mesh);
}

SphereObj *sphere_create(Vec3 position, float radius, Mat *mat)
{
    SphereObj *sphere = (SphereObj *)malloc(sizeof(SphereObj));
    sphere->position = position;
    sphere->radius = radius;
    sphere->mat = mat;
    return sphere;
}

void sphere_delete(SphereObj *sphere)
{
    if (sphere)
    {
        free(sphere);
    }
}

/////////////////////////////////
// ========== WORLD ========== //
/////////////////////////////////
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

///////////////////////////////
// ========== RAY ========== //
///////////////////////////////
Vec3 ray_at(const Ray3 *ray, float t)
{
    return (Vec3){
        ray->origin.x + t * ray->direction.x,
        ray->origin.y + t * ray->direction.y,
        ray->origin.z + t * ray->direction.z};
}

// Möller–Trumbore intersection
// thanks Sebastian Lague for the implementation: https://youtu.be/Qz0KTGYJtUk?t=1418
Ray3Hit ray_hit_tri(const Ray3 *ray, const TriObj *tri)
{
    Ray3Hit hit;
    hit.hit = 0;

    Vec3 edge1 = vec_sub(tri->p2, tri->p1);
    Vec3 edge2 = vec_sub(tri->p3, tri->p1);
    Vec3 normal = vec_cross(edge1, edge2);

    float det = -vec_dot(ray->direction, normal);

    if (det < 1e-6f)
    {
        return hit;
    }

    float invdet = 1.0f / det;

    Vec3 ao = vec_sub(ray->origin, tri->p1);
    Vec3 dao = vec_cross(ao, ray->direction);

    float dst = vec_dot(ao, normal) * invdet;

    if (dst < 0.0f)
    {
        return hit;
    }

    float u = vec_dot(edge2, dao) * invdet;

    if (u < 0.0f || u > 1.0f)
    {
        return hit;
    }

    float v = -vec_dot(edge1, dao) * invdet;

    if (v < 0.0f || u + v > 1.0f)
    {
        return (Ray3Hit){.hit = 0};
    }

    float w = 1.0f - u - v;

    hit.hit = det >= 1e-6 && dst >= 0.0f && u >= 0.0f && v >= 0.0f && w >= 0.0f ? 1 : 0;
    hit.pos = ray_at(ray, dst);
    hit.normal = vec_normalize((Vec3){tri->p1_n.x * w + tri->p2_n.x * u + tri->p3_n.x * v,
                                      tri->p1_n.y * w + tri->p2_n.y * u + tri->p3_n.y * v,
                                      tri->p1_n.z * w + tri->p2_n.z * u + tri->p3_n.z * v});
    hit.t = dst;

    return hit;
}

Ray3Hit ray_hit_sphere(const Ray3 *ray, const SphereObj *sphere)
{
    Ray3Hit hit;
    hit.hit = 0;

    Vec3 oc = vec_sub(sphere->position, ray->origin);
    float a = vec_length_squared(ray->direction);
    float h = vec_dot(ray->direction, oc);
    float c = vec_length_squared(oc) - sphere->radius * sphere->radius;
    float discriminant = h * h - a * c;
    if (discriminant >= 0)
    {
        float t = (h - sqrtf(discriminant)) / a;
        if (t >= 0.0f)
        {
            hit.hit = 1;
            hit.t = t;
            hit.pos = ray_at(ray, hit.t);
            hit.normal = vec_normalize(vec_sub(hit.pos, sphere->position));
        }
    }

    return hit;
}

void ray_bounce(Ray3 *ray, Ray3Hit *hit, Mat *mat)
{
    // apply color
    ray->color = vec_mult(ray->color, mat->color);

    Vec3 emittedLight = vec_mult_v(mat->emission, mat->emission_strength);
    ray->radiance = vec_add(ray->radiance, vec_mult(emittedLight, ray->color));

    // reflective bounce
    Vec3 i_n = vec_normalize(ray->direction);
    float d = vec_dot(i_n, hit->normal);
    ray->origin = hit->pos;
    ray->direction = vec_sub(i_n, vec_mult_v(hit->normal, 2.0f * d));

    // scatter from roughness
    ray->direction = vec_lerp(ray->direction, random_hemi_normal_distribution(hit->normal), mat->roughness);
}

///////////////////////////////
// ========== CAM ========== //
///////////////////////////////
Cam *camera_create()
{
    Cam *cam = (Cam *)malloc(sizeof(Cam));
    cam->position = (Vec3){0, 0, 0};
    cam->rotation = (Vec3){0, 0, 0};
    cam->fov = 1.0f;
    return cam;
}

void camera_delete(Cam *cam)
{
    if (cam)
    {
        free(cam);
    }
}

//////////////////////////////////
// ========== RENDER ========== //
//////////////////////////////////
Renderer *renderer_create(unsigned int width, unsigned int height, unsigned int thread_count)
{
    Renderer *renderer = malloc(sizeof(Renderer));
    *renderer = (Renderer){(RenderSettings){NULL, NULL, width, height, 5, 20}};

    renderer->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    renderer->render_chunks = (RenderChunk **)malloc(sizeof(RenderChunk *) * thread_count);

    renderer->thread_station.thread_count = thread_count;

    renderer->thread_station.pixels = (Vec3 *)malloc(sizeof(Vec3) * width * height);

    pthread_mutex_init(&renderer->thread_station.finished_mutex, NULL);
    pthread_cond_init(&renderer->thread_station.thread_done_cond, NULL);

    int px_inc = (width * height) / thread_count;
    for (int i = 0; i < thread_count; ++i)
    {
        int px_start = i * px_inc;
        int px_end = i * px_inc + px_inc;
        if (px_end > width * height)
        {
            px_end = width * height;
        }

        if (i == thread_count - 1)
        {
            int offset = width * height - px_inc * thread_count;
            px_end += offset;
        }

        RenderChunk *rc = (RenderChunk *)malloc(sizeof(RenderChunk));
        *rc = (RenderChunk){&renderer->settings, px_start, px_end, i, 0, 0};

        rc->thread_station = &renderer->thread_station;

        pthread_mutex_init(&rc->wake_mutex, NULL);
        pthread_mutex_init(&rc->terminate_mutex, NULL);
        pthread_cond_init(&rc->wake_cond, NULL);

        renderer->render_chunks[i] = rc;
        pthread_create(&renderer->threads[i], NULL, worker_thread, rc);
    }

    return renderer;
}

void renderer_set_world(Renderer *renderer, World *world)
{
    renderer->settings.world = world;
}

void renderer_set_camera(Renderer *renderer, Cam *camera)
{
    renderer->settings.camera = camera;
}

void renderer_set_samples(Renderer *renderer, unsigned int samples)
{
    renderer->settings.samples = samples;
}
void renderer_set_bounces(Renderer *renderer, unsigned int bounces)
{
    renderer->settings.bounces = bounces;
}

void renderer_delete(Renderer *renderer)
{
    if (!renderer)
    {
        return;
    }

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        RenderChunk *rc = renderer->render_chunks[i];

        if (!rc)
        {
            continue;
        }

        pthread_mutex_lock(&rc->terminate_mutex);
        rc->terminate = 1;
        pthread_cond_signal(&rc->wake_cond);
        pthread_mutex_unlock(&rc->terminate_mutex);

        printf("Freeing: %d\n", i);
        fflush(stdout);

        pthread_join(renderer->threads[i], NULL);

        pthread_mutex_destroy(&rc->wake_mutex);
        pthread_mutex_destroy(&rc->terminate_mutex);
        pthread_cond_destroy(&rc->wake_cond);

        free(rc);
    }

    if (renderer->render_chunks)
    {
        free(renderer->render_chunks);
    }

    pthread_mutex_destroy(&renderer->thread_station.finished_mutex);
    pthread_cond_destroy(&renderer->thread_station.thread_done_cond);

    if (renderer->threads)
    {
        free(renderer->threads);
    }

    if (renderer->thread_station.pixels)
    {
        free(renderer->thread_station.pixels);
    }

    free(renderer);
}

void *worker_thread(void *data)
{
    RenderChunk *rc = (RenderChunk *)data;

    random_thread_init(rc->index);

    while (1)
    {
        pthread_mutex_lock(&rc->wake_mutex);
        if (!rc->ready)
        {
            pthread_cond_wait(&rc->wake_cond, &rc->wake_mutex);

            if (rc->terminate)
            {
                break;
            }
        }

        rc->ready = 0;

        pthread_mutex_unlock(&rc->wake_mutex);

        render_chunk(rc);

        pthread_mutex_lock(&rc->thread_station->finished_mutex);
        rc->thread_station->finished_count++;
        if (rc->thread_station->finished_count == rc->thread_station->thread_count)
        {
            pthread_cond_signal(&rc->thread_station->thread_done_cond); // wake main
        }
        pthread_mutex_unlock(&rc->thread_station->finished_mutex);
    }

    return NULL;
}

void render_handle_tri(Ray3 *ray, TriObj *tri, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_tri(ray, tri);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *tri->mat;
    }
}

void render_handle_mesh(Ray3 *ray, MeshObj *mesh, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        render_handle_tri(ray, mesh->tris[i], hit_info, hit_mat, t_lowest);
    }
}

void render_handle_sphere(Ray3 *ray, SphereObj *sphere, Ray3Hit *hit_info, Mat *hit_mat, float *t_lowest)
{
    Ray3Hit ray_hit = ray_hit_sphere(ray, sphere);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *sphere->mat;
    }
}

void render_chunk(void *data)
{
    RenderChunk *rc = (RenderChunk *)data;

    RenderSettings *rs = rc->settings;

    if (!rs->camera || !rs->world)
    {
        return;
    }

    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)rs->width / rs->height);

    float pixel_delta_u = viewport_width / rs->width;
    float pixel_delta_v = viewport_height / rs->height;

    Vec3 viewport_top_left = vec_sub(rs->camera->position, (Vec3){viewport_width / 2.0f, viewport_height / 2.0f, rs->camera->fov});
    Vec3 pixel00_pos = vec_add(viewport_top_left, (Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    for (int i = rc->px_start; i < rc->px_end; ++i)
    {
        int x = i % rs->width;
        int y = i / rs->width;

        Vec3 pixel_center = vec_add(pixel00_pos, (Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});
        Vec3 ray_direction = vec_sub(pixel_center, rs->camera->position);
        Vec3 ray_dir_normal = vec_normalize(ray_direction);
        Vec3 ray_rotated_normal = ray_dir_normal;

        float temp_y = ray_dir_normal.y * cosf(rs->camera->rotation.x) - ray_dir_normal.z * sinf(rs->camera->rotation.x);
        float temp_z = ray_dir_normal.y * sinf(rs->camera->rotation.x) + ray_dir_normal.z * cosf(rs->camera->rotation.x);

        ray_rotated_normal.x = ray_dir_normal.x * cosf(rs->camera->rotation.y) + temp_z * sinf(rs->camera->rotation.y);
        ray_rotated_normal.y = temp_y;
        ray_rotated_normal.z = temp_z * cosf(rs->camera->rotation.y) - ray_dir_normal.x * sinf(rs->camera->rotation.y);

        ray_direction = vec_mult_v(ray_rotated_normal, vec_length(ray_direction));

        Vec3 render_color = (Vec3){0, 0, 0};

        for (int i = 0; i < rs->samples; ++i)
        {
            Ray3 ray;
            ray.origin = rs->camera->position;
            ray.direction = ray_direction;
            ray.color = (Vec3){1, 1, 1};
            ray.radiance = (Vec3){0, 0, 0};

            for (int j = 0; j < rs->bounces; ++j)
            {
                float t_lowest = FLT_MAX;

                Ray3Hit hit_info;
                hit_info.hit = 0;
                Mat hit_mat;

                for (int k = 0; k < rs->world->object_index; ++k)
                {
                    switch (rs->world->objects_track[k])
                    {
                    case OBJECT_TRI:
                        render_handle_tri(&ray, (TriObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_MESH:
                        render_handle_mesh(&ray, (MeshObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_SPHERE:
                        render_handle_sphere(&ray, (SphereObj *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    }
                }

                if (hit_info.hit)
                {
                    ray_bounce(&ray, &hit_info, &hit_mat);
                }
                else
                {
                    Vec3 unit_direction = vec_mult_v(vec_normalize(ray.direction), -1.0f);
                    float a = 0.5f * (unit_direction.y + 1.0f);
                    Vec3 color = vec_add((Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (Vec3){a * 0.5f, a * 0.7f, a * 1.0f});
                    // color = vec_div_v(color, 1.5f);
                    ray.radiance = vec_add(ray.radiance, vec_mult(ray.color, color));
                    break;
                }
            }

            render_color = vec_add(render_color, ray.radiance);
        }

        render_color = vec_div_v(render_color, (float)rs->samples);

        float gamma = 0.9f;

        render_color.x = powf(render_color.x, 1.0f / gamma);
        render_color.y = powf(render_color.y, 1.0f / gamma);
        render_color.z = powf(render_color.z, 1.0f / gamma);

        // clamp color
        if (render_color.x > 1.0f)
        {
            render_color.x = 1.0f;
        }
        if (render_color.y > 1.0f)
        {
            render_color.y = 1.0f;
        }
        if (render_color.z > 1.0f)
        {
            render_color.z = 1.0f;
        }

        rc->thread_station->pixels[index_2d_to_1d(x, y, rc->settings->width)] = render_color;

        // display render progress
        if (i % 100 == 0 && rc->index == 3)
        {
            printf("[%d] %d / %d\n", rc->index, i - rc->px_start, rc->px_end - rc->px_start);
            fflush(stdout);
        }
    }
}

void render(Renderer *renderer)
{
    pthread_mutex_lock(&renderer->thread_station.finished_mutex);
    renderer->thread_station.finished_count = 0;
    pthread_mutex_unlock(&renderer->thread_station.finished_mutex);

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        RenderChunk *rc = renderer->render_chunks[i];
        pthread_mutex_lock(&rc->wake_mutex);
        rc->ready = 1;
        pthread_cond_signal(&rc->wake_cond);
        pthread_mutex_unlock(&rc->wake_mutex);
    }

    pthread_mutex_lock(&renderer->thread_station.finished_mutex);
    while (renderer->thread_station.finished_count < renderer->thread_station.thread_count)
    {
        pthread_cond_wait(&renderer->thread_station.thread_done_cond, &renderer->thread_station.finished_mutex);
    }
    pthread_mutex_unlock(&renderer->thread_station.finished_mutex);
}

#endif // MINITRACER_IMPLEMENTATION
