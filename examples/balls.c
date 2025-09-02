#include "../minitracer.h"

#include <raylib.h>
#include "raylib_utils/common.h"

int main(void)
{
    int width = 320;
    int height = 180;
    int render_scale = 1;                       // scales render width and height
    float camera_speed = 0.025f * render_scale; // scale speed with render scale to account for lag
    int b_log_samples = 1;
    int b_enable_controls = 1;

    MT_World *world = mt_world_create(1000);
    MT_Camera *camera = mt_camera_create();
    MT_Environment *environment = mt_environment_create();
    environment->brightness = 0.0f;
    mt_world_set_environment(world, environment);

    MT_Renderer *renderer = mt_renderer_create(width * render_scale, height * render_scale, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 5000);
    mt_renderer_set_bounces(renderer, 64);
    mt_renderer_set_progressive(renderer, 1);
    mt_renderer_set_antialiasing(renderer, 1);

    camera->position.x = -1.359;
    camera->position.y = -4.5;
    camera->position.z = -6.807;
    camera->rotation.x = 0.487;
    camera->rotation.y = -2.95;
    camera->rotation.z = 0;
    camera->fov = 1.0f;

    MT_Material *mat_diffuse = mt_material_create();
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.0f;
    MT_Material *mat_light = mt_material_create();
    mat_light->emission_strength = 10.0f;
    mat_light->color = (MT_Vec3){255 / 255.0f, 241 / 255.0f, 201 / 255.0f};
    MT_Material *mat_glass = mt_material_create();
    mat_glass->b_is_refractive = 1;
    mat_glass->ior = 1.5f;
    MT_Material *mat_glass_red = mt_material_create();
    mat_glass_red->b_is_refractive = 1;
    mat_glass_red->ior = 1.5f;
    mat_glass_red->color = (MT_Vec3){1.0f, 0.5f, 0.5f};
    MT_Material *mat_glass_green = mt_material_create();
    mat_glass_green->b_is_refractive = 1;
    mat_glass_green->ior = 1.5f;
    mat_glass_green->color = (MT_Vec3){0.5f, 1.0f, 0.5f};
    MT_Material *mat_glass_blue = mt_material_create();
    mat_glass_blue->b_is_refractive = 1;
    mat_glass_blue->ior = 1.5f;
    mat_glass_blue->color = (MT_Vec3){0.5f, 0.5f, 1.0f};

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){50, 1, 50}, mat_diffuse);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);
    MT_Mesh *light = mt_mesh_create_plane((MT_Vec3){0, -4.9, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){2.5, 1, 2.5}, mat_light);
    mt_world_add_object(world, light, MT_OBJECT_MESH);

    MT_Sphere *ball = mt_sphere_create((MT_Vec3){0, -1, 0}, 1.0f, mat_glass);
    mt_world_add_object(world, ball, MT_OBJECT_SPHERE);

    MT_Sphere *ball2 = mt_sphere_create((MT_Vec3){-2, -1, 2}, 1.0f, mat_glass_red);
    mt_world_add_object(world, ball2, MT_OBJECT_SPHERE);

    MT_Sphere *ball3 = mt_sphere_create((MT_Vec3){-2, -1, -2}, 1.0f, mat_glass_green);
    mt_world_add_object(world, ball3, MT_OBJECT_SPHERE);

    MT_Sphere *ball4 = mt_sphere_create((MT_Vec3){2, -1, -2}, 1.0f, mat_glass_blue);
    mt_world_add_object(world, ball4, MT_OBJECT_SPHERE);

    MT_Sphere *ball5 = mt_sphere_create((MT_Vec3){2, -1, 2}, 1.0f, mat_glossy);
    mt_world_add_object(world, ball5, MT_OBJECT_SPHERE);

    RaylibInstance instance = raylib_instance_create(width, height, render_scale, 2500, 200);
    while (!WindowShouldClose())
    {
        if (b_log_samples)
        {
            printf("[Renderer] Sample: %d\n", mt_renderer_get_progressive_index(renderer));
        }
        mt_render(renderer);

        if (b_enable_controls)
        {
            if (raylib_handle_movement(camera, camera_speed))
            {
                mt_renderer_reset_progressive(renderer);
            }
        }

        raylib_render_pixels(instance, renderer);
    }

    mt_renderer_delete(renderer);
    mt_world_delete(world);
    mt_camera_delete(camera);

    raylib_instance_delete(instance);

    return 0;
}
