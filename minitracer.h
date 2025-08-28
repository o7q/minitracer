// minitracer

////////////////////////////////////////////////////////////////////////////////////
// MIT License                                                                    //
//                                                                                //
// Copyright (c) 2025 James                                                       //
//                                                                                //
// Permission is hereby granted, free of charge, to any person obtaining a copy   //
// of this software and associated documentation files (the "Software"), to deal  //
// in the Software without restriction, including without limitation the rights   //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      //
// copies of the Software, and to permit persons to whom the Software is          //
// furnished to do so, subject to the following conditions:                       //
//                                                                                //
// The above copyright notice and this permission notice shall be included in all //
// copies or substantial portions of the Software.                                //
//                                                                                //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  //
// SOFTWARE.                                                                      //
////////////////////////////////////////////////////////////////////////////////////

#ifndef MINITRACER_H
#define MINITRACER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <pthread.h>

///////////////////////////////
// ========== VEC ========== //
///////////////////////////////
typedef struct MT_Vec3
{
    float x, y, z;
} MT_Vec3;

MT_Vec3 vec3_add(MT_Vec3 a, MT_Vec3 b);
MT_Vec3 vec3_sub(MT_Vec3 a, MT_Vec3 b);
MT_Vec3 vec3_mult(MT_Vec3 a, MT_Vec3 b);
MT_Vec3 vec3_div(MT_Vec3 a, MT_Vec3 b);

MT_Vec3 vec3_add_v(MT_Vec3 a, float v);
MT_Vec3 vec3_sub_v(MT_Vec3 a, float v);
MT_Vec3 vec3_mult_v(MT_Vec3 a, float v);
MT_Vec3 vec3_div_v(MT_Vec3 a, float v);

float vec3_dot(MT_Vec3 a, MT_Vec3 b);
MT_Vec3 vec3_cross(MT_Vec3 a, MT_Vec3 b);

float vec3_length_squared(MT_Vec3 a);
float vec3_length(MT_Vec3 a);
MT_Vec3 vec3_normalize(MT_Vec3 a);
MT_Vec3 vec3_lerp(MT_Vec3 a, MT_Vec3 b, float t);

//////////////////////////////////
// ========== MATRIX ========== //
//////////////////////////////////
typedef struct MT_Mat4x4
{
    float m[4][4];
} MT_Mat4x4;

MT_Mat4x4 mat4x4_mult(MT_Mat4x4 a, MT_Mat4x4 b);
MT_Vec3 mat4x4_mult_vec3(MT_Mat4x4 m, MT_Vec3 v);

MT_Mat4x4 mat4x4_create_translation(MT_Vec3 translation);
MT_Mat4x4 mat4x4_create_rotation(MT_Vec3 rotation);
MT_Mat4x4 mat4x4_create_scale(MT_Vec3 scale);

//////////////////////////////////////
// ========== MATH UTILS ========== //
//////////////////////////////////////
static const double MT_PI = 3.14159265358979323846;
static const double MT_EPSILON = 0.000001;

////////////////////////////////////
// ========== MATERIAL ========== //
////////////////////////////////////
typedef struct MT_Material
{
    MT_Vec3 color;
    MT_Vec3 emission;
    float emission_strength;
    float roughness;
    int is_transparent;
    float ior;
} MT_Material;

MT_Material *material_create();
void material_delete(MT_Material *material);

//////////////////////////////////
// ========== OBJECT ========== //
//////////////////////////////////
typedef enum ObjectType
{
    OBJECT_TRI,
    OBJECT_MESH,
    OBJECT_SPHERE
} ObjectType;

typedef struct MT_Tri
{
    MT_Vec3 p1, p2, p3;
    MT_Vec3 p1_n, p2_n, p3_n;
    MT_Vec3 normal;

    MT_Material *mat;
} MT_Tri;

typedef struct MT_Mesh MT_Mesh;

typedef struct MT_Sphere
{
    MT_Vec3 position;
    float radius;

    MT_Material *mat;
} MT_Sphere;

MT_Tri *tri_create(MT_Vec3 p1, MT_Vec3 p2, MT_Vec3 p3, MT_Material *mat);
void tri_delete(MT_Tri *tri);

MT_Mesh *mesh_create(unsigned int max_tris);
MT_Mesh *mesh_create_plane(MT_Vec3 position, MT_Vec3 rotation, MT_Vec3 scale, MT_Material *material);
MT_Mesh *mesh_create_cube(MT_Vec3 position, MT_Vec3 rotation, MT_Vec3 scale, MT_Material *material);
void mesh_add_tri(MT_Mesh *mesh, MT_Tri *tri);
void mesh_init_normals(MT_Mesh *mesh);
void mesh_move(MT_Mesh *mesh, MT_Vec3 position);
void mesh_rotate(MT_Mesh *mesh, MT_Vec3 rotation);
void mesh_scale(MT_Mesh *mesh, MT_Vec3 scale);
void mesh_transform(MT_Mesh *mesh, MT_Vec3 translation, MT_Vec3 rotation, MT_Vec3 scale);
void mesh_delete(MT_Mesh *mesh);

