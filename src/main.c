#include <stdio.h>

#include <raylib.h>

#include "camera.h"
#include "render.h"
#include "world.h"
#include "mesh.h"

int main()
{
    int renderWidth = 100;
    int renderHeight = 80;
    int windowWidth = renderWidth * 8;
    int windowHeight = renderHeight * 8;

    Cam3 camera;
    camera.pos = (Vec3){0, 0, 0};
    camera.fov = 1.0f;
    camera.width = renderWidth;
    camera.height = renderHeight;

    World *world = world_create(100);

    Mesh3 *mesh = mesh_create(3);

    Tri3 tri;
    tri.p1 = (Vec3){0, 0, 0};
    tri.p2 = (Vec3){3, 0, 0};
    tri.p3 = (Vec3){1.5, 3, 0};
    tri.offset = (Vec3){0, 0, -2};
    tri.mat = (Mat){(Vec3){1, 0, 0}, 1.0f};

    Tri3 tri2;
    tri2.p1 = (Vec3){0, 0, 0};
    tri2.p2 = (Vec3){5, 0, 0};
    tri2.p3 = (Vec3){4, 6, 0};
    tri2.offset = (Vec3){0, 0, -5};
    tri2.mat = (Mat){(Vec3){0, 0, 1}, 1.0f};

    mesh_add_tri(mesh, tri);
    mesh_add_tri(mesh, tri2);

    world_add_mesh(world, mesh);

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
            camera.pos.z -= 0.1f;
        if (IsKeyDown(KEY_S))
            camera.pos.z += 0.1f;
        if (IsKeyDown(KEY_A))
            camera.pos.x -= 0.1f;
        if (IsKeyDown(KEY_D))
            camera.pos.x += 0.1f;
        if (IsKeyDown(KEY_Z))
            camera.fov += 0.1f;
        if (IsKeyDown(KEY_X))
            camera.fov -= 0.1f;
        if (IsKeyDown(KEY_UP))
            camera.rot.x += 0.1f;
        if (IsKeyDown(KEY_DOWN))
            camera.rot.x -= 0.1f;
        if (IsKeyDown(KEY_LEFT))
            camera.rot.y -= 0.1f;
        if (IsKeyDown(KEY_RIGHT))
            camera.rot.y += 0.1f;

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