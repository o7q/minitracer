#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

#include "camera.h"
#include "render.h"
#include "world.h"
#include "object.h"

#include <math.h>

#include "random.h"

int main(void)
{
    random_init();

    int renderWidth = 120;
    int renderHeight = 100;
    int windowWidth = renderWidth * 6;
    int windowHeight = renderHeight * 6;

    Cam3 camera;
    camera.pos = (Vec3){0, 0, 0};
    camera.fov = 1.0f;
    camera.width = renderWidth;
    camera.height = renderHeight;

    World *world = world_create(100);

    Mesh3 *mesh = mesh_create(1000);

    // Tri3 tri;
    // tri.p1 = (Vec3){0, 0, -5};
    // tri.p2 = (Vec3){0, 0, -2};
    // tri.p3 = (Vec3){0, -2, -2.5};
    // tri.mat = (Mat){(Vec3){1, 1, 0}, 1.0f};
    // tri_init_normals(&tri);

    // Tri3 tri2;
    // tri2.p1 = (Vec3){0, 0, -9};
    // tri2.p2 = (Vec3){5, 0, 0};
    // tri2.p3 = (Vec3){4, 6, 0};
    // tri2.mat = (Mat){(Vec3){0, 1, 0}, 1.0f};
    // tri_init_normals(&tri2);

    Tri3 tri3;
    tri3.p1 = (Vec3){25, 1, -50};
    tri3.p2 = (Vec3){50, 1, 0};
    tri3.p3 = (Vec3){-50, 1, 0};
    tri3.mat = (Mat){(Vec3){0, 0, 1}, 1.0f};
    tri_init_normals(&tri3);

    // mesh_add_tri(mesh, tri);
    // mesh_add_tri(mesh, tri2);
    mesh_add_tri(mesh, tri3);

    for (int i = 0; i < 10; ++i)
    {
        Tri3 randtri;
        randtri.p1.x = random_float() * 4;
        randtri.p1.y = -random_float() * 4;
        randtri.p1.z = -random_float() * 4;
        randtri.p2.x = random_float() * 4;
        randtri.p2.y = -random_float() * 4;
        randtri.p2.z = -random_float() * 4;
        randtri.p3.x = random_float() * 4;
        randtri.p3.y = -random_float() * 4;
        randtri.p3.z = -random_float() * 4;

        randtri.mat.color = (Vec3){random_float(), random_float(), random_float()};
        tri_init_normals(&randtri);

        mesh_add_tri(mesh, randtri);
    }

    world_add_object(world, mesh, OBJECT_MESH);

    InitWindow(windowWidth, windowHeight, "raytracer");
    SetWindowPosition(2500, 200);
    RenderTexture2D target = LoadRenderTexture(renderWidth, renderHeight);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE))
            camera.pos.y -= 0.1f;
        if (IsKeyDown(KEY_LEFT_SHIFT))
            camera.pos.y += 0.1f;
        if (IsKeyDown(KEY_W))
        {
            camera.pos.z -= 0.1f * cosf(camera.rot.y);
            camera.pos.x -= 0.1f * sinf(camera.rot.y);
        }
        if (IsKeyDown(KEY_S))
        {
            camera.pos.z += 0.1f * cosf(camera.rot.y);
            camera.pos.x += 0.1f * sinf(camera.rot.y);
        }
        if (IsKeyDown(KEY_A))
        {
            camera.pos.x -= 0.1f * cosf(camera.rot.y);
            camera.pos.z += 0.1f * sinf(camera.rot.y);
        }
        if (IsKeyDown(KEY_D))
        {
            camera.pos.x += 0.1f * cosf(camera.rot.y);
            camera.pos.z -= 0.1f * sinf(camera.rot.y);
        }
        // camera.pos.x += 0.1f;
        if (IsKeyDown(KEY_Z))
            camera.fov += 0.1f;
        if (IsKeyDown(KEY_X))
            camera.fov -= 0.1f;
        if (IsKeyDown(KEY_UP))
            camera.rot.x -= 0.1f;
        if (IsKeyDown(KEY_DOWN))
            camera.rot.x += 0.1f;
        if (IsKeyDown(KEY_LEFT))
            camera.rot.y += 0.1f;
        if (IsKeyDown(KEY_RIGHT))
            camera.rot.y -= 0.1f;

        BeginTextureMode(target);

        ClearBackground(BLACK);
        render(&camera, world);

        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);
        Rectangle src = {0, 0, (float)renderWidth, -(float)renderHeight};
        Rectangle dest = {0, 0, (float)windowWidth, (float)windowHeight};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}