MT_Sphere *sphere_create(MT_Vec3 position, float radius, MT_Material *mat);
void sphere_delete(MT_Sphere *sphere);

///////////////////////////////
// ========== RAY ========== //
///////////////////////////////
typedef struct MT_Ray MT_Ray;
typedef struct MT_RayHit MT_RayHit;

/////////////////////////////////
// ========== WORLD ========== //
/////////////////////////////////
typedef struct MT_World
{
    void **objects;
    ObjectType *objects_track;

    unsigned int object_index;
    unsigned int max_objects;

} MT_World;

MT_World *world_create(unsigned int max_objects);
void world_add_object(MT_World *world, void *object, ObjectType object_type);
void world_delete(MT_World *world);

//////////////////////////////////
// ========== CAMERA ========== //
//////////////////////////////////
typedef struct MT_Camera
{
    MT_Vec3 position;
    MT_Vec3 rotation;

    float fov;
} MT_Camera;

MT_Camera *camera_create();
void camera_delete(MT_Camera *cam);

//////////////////////////////////
// ========== RENDER ========== //
//////////////////////////////////
typedef struct MT_RenderSettings MT_RenderSettings;
typedef struct MT_RenderThreadStation MT_RenderThreadStation;
typedef struct MT_RenderChunk MT_RenderChunk;
typedef struct MT_Renderer MT_Renderer;

MT_Renderer *renderer_create(unsigned int width, unsigned int height, unsigned int thread_count);
void renderer_set_samples(MT_Renderer *renderer, unsigned int samples);
void renderer_set_bounces(MT_Renderer *renderer, unsigned int bounces);
void renderer_set_world(MT_Renderer *renderer, MT_World *world);
void renderer_set_camera(MT_Renderer *renderer, MT_Camera *camera);
void renderer_delete(MT_Renderer *renderer);

MT_Vec3 *renderer_get_pixels(MT_Renderer *renderer);
MT_Vec3 renderer_get_pixel(MT_Renderer* renderer, int x, int y);
int renderer_get_width(MT_Renderer *renderer);
int renderer_get_height(MT_Renderer *renderer);

void render(MT_Renderer *renderer);

#endif // MINITRACER_H

#ifdef MINITRACER_IMPLEMENTATION

//////////////////////////////////////
// ========== MATH UTILS ========== //
//////////////////////////////////////
static __thread unsigned int seed = 0;

static void mt__random_thread_init(int thread_id)
{
    seed = (unsigned int)time(NULL) + thread_id;
}

static inline float mt__random_float_thread()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return 2.0f * (seed / (float)UINT_MAX) - 1.0f;
}

static inline float mt__lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

static inline int mt__sign(float v)
{
    return v < 0.0f ? -1 : 1;
}

static inline unsigned int mt__index_2d_to_1d(unsigned int x, unsigned int y, unsigned int width)
{
    return y * width + x;
}

static inline MT_Vec3 mt__random_hemi(MT_Vec3 normal)
{
    MT_Vec3 dir = (MT_Vec3){mt__random_float_thread(), mt__random_float_thread(), mt__random_float_thread()};
    return vec3_mult_v(dir, mt__sign(vec3_dot(normal, dir)));
}

static inline MT_Vec3 mt__random_hemi_normal_distribution(MT_Vec3 normal)
{
    float u1 = (mt__random_float_thread() + 1.0f) * 0.5f;
    float u2 = (mt__random_float_thread() + 1.0f) * 0.5f;

    float cos_theta = u1;
    float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    float phi = 2.0f * MT_PI * u2;

    MT_Vec3 local_dir = {
        sin_theta * cosf(phi),
        sin_theta * sinf(phi),
        cos_theta};

    return vec3_mult_v(local_dir, mt__sign(vec3_dot(normal, local_dir)));
}

