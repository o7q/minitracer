#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#include "camera.h"
#include "render.h"
#include "world.h"
#include "object.h"
#include "math_utils.h"

int main(void)
{
    random_init();

    Cam camera = camera_create();
    Renderer renderer = renderer_create(120, 100, 1);
    renderer.samples = 20;

    World world = world_create(100);
    MeshObj mesh = mesh_create(1000);

    TriObj floor;
    floor.p1 = (Vec3){25, 1, -50};
    floor.p2 = (Vec3){50, 1, 20};
    floor.p3 = (Vec3){-50, 1, 20};
    floor.mat.color = (Vec3){0.8, 0.4, 0.7};
    floor.mat.emission = (Vec3){0, 0, 0};
    floor.mat.emission_strength = 0.0f;
    floor.mat.roughness = 1.0f;
    tri_init_normals(&floor);

    SphereObj sphere;
    sphere.position = (Vec3){-1, 0, 0};
    sphere.radius = 1;
    sphere.mat.color = (Vec3){1.0f, 0.5f, 1.0f};
    sphere.mat.emission = (Vec3){1, 1, 0.9};
    sphere.mat.emission_strength = 0.0f;
    sphere.mat.roughness = 0.0f;

    SphereObj sphere2;
    sphere2.position = (Vec3){0, 0, -2.5};
    sphere2.radius = 1;
    sphere2.mat.color = (Vec3){0.5f, 1.0f, 0.9f};
    sphere2.mat.emission = (Vec3){1, 1, 0.9};
    sphere2.mat.emission_strength = 0.0f;
    sphere2.mat.roughness = 0.0f;

    TriObj tri = tri_create((Vec3){0, -4, 0}, (Vec3){20, -4, 0}, (Vec3){5, -4, -20});
    tri.mat.color = (Vec3){1, 1, 1};
    tri.mat.emission = (Vec3){1, 1, 1};
    tri.mat.emission_strength = 1.0f;
    tri.mat.roughness = 0.25f;

    mesh_add_tri(&mesh, floor);
    mesh_add_tri(&mesh, tri);

    world_add_object(&world, &mesh, OBJECT_MESH);
    world_add_object(&world, &sphere, OBJECT_SPHERE);
    world_add_object(&world, &sphere2, OBJECT_SPHERE);

    InitWindow(renderer.width * 6, renderer.height * 6, "raytracer");
    SetWindowPosition(2500, 200);

    RenderTexture2D target = LoadRenderTexture(renderer.width, renderer.height);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE))
        {
            camera.position.y -= 0.1f;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            camera.position.y += 0.1f;
        }

        if (IsKeyDown(KEY_W))
        {
            camera.position.z -= 0.1f * cosf(camera.rotation.y);
            camera.position.x -= 0.1f * sinf(camera.rotation.y);
        }
        if (IsKeyDown(KEY_S))
        {
            camera.position.z += 0.1f * cosf(camera.rotation.y);
            camera.position.x += 0.1f * sinf(camera.rotation.y);
        }
        if (IsKeyDown(KEY_A))
        {
            camera.position.x -= 0.1f * cosf(camera.rotation.y);
            camera.position.z += 0.1f * sinf(camera.rotation.y);
        }
        if (IsKeyDown(KEY_D))
        {
            camera.position.x += 0.1f * cosf(camera.rotation.y);
            camera.position.z -= 0.1f * sinf(camera.rotation.y);
        }
        if (IsKeyDown(KEY_Z))
        {
            camera.fov += 0.1f;
        }
        if (IsKeyDown(KEY_X))
        {
            camera.fov -= 0.1f;
        }
        if (IsKeyDown(KEY_UP))
        {
            camera.rotation.x -= 0.1f;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            camera.rotation.x += 0.1f;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            camera.rotation.y += 0.1f;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            camera.rotation.y -= 0.1f;
        }

        render(&renderer, &camera, &world);

        BeginTextureMode(target);
        ClearBackground(BLACK);
        UpdateTexture(target.texture, renderer.pixels);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = {0, 0, (float)renderer.width, (float)renderer.height};
        Rectangle dest = {0, 0, (float)renderer.width * 6, (float)renderer.height * 6};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}