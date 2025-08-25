#include "object.h"

#include <stdlib.h>

#include <math.h>

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