///////////////////////////////
// ========== VEC ========== //
///////////////////////////////
MT_Vec3 vec3_add(MT_Vec3 a, MT_Vec3 b) { return (MT_Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
MT_Vec3 vec3_sub(MT_Vec3 a, MT_Vec3 b) { return (MT_Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }
MT_Vec3 vec3_mult(MT_Vec3 a, MT_Vec3 b) { return (MT_Vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }
MT_Vec3 vec3_div(MT_Vec3 a, MT_Vec3 b) { return (MT_Vec3){a.x / b.x, a.y / b.y, a.z / b.z}; }

MT_Vec3 vec3_add_v(MT_Vec3 a, float v) { return (MT_Vec3){a.x + v, a.y + v, a.z + v}; }
MT_Vec3 vec3_sub_v(MT_Vec3 a, float v) { return (MT_Vec3){a.x - v, a.y - v, a.z - v}; }
MT_Vec3 vec3_mult_v(MT_Vec3 a, float v) { return (MT_Vec3){a.x * v, a.y * v, a.z * v}; }
MT_Vec3 vec3_div_v(MT_Vec3 a, float v) { return (MT_Vec3){a.x / v, a.y / v, a.z / v}; }

float vec3_dot(MT_Vec3 a, MT_Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

MT_Vec3 vec3_cross(MT_Vec3 a, MT_Vec3 b)
{
    return (MT_Vec3){a.y * b.z - a.z * b.y,
                     a.z * b.x - a.x * b.z,
                     a.x * b.y - a.y * b.x};
}

float vec3_length_squared(MT_Vec3 a)
{
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float vec3_length(MT_Vec3 a)
{
    return sqrtf(vec3_length_squared(a));
}

MT_Vec3 vec3_normalize(MT_Vec3 a)
{
    float m = vec3_length(a);
    return (MT_Vec3){a.x / m, a.y / m, a.z / m};
}

MT_Vec3 vec3_lerp(MT_Vec3 a, MT_Vec3 b, float t)
{
    return (MT_Vec3){mt__lerp(a.x, b.x, t), mt__lerp(a.y, b.y, t), mt__lerp(a.z, b.z, t)};
}

//////////////////////////////////
// ========== MATRIX ========== //
//////////////////////////////////
MT_Mat4x4 mat4x4_mult(MT_Mat4x4 a, MT_Mat4x4 b)
{
    MT_Mat4x4 out;

    out.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
    out.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
    out.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
    out.m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

    out.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
    out.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
    out.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
    out.m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

    out.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
    out.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
    out.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
    out.m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

    out.m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
    out.m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
    out.m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
    out.m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

    return out;
}

MT_Vec3 mat4x4_mult_vec3(MT_Mat4x4 m, MT_Vec3 v)
{
    MT_Vec3 out;

    out.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + 1.0f * m.m[0][3];
    out.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + 1.0f * m.m[1][3];
    out.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + 1.0f * m.m[2][3];

    return out;
}

MT_Mat4x4 mat4x4_create_translation(MT_Vec3 translation)
{
    MT_Mat4x4 out;

    out.m[0][0] = 1.0f;
    out.m[0][1] = 0;
    out.m[0][2] = 0;
    out.m[0][3] = translation.x;

    out.m[1][0] = 0;
    out.m[1][1] = 1.0f;
    out.m[1][2] = 0;
    out.m[1][3] = translation.y;

    out.m[2][0] = 0;
    out.m[2][1] = 0;
    out.m[2][2] = 1.0f;
    out.m[2][3] = translation.z;

    out.m[3][0] = 0;
    out.m[3][1] = 0;
    out.m[3][2] = 0;
    out.m[3][3] = 1.0f;

    return out;
}

MT_Mat4x4 mat4x4_create_rotation(MT_Vec3 rotation)
{
    float x_cos = cosf(rotation.x);
    float x_sin = sinf(rotation.x);
    float y_cos = cosf(rotation.y);
    float y_sin = sinf(rotation.y);
    float z_cos = cosf(rotation.z);
    float z_sin = sinf(rotation.z);

    MT_Mat4x4 rot;

    rot.m[0][0] = z_cos * y_cos;
    rot.m[0][1] = z_cos * y_sin * x_sin - z_sin * x_cos;
    rot.m[0][2] = z_cos * y_sin * x_cos + z_sin * x_sin;
    rot.m[0][3] = 0.0f;

    rot.m[1][0] = z_sin * y_cos;
    rot.m[1][1] = z_sin * y_sin * x_sin + z_cos * x_cos;
    rot.m[1][2] = z_sin * y_sin * x_cos - z_cos * x_sin;
    rot.m[1][3] = 0.0f;

    rot.m[2][0] = -y_sin;
    rot.m[2][1] = y_cos * x_sin;
    rot.m[2][2] = y_cos * x_cos;
    rot.m[2][3] = 0.0f;

    rot.m[3][0] = 0;
    rot.m[3][1] = 0;
    rot.m[3][2] = 0;
    rot.m[3][3] = 1.0f;

    return rot;
}

MT_Mat4x4 mat4x4_create_scale(MT_Vec3 scale)
{
    MT_Mat4x4 out;

    out.m[0][0] = scale.x;
    out.m[0][1] = 0;
    out.m[0][2] = 0;
    out.m[0][3] = 0;

    out.m[1][0] = 0;
    out.m[1][1] = scale.y;
    out.m[1][2] = 0;
    out.m[1][3] = 0;

    out.m[2][0] = 0;
    out.m[2][1] = 0;
    out.m[2][2] = scale.z;
    out.m[2][3] = 0;

    out.m[3][0] = 0;
    out.m[3][1] = 0;
    out.m[3][2] = 0;
    out.m[3][3] = 1.0f;

    return out;
}

////////////////////////////////////
// ========== MATERIAL ========== //
////////////////////////////////////
MT_Material *material_create()
{
    MT_Material *mat = (MT_Material *)malloc(sizeof(MT_Material));
    mat->color = (MT_Vec3){1.0f, 1.0f, 1.0f};
    mat->emission = (MT_Vec3){1.0f, 1.0f, 1.0f};
    mat->emission_strength = 0.0f;
    mat->roughness = 1.0f;
    mat->is_transparent = 0;
    mat->ior = 1.0f;
    return mat;
}

void material_delete(MT_Material *material)
{
    if (material)
    {
        free(material);
    }
}

//////////////////////////////////
// ========== OBJECT ========== //
//////////////////////////////////
typedef struct MT_Mesh
{
    MT_Tri **tris;

    MT_Vec3 origin_offset;

    unsigned int max_tris;
    unsigned int tri_index;
} MT_Mesh;

static void mt__tri_init_normals(MT_Tri *tri)
{
    MT_Vec3 u = vec3_sub(tri->p2, tri->p1);
    MT_Vec3 v = vec3_sub(tri->p3, tri->p1);

    MT_Vec3 normal = vec3_normalize(vec3_cross(u, v));
    tri->normal = normal;
    tri->p1_n = normal;
    tri->p2_n = normal;
    tri->p3_n = normal;
}

MT_Tri *tri_create(MT_Vec3 p1, MT_Vec3 p2, MT_Vec3 p3, MT_Material *mat)
{
    MT_Tri *tri = (MT_Tri *)malloc(sizeof(MT_Tri));
    tri->p1 = p1;
    tri->p2 = p2;
    tri->p3 = p3;
    tri->mat = mat;

    mt__tri_init_normals(tri);

    return tri;
}

void tri_delete(MT_Tri *tri)
{
    if (tri)
    {
        free(tri);
    }
}

MT_Mesh *mesh_create(unsigned int max_tris)
{
    MT_Mesh *mesh = (MT_Mesh *)malloc(sizeof(MT_Mesh));
    mesh->tris = (MT_Tri **)malloc(sizeof(MT_Tri *) * max_tris);
    mesh->tri_index = 0;
    mesh->max_tris = max_tris;
    return mesh;
}

MT_Mesh *mesh_create_plane(MT_Vec3 position, MT_Vec3 rotation, MT_Vec3 scale, MT_Material *material)
{
    MT_Mesh *plane = mesh_create(2);

    MT_Tri *t1 = tri_create((MT_Vec3){-0.5f, 0, 0.5f}, (MT_Vec3){-0.5f, 0, -0.5f}, (MT_Vec3){0.5f, 0, 0.5f}, material);
    MT_Tri *t2 = tri_create((MT_Vec3){0.5f, 0, 0.5f}, (MT_Vec3){-0.5f, 0, -0.5f}, (MT_Vec3){0.5f, 0, -0.5f}, material);
    mesh_add_tri(plane, t1);
    mesh_add_tri(plane, t2);

    mesh_transform(plane, position, rotation, scale);

    return plane;
}

MT_Mesh *mesh_create_cube(MT_Vec3 position, MT_Vec3 rotation, MT_Vec3 scale, MT_Material *material)
{
    MT_Mesh *cube = mesh_create(12);

    MT_Mesh **planes = (MT_Mesh **)malloc(sizeof(MT_Mesh *) * 6);

    for (int i = 0; i < 6; ++i)
    {
        planes[i] = mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){1, 1, 1}, material);
    }

    // top
    mesh_move(planes[0], (MT_Vec3){0, -0.5, 0});

    // bottom
    mesh_move(planes[1], (MT_Vec3){0, 0.5, 0});
    mesh_rotate(planes[1], (MT_Vec3){MT_PI, 0, 0});

    // front
    mesh_move(planes[2], (MT_Vec3){0, 0, -0.5});
    mesh_rotate(planes[2], (MT_Vec3){MT_PI / 2.0f, 0, 0});

    // back
    mesh_move(planes[3], (MT_Vec3){0, 0, 0.5});
    mesh_rotate(planes[3], (MT_Vec3){-MT_PI / 2.0f, 0, 0});

    // left
    mesh_move(planes[4], (MT_Vec3){-0.5, 0, 0});
    mesh_rotate(planes[4], (MT_Vec3){0, 0, -MT_PI / 2.0f});

    // right
    mesh_move(planes[5], (MT_Vec3){0.5, 0, 0});
    mesh_rotate(planes[5], (MT_Vec3){0, 0, MT_PI / 2.0f});

    for (int i = 0; i < 6; ++i)
    {
        mesh_add_tri(cube, planes[i]->tris[0]);
        mesh_add_tri(cube, planes[i]->tris[1]);
    }

    free(planes);

    mesh_transform(cube, position, rotation, scale);

    return cube;
}

// moves a tri into the mesh, deleting the mesh will delete all children tris
void mesh_add_tri(MT_Mesh *mesh, MT_Tri *tri)
{
    if (mesh->tri_index >= mesh->max_tris)
    {
        return;
    }

    mesh->tris[mesh->tri_index] = tri;
    ++mesh->tri_index;
}

void mesh_init_normals(MT_Mesh *mesh)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        mt__tri_init_normals(mesh->tris[i]);
    }
}

void mesh_move(MT_Mesh *mesh, MT_Vec3 position)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        MT_Tri *tri = mesh->tris[i];

        tri->p1 = vec3_add(tri->p1, position);
        tri->p2 = vec3_add(tri->p2, position);
        tri->p3 = vec3_add(tri->p3, position);
    }

    mesh->origin_offset = vec3_add(mesh->origin_offset, position);
}

