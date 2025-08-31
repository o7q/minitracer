#include "minitracer.h"
#include <raylib.h>

int main(void)
{
    int windowScale = 2;
    int renderScale = 4;

    MT_World *world = mt_world_create(100);
    MT_Camera *camera = mt_camera_create();
    MT_Renderer *renderer = mt_renderer_create(120 * renderScale, 100 * renderScale, 16);

    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 20);
    mt_renderer_set_bounces(renderer, 16);

    camera->position.x = 0;
    camera->position.y = -2.7;
    camera->position.z = 6.4;
    camera->rotation.x = 0.05;
    camera->rotation.y = 0;
    camera->rotation.z = 0;
    camera->fov = 0.97f;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_diffuse_red = mt_material_create();
    mat_diffuse_red->color = (MT_Vec3){255 / 255.0f, 66 / 255.0f, 33 / 255.0f};
    MT_Material *mat_diffuse_green = mt_material_create();
    mat_diffuse_green->color = (MT_Vec3){77 / 255.0f, 219 / 255.0f, 55 / 255.0f};
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.1f;
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 9.0f;
    mat_light->color = (MT_Vec3){255 / 255.0f, 241 / 255.0f, 201 / 255.0f};
    MT_Material *mat_glass = mt_material_create();
    mat_glass->is_refractive = 1;
    mat_glass->ior = 1.5f;

    // room
    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){20, 1, 20}, mat_diffuse);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);
    MT_Mesh *red_wall = mt_mesh_create_plane((MT_Vec3){-2.49, -2.49, 0}, (MT_Vec3){0, 0, -MT_PI / 2.0f}, (MT_Vec3){5, 1, 5}, mat_diffuse_red);
    mt_world_add_object(world, red_wall, MT_OBJECT_MESH);
    MT_Mesh *green_wall = mt_mesh_create_plane((MT_Vec3){2.49, -2.49, 0}, (MT_Vec3){0, 0, MT_PI / 2.0f}, (MT_Vec3){5, 1, 5}, mat_diffuse_green);
    mt_world_add_object(world, green_wall, MT_OBJECT_MESH);
    MT_Mesh *back_wall = mt_mesh_create_plane((MT_Vec3){0, -2.49, -2.49}, (MT_Vec3){MT_PI / 2.0f, 0, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse);
    mt_world_add_object(world, back_wall, MT_OBJECT_MESH);
    MT_Mesh *ceil = mt_mesh_create_plane((MT_Vec3){0, -4.99, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse);
    mt_world_add_object(world, ceil, MT_OBJECT_MESH);
    MT_Mesh *light = mt_mesh_create_plane((MT_Vec3){0, -4.9, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){1.5, 1, 1.5}, mat_light);
    mt_world_add_object(world, light, MT_OBJECT_MESH);
    
    MT_Sphere *sphere = mt_sphere_create((MT_Vec3){-1, -1, -1}, 1.0f, mat_glass);
    mt_world_add_object(world, sphere, MT_OBJECT_SPHERE);
    MT_Mesh *cube = mt_mesh_create_cube((MT_Vec3){1, -0.55, 1}, (MT_Vec3){0, 1, 0}, (MT_Vec3){1, 1, 1}, mat_glossy);
    mt_world_add_object(world, cube, MT_OBJECT_MESH);

    Color *color = (Color *)malloc(sizeof(Color) * mt_renderer_get_width(renderer) * mt_renderer_get_height(renderer));
    InitWindow(mt_renderer_get_width(renderer) * windowScale, mt_renderer_get_height(renderer) * windowScale, "minitracer");
    SetWindowPosition(2500, 200);
    RenderTexture2D target = LoadRenderTexture(mt_renderer_get_width(renderer), mt_renderer_get_height(renderer));
    SetTargetFPS(60);

    mt_render(renderer);
    while (!WindowShouldClose())
    {
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
