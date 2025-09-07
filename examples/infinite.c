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
    int b_log_samples = 1;
    int b_enable_controls = 1;

    MT_World *world = mt_world_create(1000);
    MT_Camera *camera = mt_camera_create();
    MT_Environment *environment = mt_environment_create();
    environment->brightness = 1.0f;
    mt_world_set_environment(world, environment);

    MT_Renderer *renderer = mt_renderer_create(render_width, render_height, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 5000);
    mt_renderer_set_bounces(renderer, 32);
    mt_renderer_set_progressive(renderer, 1);
    mt_renderer_set_antialiasing(renderer, 1);

    camera->position.x = 4.675;
    camera->position.y = 3.4;
    camera->position.z = 4.967;
    camera->rotation.x = -0.025;
    camera->rotation.y = 0.787;
    camera->rotation.z = 0;
    camera->fov = 1.0f;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.005f;
    mat_glossy->color = (MT_Vec3){1, 0.99, 0.99};
    MT_Material *mat_glossy_green = mt_material_create();
    mat_glossy_green->roughness = 0.0f;
    mat_glossy_green->color = (MT_Vec3){1, 1, 0.7};
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 1.0f;
    mat_light->color = (MT_Vec3){255 / 255.0f, 241 / 255.0f, 201 / 255.0f};
    MT_Material *mat_light2 = mt_material_create();
    mat_light2->emission_strength = 1.0f;
    mat_light2->color = (MT_Vec3){255 / 255.0f, 216 / 255.0f, 161 / 255.0f};
    MT_Material *mat_glass = mt_material_create();
    mat_glass->b_is_refractive = 1;
    mat_glass->ior = 1.5f;
    MT_Material *mat_glass2 = mt_material_create();
    mat_glass2->b_is_refractive = 1;
    mat_glass2->ior = 2.4f;
    mat_glass2->color = (MT_Vec3){1, 1, 1};

    MT_Mesh *reflection_cube = mt_mesh_create_cube((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){10, 10, 10}, mat_glossy);
    mt_world_add_object(world, reflection_cube, MT_OBJECT_MESH);
    MT_Mesh *light = mt_mesh_create_plane((MT_Vec3){0, -4.9, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){3, 1, 3}, mat_light);
    mt_world_add_object(world, light, MT_OBJECT_MESH);

    MT_Mesh *cube = mt_mesh_create_cube((MT_Vec3){0, 5 - 1 / 2.0f - 0.01, 0}, (MT_Vec3){0, 0.5, 0}, (MT_Vec3){1, 1, 1}, mat_diffuse);
    mt_world_add_object(world, cube, MT_OBJECT_MESH);
    MT_Mesh *cube2 = mt_mesh_create_cube((MT_Vec3){0, 5 - 1.25f / 2.0f - 0.01, 3.5}, (MT_Vec3){0, -0.3, 0}, (MT_Vec3){1.25, 1.25, 1.25}, mat_glass2);
    mt_world_add_object(world, cube2, MT_OBJECT_MESH);
    MT_Sphere *sphere = mt_sphere_create((MT_Vec3){-2, 5 - 1, 2}, 1.0f, mat_glass);
    mt_world_add_object(world, sphere, MT_OBJECT_SPHERE);
    MT_Sphere *sphere2 = mt_sphere_create((MT_Vec3){2, 5 - 1, -2}, 1.0f, mat_glossy_green);
    mt_world_add_object(world, sphere2, MT_OBJECT_SPHERE);
    MT_Sphere *sphere3 = mt_sphere_create((MT_Vec3){2, 5 - 0.25f, 0}, 0.25f, mat_light2);
    mt_world_add_object(world, sphere3, MT_OBJECT_SPHERE);

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