void mesh_rotate(MT_Mesh *mesh, MT_Vec3 rotation)
{
    MT_Mat4x4 rotation_mat = mat4x4_create_rotation(rotation);

    for (int i = 0; i < mesh->tri_index; ++i)
    {
        MT_Tri *tri = mesh->tris[i];

        tri->p1 = vec3_sub(tri->p1, mesh->origin_offset);
        tri->p2 = vec3_sub(tri->p2, mesh->origin_offset);
        tri->p3 = vec3_sub(tri->p3, mesh->origin_offset);

        tri->p1 = mat4x4_mult_vec3(rotation_mat, tri->p1);
        tri->p2 = mat4x4_mult_vec3(rotation_mat, tri->p2);
        tri->p3 = mat4x4_mult_vec3(rotation_mat, tri->p3);

        tri->p1_n = mat4x4_mult_vec3(rotation_mat, tri->p1_n);
        tri->p2_n = mat4x4_mult_vec3(rotation_mat, tri->p2_n);
        tri->p3_n = mat4x4_mult_vec3(rotation_mat, tri->p3_n);

        tri->p1 = vec3_add(tri->p1, mesh->origin_offset);
        tri->p2 = vec3_add(tri->p2, mesh->origin_offset);
        tri->p3 = vec3_add(tri->p3, mesh->origin_offset);
    }
}

