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
    environment->brightness = 0;
    mt_world_set_environment(world, environment);

    MT_Renderer *renderer = mt_renderer_create(render_width, render_height, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 5000);
    mt_renderer_set_bounces(renderer, 64);
    mt_renderer_enable_progressive(renderer, 1);
    mt_renderer_enable_antialiasing(renderer, 1);

    camera->position.x = 1.721;
    camera->position.y = -1.075;
    camera->position.z = 3.933;
    camera->rotation.x = 0;
    camera->rotation.y = 0.385;
    camera->rotation.z = 0;
    camera->fov = 1.250;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.6f;
    MT_Material *mat_glossy_floor = mt_material_create();
    mat_glossy_floor->roughness = 0.4f;
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 10.0f;
    MT_Material *mat_light2 = mt_material_create();
    mat_light2->emission_strength = 4.0f;
    MT_Material *mat_light3 = mt_material_create();
    mat_light3->emission_strength = 0.2f;
    mat_light->color = (MT_Vec3){255 / 255.0f, 241 / 255.0f, 201 / 255.0f};
    MT_Material *mat_glass = mt_material_create();
    mat_glass->b_is_refractive = 1;
    mat_glass->roughness = 0.0f;

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){50, 1, 50}, mat_glossy_floor);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);
    MT_Mesh *light = mt_mesh_create_plane((MT_Vec3){-2, -4.9, 2}, (MT_Vec3){-0.4, 0, -0.4}, (MT_Vec3){2.5, 1, 2.5}, mat_light2);
    mt_world_add_object(world, light, MT_OBJECT_MESH);
    MT_Mesh *light2 = mt_mesh_create_plane((MT_Vec3){2, -4.9, -2}, (MT_Vec3){0.4, 0, 0.4}, (MT_Vec3){2.5, 1, 2.5}, mat_light);
    mt_world_add_object(world, light2, MT_OBJECT_MESH);

    MT_Mesh *suzanne = mt_mesh_create_from_stl("./examples/models/suzanne.stl", (MT_Vec3){0, -1, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){1, 1, 1}, mat_glossy);
    mt_world_add_object(world, suzanne, MT_OBJECT_MESH);

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

        mt_renderer_get_pixels(renderer, instance.render_pixels, 1.0f, 1);
        raylib_display(instance);

        raylib_handle_debug_input(instance, camera);
    }

    mt_renderer_delete(renderer);
    mt_world_delete(world);
    mt_camera_delete(camera);

    raylib_instance_delete(instance);

    return 0;
}
