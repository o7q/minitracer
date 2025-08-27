#include "minitracer.h"
#include <raylib.h>

int main(void)
{
    int windowScale = 6;

    World *world = world_create(100);
    Cam *camera = camera_create();
    camera->position.x = 0;
    camera->position.y = -1.3f;
    camera->position.z = 4.0f;
    Renderer *renderer = renderer_create(100, 100, 16);

    renderer_set_world(renderer, world);
    renderer_set_camera(renderer, camera);
    renderer_set_samples(renderer, 20);
    renderer_set_bounces(renderer, 6);

    Mat *matte_mat = material_create();
    Mat *matte_mat_red = material_create();
    matte_mat_red->color = (Vec3){1, 0, 0};
    Mat *matte_mat_green = material_create();
    matte_mat_green->color = (Vec3){0, 1, 0};
    Mat *emission_mat = material_create();
    emission_mat->emission = (Vec3){255 / 255.0f, 247 / 255.0f, 153 / 255.0f};
    emission_mat->emission_strength = 4.0f;
    Mat *glossy_mat = material_create();
    glossy_mat->roughness = 0.1f;

    MeshObj *floor = mesh_create_plane(matte_mat);
    mesh_scale(floor, (Vec3){20.0f, 1.0f, 20.0f});
    mesh_init_normals(floor);
    world_add_object(world, floor, OBJECT_MESH);

    MeshObj *wall = mesh_create_plane(matte_mat_red);
    mesh_scale(wall, (Vec3){3.0f, 1.0f, 3.0f});
    mesh_move(wall, (Vec3){-1.5f, -1.5f, -0.1f});
    mesh_rotate(wall, (Vec3){0, 0, 3.14159 / 2.0f});
    world_add_object(world, wall, OBJECT_MESH);

    MeshObj *wall2 = mesh_create_plane(matte_mat_green);
    mesh_scale(wall2, (Vec3){3.0f, 1.0f, 3.0f});
    mesh_move(wall2, (Vec3){1.5f, -1.5f, -0.1f});
    mesh_rotate(wall2, (Vec3){0, 0, -3.14159 / 2.0f});
    world_add_object(world, wall2, OBJECT_MESH);

    MeshObj *wall3 = mesh_create_plane(matte_mat);
    mesh_scale(wall3, (Vec3){3.0f, 1.0f, 3.0f});
    mesh_move(wall3, (Vec3){0.0f, -1.5f, -1.5f});
    mesh_rotate(wall3, (Vec3){-3.14159 / 2.0f, 0, 0});
    world_add_object(world, wall3, OBJECT_MESH);

    MeshObj *ceil = mesh_create_plane(matte_mat);
    mesh_scale(ceil, (Vec3){3.0f, 1.0f, 3.0f});
    mesh_move(ceil, (Vec3){0.0f, -2.9f, -0.1f});
    mesh_rotate(ceil, (Vec3){-3.14159, 0, 0});
    world_add_object(world, ceil, OBJECT_MESH);

    MeshObj *light = mesh_create_plane(emission_mat);
    mesh_scale(light, (Vec3){1.0f, 1.0f, 1.0f});
    mesh_move(light, (Vec3){0.0f, -2.8f, 0.0f});
    mesh_rotate(light, (Vec3){-3.14159, 0, 0});
    world_add_object(world, light, OBJECT_MESH);

    MeshObj *cube = mesh_create_cube(matte_mat);
    mesh_move(cube, (Vec3){0.6f, -0.5f, 0});
    mesh_rotate(cube, (Vec3){0, -0.5f, 0});
    world_add_object(world, cube, OBJECT_MESH);

    MeshObj *cube2 = mesh_create_cube(matte_mat);
    mesh_move(cube2, (Vec3){-0.6f, -0.5f, -0.6f});
    mesh_scale(cube2, (Vec3){1, 2, 1});
    mesh_rotate(cube2, (Vec3){0, 0.5f, 0});
    world_add_object(world, cube2, OBJECT_MESH);

    SphereObj *sphere = sphere_create((Vec3){-0.6f, -0.5f, 0.62f}, 0.5f, glossy_mat);
    world_add_object(world, sphere, OBJECT_SPHERE);

    Color *color = (Color *)malloc(sizeof(Color) * renderer->settings.width * renderer->settings.height);
    InitWindow(renderer->settings.width * windowScale, renderer->settings.height * windowScale, "minitracer");
    SetWindowPosition(2500, 200);
    RenderTexture2D target = LoadRenderTexture(renderer->settings.width, renderer->settings.height);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        render(renderer);
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
        Rectangle dest = {0, 0, (float)renderer->settings.width * windowScale, (float)renderer->settings.height * windowScale};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();

        // printf("pos: %.3f %.3f %.3f\nrot: %.3f %.3f %.3f\n\n", camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y, camera->rotation.z);
        // fflush(stdout);
    }

    CloseWindow();

    renderer_delete(renderer);
    world_delete(world);
    camera_delete(camera);
    free(color);

    return 0;
}