void mesh_scale(MT_Mesh *mesh, MT_Vec3 scale)
{
    MT_Mat4x4 scale_mat = mat4x4_create_scale(scale);

    for (int i = 0; i < mesh->tri_index; ++i)
    {
        MT_Tri *tri = mesh->tris[i];

        tri->p1 = mat4x4_mult_vec3(scale_mat, tri->p1);
        tri->p2 = mat4x4_mult_vec3(scale_mat, tri->p2);
        tri->p3 = mat4x4_mult_vec3(scale_mat, tri->p3);
    }
}

void mesh_transform(MT_Mesh *mesh, MT_Vec3 translation, MT_Vec3 rotation, MT_Vec3 scale)
{
    MT_Mat4x4 translate_mat = mat4x4_create_translation(translation);
    MT_Mat4x4 rotation_mat = mat4x4_create_rotation(rotation);
    MT_Mat4x4 scale_mat = mat4x4_create_scale(scale);

    for (int i = 0; i < mesh->tri_index; ++i)
    {
        MT_Tri *tri = mesh->tris[i];

        tri->p1 = vec3_sub(tri->p1, mesh->origin_offset);
        tri->p2 = vec3_sub(tri->p2, mesh->origin_offset);
        tri->p3 = vec3_sub(tri->p3, mesh->origin_offset);

        tri->p1 = mat4x4_mult_vec3(scale_mat, tri->p1);
        tri->p2 = mat4x4_mult_vec3(scale_mat, tri->p2);
        tri->p3 = mat4x4_mult_vec3(scale_mat, tri->p3);

        tri->p1 = mat4x4_mult_vec3(rotation_mat, tri->p1);
        tri->p2 = mat4x4_mult_vec3(rotation_mat, tri->p2);
        tri->p3 = mat4x4_mult_vec3(rotation_mat, tri->p3);

        // fix normals
        tri->p1_n = mat4x4_mult_vec3(rotation_mat, tri->p1_n);
        tri->p2_n = mat4x4_mult_vec3(rotation_mat, tri->p2_n);
        tri->p3_n = mat4x4_mult_vec3(rotation_mat, tri->p3_n);

        tri->p1 = mat4x4_mult_vec3(translate_mat, tri->p1);
        tri->p2 = mat4x4_mult_vec3(translate_mat, tri->p2);
        tri->p3 = mat4x4_mult_vec3(translate_mat, tri->p3);

        tri->p1 = vec3_add(tri->p1, mesh->origin_offset);
        tri->p2 = vec3_add(tri->p2, mesh->origin_offset);
        tri->p3 = vec3_add(tri->p3, mesh->origin_offset);
    }

    mesh->origin_offset = vec3_add(mesh->origin_offset, translation);
}

void mesh_delete(MT_Mesh *mesh)
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

MT_Sphere *sphere_create(MT_Vec3 position, float radius, MT_Material *mat)
{
    MT_Sphere *sphere = (MT_Sphere *)malloc(sizeof(MT_Sphere));
    sphere->position = position;
    sphere->radius = radius;
    sphere->mat = mat;
    return sphere;
}

void sphere_delete(MT_Sphere *sphere)
{
    if (sphere)
    {
        free(sphere);
    }
}

