#include "minitracer.h"
#include <raylib.h>

int main(void)
{
    int windowScale = 6;

    MT_World *world = mt_world_create(100);
    MT_Camera *camera = mt_camera_create();
    MT_Renderer *renderer = mt_renderer_create(120, 100, 16);

    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 10);
    mt_renderer_set_bounces(renderer, 1);

    camera->position.x = 0;
    camera->position.y = -1.0f;
    camera->position.z = 1.0f;
    camera->rotation.x = 0;
    camera->rotation.y = 0;
    camera->rotation.z = 0;
    camera->fov = 1.0f;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.25f;
    mat_glossy->color = (MT_Vec3){1.0f, 0.9f, 1.0f};
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 1.0f;
    MT_Material *mat_glass = mt_material_create();
    mat_glass->is_transparent = 1;

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){20, 1, 20}, mat_diffuse);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);

    // MT_Mesh *glass_cube = mt_mesh_create_cube((MT_Vec3){0, -1.01, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){2, 2, 2}, mat_glass);
    // mt_world_add_object(world, glass_cube, MT_OBJECT_MESH);

    // MT_Mesh *m_mesh = mt_mesh_create_from_stl("m.stl", (MT_Vec3){0, -2, -5}, (MT_Vec3){MT_PI / 2, 0, 0}, (MT_Vec3){4, 4, 4}, mat_light);
    // mt_world_add_object(world, m_mesh, MT_OBJECT_MESH);

    // MT_Sphere* sphere = mt_sphere_create((MT_Vec3){0, -1, -3}, 1.0f, mat_glossy);
    // mt_world_add_object(world, sphere, MT_OBJECT_SPHERE);

    Color *color = (Color *)malloc(sizeof(Color) * mt_renderer_get_width(renderer) * mt_renderer_get_height(renderer));
    InitWindow(mt_renderer_get_width(renderer) * windowScale, mt_renderer_get_height(renderer) * windowScale, "minitracer");
    SetWindowPosition(2500, 200);
    RenderTexture2D target = LoadRenderTexture(mt_renderer_get_width(renderer), mt_renderer_get_height(renderer));
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        mt_render(renderer);
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
        if (IsKeyDown(KEY_Q))
        {
            camera->rotation.z -= 0.05f;
        }
        if (IsKeyDown(KEY_E))
        {
            camera->rotation.z += 0.05f;
        }
        if (IsKeyDown(KEY_ONE))
        {
            printf("pos: %.3f %.3f %.3f\nrot: %.3f %.3f %.3f\nfov: %.3f\n\n", camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y, camera->rotation.z, camera->fov);
            fflush(stdout);
        }

        BeginTextureMode(target);
        ClearBackground(BLACK);

        for (int y = 0; y < mt_renderer_get_height(renderer); ++y)
        {
            for (int x = 0; x < mt_renderer_get_width(renderer); ++x)
            {
                MT_Vec3 pixel = mt_renderer_get_pixel(renderer, x, y);
                color[y * (int)mt_renderer_get_width(renderer) + x] = (Color){pixel.x * 255.0f,
                                                                              pixel.y * 255.0f,
                                                                              pixel.z * 255.0f,
                                                                              255.0f};
            }
        }
        UpdateTexture(target.texture, color);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle src = {0, 0, (float)mt_renderer_get_width(renderer), (float)mt_renderer_get_height(renderer)};
        Rectangle dest = {0, 0, (float)mt_renderer_get_width(renderer) * windowScale, (float)mt_renderer_get_height(renderer) * windowScale};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();

    mt_renderer_delete(renderer);
    mt_world_delete(world);
    mt_camera_delete(camera);
    free(color);

    return 0;
}
