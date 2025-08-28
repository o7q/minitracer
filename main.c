#include "minitracer.h"
#include <raylib.h>

int main(void)
{
    int windowScale = 5;

    World *world = world_create(100);
    Cam *camera = camera_create();
    camera->position.x = 0;
    camera->position.y = -1.3f;
    camera->position.z = 4.0f;
    camera->rotation.x = 0;
    camera->rotation.y = 0;
    camera->rotation.z = 0;
    camera->fov = 1.0f;
    Renderer *renderer = renderer_create(150, 120, 16);

    renderer_set_world(renderer, world);
    renderer_set_camera(renderer, camera);
    renderer_set_samples(renderer, 20);
    renderer_set_bounces(renderer, 6);

    Mat *mat_diffuse = material_create();
    Mat *mat_diffuse_red = material_create();
    mat_diffuse_red->color = (Vec3){1.0f, 0.2f, 0.3f};
    Mat *mat_glass = material_create();
    mat_glass->is_transparent = 1;
    mat_glass->ior = 1.0f;

    MeshObj *floor = mesh_create_plane(mat_diffuse);
    mesh_scale(floor, (Vec3){20, 1, 20});
    world_add_object(world, floor, OBJECT_MESH);

    SphereObj *ball = sphere_create((Vec3){0, -1, -6}, 1.0f, mat_diffuse_red);
    world_add_object(world, ball, OBJECT_SPHERE);

    MeshObj *cube = mesh_create_cube(mat_glass);
    mesh_scale(cube, (Vec3){2, 2, 2});
    mesh_move(cube, (Vec3){0, -1, 0});
    world_add_object(world, cube, OBJECT_MESH);

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
            camera->rotation.x -= 0.1f / camera->fov / 2.0f;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            camera->rotation.x += 0.1f / camera->fov / 2.0f;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            camera->rotation.y += 0.1f / camera->fov / 2.0f;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            camera->rotation.y -= 0.1f / camera->fov / 2.0f;
        }
        if (IsKeyDown(KEY_ONE))
        {
            printf("pos: %.3f %.3f %.3f\nrot: %.3f %.3f %.3f\nfov: %.3f\n\n", camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y, camera->rotation.z, camera->fov);
            fflush(stdout);
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
    }

    CloseWindow();

    renderer_delete(renderer);
    world_delete(world);
    camera_delete(camera);
    free(color);

    return 0;
}