///////////////////////////////
// ========== RAY ========== //
///////////////////////////////
typedef struct MT_Ray
{
    MT_Vec3 origin;
    MT_Vec3 direction;
    MT_Vec3 radiance;
    MT_Vec3 color;
} MT_Ray;

typedef struct MT_RayHit
{
    MT_Vec3 pos;
    MT_Vec3 normal;
    int hit;
    float t;
    int is_backface;
} MT_RayHit;

static MT_Vec3 mt__ray_at(const MT_Ray *ray, float t)
{
    return (MT_Vec3){
        ray->origin.x + t * ray->direction.x,
        ray->origin.y + t * ray->direction.y,
        ray->origin.z + t * ray->direction.z};
}

// Möller–Trumbore intersection
// thanks to Sebastian Lague for the implementation: https://youtu.be/Qz0KTGYJtUk?t=1418
static MT_RayHit mt__ray_hit_tri(const MT_Ray *ray, const MT_Tri *tri)
{
    MT_RayHit hit;
    hit.hit = 0;

    MT_Vec3 edge1 = vec3_sub(tri->p2, tri->p1);
    MT_Vec3 edge2 = vec3_sub(tri->p3, tri->p1);
    MT_Vec3 tri_normal = vec3_cross(edge1, edge2);

    float det = vec3_dot(ray->direction, tri_normal);
    if (det > -MT_EPSILON)
    {
        return hit;
    }

    float invdet = 1.0f / det;

    MT_Vec3 ray_offset = vec3_sub(ray->origin, tri->p1);

    float dst = -vec3_dot(ray_offset, tri_normal) * invdet;
    if (dst < 0.0f)
    {
        return hit;
    }

    MT_Vec3 ray_offset_perp = vec3_cross(ray_offset, ray->direction);

    float u = -vec3_dot(edge2, ray_offset_perp) * invdet;
    if (u < 0.0f || u > 1.0f)
    {
        return hit;
    }
    float v = vec3_dot(edge1, ray_offset_perp) * invdet;
    if (v < 0.0f || u + v > 1.0f)
    {
        return hit;
    }

    float w = 1.0f - u - v;

    hit.hit = 1;
    hit.pos = mt__ray_at(ray, dst);
    hit.normal = vec3_normalize((MT_Vec3){tri->p1_n.x * w + tri->p2_n.x * u + tri->p3_n.x * v,
                                          tri->p1_n.y * w + tri->p2_n.y * u + tri->p3_n.y * v,
                                          tri->p1_n.z * w + tri->p2_n.z * u + tri->p3_n.z * v});
    hit.t = dst;

    return hit;
}

static MT_RayHit mt__ray_hit_sphere(const MT_Ray *ray, const MT_Sphere *sphere)
{
    MT_RayHit hit;
    hit.hit = 0;

    MT_Vec3 oc = vec3_sub(sphere->position, ray->origin);
    float a = vec3_length_squared(ray->direction);
    float h = vec3_dot(ray->direction, oc);
    float c = vec3_length_squared(oc) - sphere->radius * sphere->radius;
    float discriminant = h * h - a * c;
    if (discriminant >= 0)
    {
        float t = (h - sqrtf(discriminant)) / a;
        if (t >= 0.0f)
        {
            hit.hit = 1;
            hit.t = t;
            hit.pos = mt__ray_at(ray, hit.t);
            hit.normal = vec3_normalize(vec3_sub(hit.pos, sphere->position));
        }
    }

    return hit;
}

static void mt__ray_bounce(MT_Ray *ray, MT_RayHit *hit, MT_Material *mat)
{
    // apply color
    ray->color = vec3_mult(ray->color, mat->color);

    MT_Vec3 emittedLight = vec3_mult_v(mat->emission, mat->emission_strength);
    ray->radiance = vec3_add(ray->radiance, vec3_mult(emittedLight, ray->color));

    if (mat->is_transparent)
    {
        MT_Vec3 i_n = vec3_normalize(ray->direction);
        ray->origin = vec3_add(hit->pos, i_n);
        ray->direction = i_n;
        ray->direction.x *= 2.0f;
    }
    else
    {
        // reflective bounce
        MT_Vec3 i_n = vec3_normalize(ray->direction);
        float d = vec3_dot(i_n, hit->normal);
        ray->origin = hit->pos;
        ray->direction = vec3_sub(i_n, vec3_mult_v(hit->normal, 2.0f * d));

        // scatter from roughness
        ray->direction = vec3_lerp(ray->direction, mt__random_hemi_normal_distribution(hit->normal), mat->roughness);
    }
}

/////////////////////////////////
// ========== WORLD ========== //
/////////////////////////////////
MT_World *world_create(unsigned int max_objects)
{
    MT_World *world = (MT_World *)malloc(sizeof(MT_World));
    world->objects = (void **)malloc(sizeof(void *) * max_objects);
    world->objects_track = (ObjectType *)malloc(sizeof(ObjectType) * max_objects);
    world->object_index = 0;
    world->max_objects = max_objects;
    return world;
}

