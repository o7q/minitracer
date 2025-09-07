#include "../minitracer.h"

#include <raylib.h>
#include "raylib_utils/common.h"

int main(void)
{
    int width = 180;
    int height = 180;
    int render_scale = 1; // scales render width and height
    int render_width = width * render_scale;
    int render_height = height * render_scale;
    float camera_speed = 0.025f * render_scale; // scale speed with render scale to account for lag
    int b_log_samples = 1;
    int b_enable_controls = 1;

    MT_World *world = mt_world_create(1000);
    MT_Camera *camera = mt_camera_create();
    MT_Environment *environment = mt_environment_create();
    environment->brightness = 1.0f;
    mt_world_set_environment(world, environment);

    MT_Renderer *renderer = mt_renderer_create(width * render_scale, height * render_scale, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 5000);
    mt_renderer_set_bounces(renderer, 64);
    mt_renderer_enable_progressive(renderer, 1);
    mt_renderer_enable_antialiasing(renderer, 1);

    camera->position.x = 0;
    camera->position.y = -2.475;
    camera->position.z = 4.875;
    camera->rotation.x = 0;
    camera->rotation.y = 0;
    camera->rotation.z = 0;
    camera->fov = 1.0f;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_diffuse_red = mt_material_create();
    mat_diffuse_red->color = (MT_Vec3){1.0f, 0.5f, 0.5f};
    MT_Material *mat_diffuse_green = mt_material_create();
    mat_diffuse_green->color = (MT_Vec3){0.5f, 1.0f, 0.5f};
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.1f;
    MT_Material *mat_light = mt_material_create();
    mat_light->color = (MT_Vec3){255 / 255.0f, 241 / 255.0f, 201 / 255.0f};
    mat_light->emission_strength = 2.0f;
    MT_Material *mat_glass = mt_material_create();
    mat_glass->b_is_refractive = 1;
    mat_glass->ior = 1.5f;

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){50, 1, 50}, mat_diffuse);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);
    MT_Mesh *light = mt_mesh_create_plane((MT_Vec3){0, -4.9, -2.5}, (MT_Vec3){0, 0, 0}, (MT_Vec3){1, 1, 1}, mat_light);
    mt_world_add_object(world, light, MT_OBJECT_MESH);

    MT_Mesh *ceiling = mt_mesh_create_plane((MT_Vec3){0, -5, -2.5}, (MT_Vec3){0, 0, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse);
    mt_world_add_object(world, ceiling, MT_OBJECT_MESH);
    MT_Mesh *back_wall = mt_mesh_create_plane((MT_Vec3){0, -2.5, -5}, (MT_Vec3){MT_PI / 2.0f, 0, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse);
    mt_world_add_object(world, back_wall, MT_OBJECT_MESH);
    MT_Mesh *left_wall = mt_mesh_create_plane((MT_Vec3){-2.5, -2.5, -2.5}, (MT_Vec3){MT_PI / 2.0f, MT_PI / 2.0f, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse_red);
    mt_world_add_object(world, left_wall, MT_OBJECT_MESH);
    MT_Mesh *right_wall = mt_mesh_create_plane((MT_Vec3){2.5, -2.5, -2.5}, (MT_Vec3){-MT_PI / 2.0f, MT_PI / 2.0f, 0}, (MT_Vec3){5, 1, 5}, mat_diffuse_green);
    mt_world_add_object(world, right_wall, MT_OBJECT_MESH);

    MT_Mesh *cube = mt_mesh_create_cube((MT_Vec3){1, -1.5 / 2.0f - 0.05, -1.5}, (MT_Vec3){0, 0.35, 0}, (MT_Vec3){1.5, 1.5, 1.5}, mat_glass);
    mt_world_add_object(world, cube, MT_OBJECT_MESH);
    MT_Mesh *cube2 = mt_mesh_create_cube((MT_Vec3){-1, -3 / 2.0f - 0.05, -3}, (MT_Vec3){0, -0.35, 0}, (MT_Vec3){1.5, 3, 1.5}, mat_glass);
    mt_world_add_object(world, cube2, MT_OBJECT_MESH);
    MT_Sphere *sphere = mt_sphere_create((MT_Vec3){-1, -0.65, -1}, 0.65f, mat_glossy);
    mt_world_add_object(world, sphere, MT_OBJECT_SPHERE);

    mt_world_recalculate_bvh(world);

    RaylibInstance instance = raylib_instance_create((MT_Vec3 *)malloc(sizeof(MT_Vec3) * render_width * render_height), render_width, render_height, render_scale, 2500, 200);
    while (!WindowShouldClose())
    {
        if (b_log_samples)
        {
            printf("[Renderer] Sample: %d\n", mt_renderer_get_progressive_index(renderer) - 1);
        }
        mt_render(renderer);

        if (b_enable_controls)
        {
            if (raylib_handle_movement(camera, camera_speed))
            {
                mt_renderer_reset_progressive(renderer);
            }
        }

        mt_renderer_get_pixels(renderer, instance.render_pixels, 0.75f, 1);
        raylib_display(instance);

        raylib_handle_debug_input(instance, camera);
    }

    mt_renderer_delete(renderer);
    mt_world_delete(world);
    mt_camera_delete(camera);

    raylib_instance_delete(instance);

    return 0;
}
