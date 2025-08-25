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

    World *world = world_create(100);
    Cam *camera = camera_create();
    Renderer *renderer = renderer_create(150, 100, 16);

    renderer_set_world(renderer, world);
    renderer_set_camera(renderer, camera);
    renderer_set_samples(renderer, 50);
    renderer_set_bounces(renderer, 5);

    Mat *matte_mat = material_create();
    Mat *emission_mat = material_create();
    emission_mat->emission = (Vec3){1.0f, 0.9f, 0.9f};
    emission_mat->emission_strength = 5.0f;
    Mat *glossy_mat = material_create();
    glossy_mat->roughness = 0.0f;

    MeshObj *floor = mesh_create_plane(matte_mat);
    mesh_scale(floor, (Vec3){20.0f, 1.0f, 20.0f});
    mesh_init_normals(floor);
    world_add_object(world, floor, OBJECT_MESH);

    MeshObj *light = mesh_create_plane(emission_mat);
    mesh_move(light, (Vec3){0, -4, 0});
    mesh_scale(light, (Vec3){2.0f, 1.0f, 2.0f});
    mesh_rotate(light, (Vec3){3.1415f, 0, 0});
    mesh_init_normals(light);
    world_add_object(world, light, OBJECT_MESH);

    SphereObj *sphere = sphere_create((Vec3){0, -0.5f, 0}, 0.5f, glossy_mat);
    world_add_object(world, sphere, OBJECT_SPHERE);

    Color *color = (Color *)malloc(sizeof(Color) * renderer->settings.width * renderer->settings.height);
    InitWindow(renderer->settings.width * 6, renderer->settings.height * 6, "raytracer");
    SetWindowPosition(2500, 200);
    RenderTexture2D target = LoadRenderTexture(renderer->settings.width, renderer->settings.height);
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        mesh_rotate(light, (Vec3){0.1f, 0, 0});
        if (IsKeyDown(KEY_SPACE))
        {
            camera->position.y -= 0.1f;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            camera->position.y += 0.1f;
        }

        if (IsKeyDown(KEY_W))
        {
            camera->position.z -= 0.1f * cosf(camera->rotation.y);
            camera->position.x -= 0.1f * sinf(camera->rotation.y);
        }
        if (IsKeyDown(KEY_S))
        {
            camera->position.z += 0.1f * cosf(camera->rotation.y);
            camera->position.x += 0.1f * sinf(camera->rotation.y);
        }
        if (IsKeyDown(KEY_A))
        {
            camera->position.x -= 0.1f * cosf(camera->rotation.y);
            camera->position.z += 0.1f * sinf(camera->rotation.y);
        }
        if (IsKeyDown(KEY_D))
        {
            camera->position.x += 0.1f * cosf(camera->rotation.y);
            camera->position.z -= 0.1f * sinf(camera->rotation.y);
        }
        if (IsKeyDown(KEY_Z))
        {
            camera->fov += 0.25f;
        }
        if (IsKeyDown(KEY_X))
        {
            if (camera->fov > 0.25f)
            {
                camera->fov -= 0.25f;
            }
        }
        if (IsKeyDown(KEY_UP))
        {
            camera->rotation.x -= 0.1f / camera->fov;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            camera->rotation.x += 0.1f / camera->fov;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            camera->rotation.y += 0.1f / camera->fov;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            camera->rotation.y -= 0.1f / camera->fov;
        }

        render(renderer);

        BeginTextureMode(target);
        ClearBackground(BLACK);
        for (int y = 0; y < renderer->settings.height; ++y)
        {
            for (int x = 0; x < renderer->settings.width; ++x)
            {
                int index = index_2d_to_1d(x, y, renderer->settings.width);
                color[index] = (Color){renderer->thread_station.pixels[index].x * 255.0f,
                                       renderer->thread_station.pixels[index].y * 255.0f,
                                       renderer->thread_station.pixels[index].z * 255.0f,
                                       255.0f};
            }
        }
        UpdateTexture(target.texture, color);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = {0, 0, (float)renderer->settings.width, (float)renderer->settings.height};
        Rectangle dest = {0, 0, (float)renderer->settings.width * 6, (float)renderer->settings.height * 6};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();

    renderer_delete(renderer);
    world_delete(world);
    camera_delete(camera);
    free(color);

    return 0;
}