void world_add_object(MT_World *world, void *object, ObjectType object_type)
{
    if (world->object_index >= world->max_objects)
    {
        return;
    }

    world->objects[world->object_index] = object;
    world->objects_track[world->object_index] = object_type;
    ++world->object_index;
}

void world_delete(MT_World *world)
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

//////////////////////////////////
// ========== CAMERA ========== //
//////////////////////////////////
MT_Camera *camera_create()
{
    MT_Camera *cam = (MT_Camera *)malloc(sizeof(MT_Camera));
    cam->position = (MT_Vec3){0, 0, 0};
    cam->rotation = (MT_Vec3){0, 0, 0};
    cam->fov = 1.0f;
    return cam;
}

void camera_delete(MT_Camera *cam)
{
    if (cam)
    {
        free(cam);
    }
}

//////////////////////////////////
// ========== RENDER ========== //
//////////////////////////////////
typedef struct MT_RenderSettings
{
    MT_World *world;
    MT_Camera *camera;

    int width, height;

    int bounces;
    int samples;

} MT_RenderSettings;

typedef struct MT_RenderThreadStation
{
    unsigned int finished_count;
    unsigned int thread_count;
    pthread_mutex_t finished_mutex;
    pthread_cond_t thread_done_cond;

    MT_Vec3 *pixels;
} MT_RenderThreadStation;

typedef struct MT_RenderChunk
{
    MT_RenderSettings *settings;
    unsigned int px_start;
    unsigned int px_end;
    unsigned int index;
    int ready;
    int terminate;

    MT_RenderThreadStation *thread_station;
    pthread_mutex_t wake_mutex;
    pthread_mutex_t terminate_mutex;
    pthread_cond_t wake_cond;
} MT_RenderChunk;

typedef struct MT_Renderer
{
    MT_RenderSettings settings;

    pthread_t *threads;
    MT_RenderChunk **render_chunks;

    MT_RenderThreadStation thread_station;
} MT_Renderer;

static void mt__render_handle_tri(MT_Ray *ray, MT_Tri *tri, MT_RayHit *hit_info, MT_Material *hit_mat, float *t_lowest)
{
    MT_RayHit ray_hit = mt__ray_hit_tri(ray, tri);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *tri->mat;
    }
}

static void mt__render_handle_mesh(MT_Ray *ray, MT_Mesh *mesh, MT_RayHit *hit_info, MT_Material *hit_mat, float *t_lowest)
{
    for (int i = 0; i < mesh->tri_index; ++i)
    {
        mt__render_handle_tri(ray, mesh->tris[i], hit_info, hit_mat, t_lowest);
    }
}

static void mt__render_handle_sphere(MT_Ray *ray, MT_Sphere *sphere, MT_RayHit *hit_info, MT_Material *hit_mat, float *t_lowest)
{
    MT_RayHit ray_hit = mt__ray_hit_sphere(ray, sphere);
    if (ray_hit.hit && ray_hit.t < *t_lowest)
    {
        hit_info->hit = 1;
        hit_info->pos = ray_hit.pos;
        hit_info->normal = ray_hit.normal;
        *t_lowest = ray_hit.t;

        *hit_mat = *sphere->mat;
    }
}

