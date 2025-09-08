#include "../minitracer.h"

#include <raylib.h>
#include "raylib_utils/common.h"

int main(void)
{
    int width = 320;
    int height = 180;
    int render_scale = 1; // scales render width and height
    int render_width = width * render_scale;
    int render_height = height * render_scale;
    float camera_speed = 0.025f * render_scale; // scale speed with render scale to account for lag
    int b_log_samples = 0;
    int b_enable_controls = 1;

    MT_World *world = mt_world_create(1000);
    MT_Camera *camera = mt_camera_create();
    MT_Environment *environment = mt_environment_create();
    environment->brightness = 1;
    mt_world_set_environment(world, environment);

    MT_Renderer *renderer = mt_renderer_create(render_width, render_height, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 5000);
    mt_renderer_set_bounces(renderer, 64);
    mt_renderer_enable_progressive(renderer, 1);
    mt_renderer_enable_antialiasing(renderer, 1);
    mt_renderer_enable_bvh(renderer, 1);

    camera->position.x = 14.178;
    camera->position.y = 1.525;
    camera->position.z = 12.026;
    camera->rotation.x = -0.075;
    camera->rotation.y = -5.424;
    camera->rotation.z = 0;
    camera->fov = 1;

    MT_Material *mat_diffuse_red = mt_material_create();
    mat_diffuse_red->color = (MT_Vec3){1, 0.5, 0.5};
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.05f;
    MT_Material *mat_glossy_green = mt_material_create();
    mat_glossy_green->color = (MT_Vec3){0.5, 1, 0.5};
    mat_glossy_green->roughness = 0;
    MT_Material *mat_glass = mt_material_create();
    mat_glass->color = (MT_Vec3){0.5, 0.5, 1};
    mat_glass->roughness = 0;
    mat_glass->b_is_refractive = 1;
    mat_glass->ior = 1.1f;
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 50.0f;
    mat_light->color = (MT_Vec3){0.5, 1, 0.5};

    MT_Sphere *sphere = mt_sphere_create((MT_Vec3){0, 0, 25}, 10.0f, mat_glossy_green);
    mt_world_add_object(world, sphere, MT_OBJECT_SPHERE);

    MT_Sphere *sphere2 = mt_sphere_create((MT_Vec3){0, -20, 25}, 10.0f, mat_glass);
    mt_world_add_object(world, sphere2, MT_OBJECT_SPHERE);

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 15, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){100, 1, 100}, mat_glossy);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);

    mt_random_init();
    for (int i = 0; i < 500; ++i)
    {
        MT_Mesh *cube = mt_mesh_create_cube((MT_Vec3){mt_random_float() * 10, mt_random_float() * 10, mt_random_float() * 10}, (MT_Vec3){0, 0, 0}, (MT_Vec3){1, 1, 1}, mat_diffuse_red);
        mt_world_add_object(world, cube, MT_OBJECT_MESH);
    }

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

        mt_renderer_get_pixels(renderer, instance.render_pixels, 1.2f, 1);
        raylib_display(instance);

        raylib_handle_debug_input(instance, camera);
    }

    mt_renderer_delete(renderer);
    mt_world_delete(world);
    mt_camera_delete(camera);
    mt_material_delete(mat_diffuse_red);
    mt_material_delete(mat_glass);
    mt_material_delete(mat_glossy);
    mt_material_delete(mat_glossy_green);
    mt_material_delete(mat_light);

    raylib_instance_delete(instance);

    return 0;
}