static void mt__render_chunk(void *data)
{
    MT_RenderChunk *rc = (MT_RenderChunk *)data;

    MT_RenderSettings *rs = rc->settings;

    if (!rs->camera || !rs->world)
    {
        return;
    }

    float viewport_height = 1.0f;
    float viewport_width = viewport_height * ((float)rs->width / rs->height);

    float pixel_delta_u = viewport_width / rs->width;
    float pixel_delta_v = viewport_height / rs->height;

    MT_Vec3 viewport_top_left = vec3_sub(rs->camera->position, (MT_Vec3){viewport_width / 2.0f, viewport_height / 2.0f, rs->camera->fov});
    MT_Vec3 pixel00_pos = vec3_add(viewport_top_left, (MT_Vec3){0.5 * pixel_delta_u, 0.5 * pixel_delta_v, 0});

    for (int i = rc->px_start; i < rc->px_end; ++i)
    {
        int x = i % rs->width;
        int y = i / rs->width;

        MT_Vec3 pixel_center = vec3_add(pixel00_pos, (MT_Vec3){x * pixel_delta_u, y * pixel_delta_v, 0});

        MT_Mat4x4 translate = mat4x4_create_translation(vec3_mult_v(rs->camera->position, -1));
        MT_Mat4x4 rotation = mat4x4_create_rotation(rs->camera->rotation);
        MT_Mat4x4 transform = mat4x4_mult(rotation, translate);

        MT_Vec3 ray_direction = mat4x4_mult_vec3(transform, pixel_center);

        MT_Vec3 render_color = (MT_Vec3){0, 0, 0};

        for (int i = 0; i < rs->samples; ++i)
        {
            MT_Ray ray;
            ray.origin = rs->camera->position;
            ray.direction = ray_direction;
            ray.color = (MT_Vec3){1, 1, 1};
            ray.radiance = (MT_Vec3){0, 0, 0};

            for (int j = 0; j < rs->bounces; ++j)
            {
                float t_lowest = FLT_MAX;

                MT_RayHit hit_info;
                hit_info.hit = 0;
                MT_Material hit_mat;

                for (int k = 0; k < rs->world->object_index; ++k)
                {
                    switch (rs->world->objects_track[k])
                    {
                    case OBJECT_TRI:
                        mt__render_handle_tri(&ray, (MT_Tri *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_MESH:
                        mt__render_handle_mesh(&ray, (MT_Mesh *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    case OBJECT_SPHERE:
                        mt__render_handle_sphere(&ray, (MT_Sphere *)rs->world->objects[k], &hit_info, &hit_mat, &t_lowest);
                        break;
                    }
                }

                if (hit_info.hit)
                {
                    mt__ray_bounce(&ray, &hit_info, &hit_mat);
                }
                else
                {
                    MT_Vec3 unit_direction = vec3_mult_v(vec3_normalize(ray.direction), -1.0f);
                    float a = 0.5f * (unit_direction.y + 1.0f);
                    MT_Vec3 color = vec3_add((MT_Vec3){1.0f - a, 1.0f - a, 1.0f - a}, (MT_Vec3){a * 0.5f, a * 0.7f, a * 1.0f});
                    ray.radiance = vec3_add(ray.radiance, vec3_mult(ray.color, color));
                    break;
                }
            }

            render_color = vec3_add(render_color, ray.radiance);
        }

        render_color = vec3_div_v(render_color, (float)rs->samples);

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

        rc->thread_station->pixels[mt__index_2d_to_1d(x, y, rc->settings->width)] = render_color;

        // display render progress
        // if (i % 100 == 0 && rc->index == 5)
        // {
        //     printf("[%d] %d / %d\n", rc->index, i - rc->px_start, rc->px_end - rc->px_start);
        //     fflush(stdout);
        // }
    }
}

static void *mt__worker_thread(void *data)
{
    MT_RenderChunk *rc = (MT_RenderChunk *)data;

    mt__random_thread_init(rc->index);

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

        mt__render_chunk(rc);

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

MT_Renderer *renderer_create(unsigned int width, unsigned int height, unsigned int thread_count)
{
    MT_Renderer *renderer = malloc(sizeof(MT_Renderer));
    *renderer = (MT_Renderer){(MT_RenderSettings){NULL, NULL, width, height, 5, 20}};

    renderer->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    renderer->render_chunks = (MT_RenderChunk **)malloc(sizeof(MT_RenderChunk *) * thread_count);

    renderer->thread_station.thread_count = thread_count;

    renderer->thread_station.pixels = (MT_Vec3 *)malloc(sizeof(MT_Vec3) * width * height);

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

        MT_RenderChunk *rc = (MT_RenderChunk *)malloc(sizeof(MT_RenderChunk));
        *rc = (MT_RenderChunk){&renderer->settings, px_start, px_end, i, 0, 0};

        rc->thread_station = &renderer->thread_station;

        pthread_mutex_init(&rc->wake_mutex, NULL);
        pthread_mutex_init(&rc->terminate_mutex, NULL);
        pthread_cond_init(&rc->wake_cond, NULL);

        renderer->render_chunks[i] = rc;
        pthread_create(&renderer->threads[i], NULL, mt__worker_thread, rc);
    }

    return renderer;
}

void renderer_set_world(MT_Renderer *renderer, MT_World *world)
{
    renderer->settings.world = world;
}

void renderer_set_camera(MT_Renderer *renderer, MT_Camera *camera)
{
    renderer->settings.camera = camera;
}

void renderer_set_samples(MT_Renderer *renderer, unsigned int samples)
{
    renderer->settings.samples = samples;
}
void renderer_set_bounces(MT_Renderer *renderer, unsigned int bounces)
{
    renderer->settings.bounces = bounces;
}

void renderer_delete(MT_Renderer *renderer)
{
    if (!renderer)
    {
        return;
    }

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        MT_RenderChunk *rc = renderer->render_chunks[i];

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

MT_Vec3 *renderer_get_pixels(MT_Renderer *renderer)
{
    return renderer->thread_station.pixels;
}

MT_Vec3 renderer_get_pixel(MT_Renderer* renderer, int x, int y)
{
    int index = mt__index_2d_to_1d(x, y, renderer->settings.width);
    return renderer->thread_station.pixels[index];
}

int renderer_get_width(MT_Renderer *renderer)
{
    return renderer->settings.width;
}
int renderer_get_height(MT_Renderer *renderer)
{
    return renderer->settings.height;
}

void render(MT_Renderer *renderer)
{
    pthread_mutex_lock(&renderer->thread_station.finished_mutex);
    renderer->thread_station.finished_count = 0;
    pthread_mutex_unlock(&renderer->thread_station.finished_mutex);

    for (int i = 0; i < renderer->thread_station.thread_count; ++i)
    {
        MT_RenderChunk *rc = renderer->render_chunks[i];
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
