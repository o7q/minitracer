#include "../minitracer.h"

int main(void)
{
    int width = 320;
    int height = 180;

    MT_World *world = mt_world_create(1000);
    MT_Environment *environment = mt_environment_create();
    environment->brightness = 1;
    mt_world_set_environment(world, environment);

    MT_Camera *camera = mt_camera_create();
    camera->position.z = 4;
    camera->position.y = -0.5f;

    MT_Renderer *renderer = mt_renderer_create(width, height, 16);
    mt_renderer_set_world(renderer, world);
    mt_renderer_set_camera(renderer, camera);
    mt_renderer_set_samples(renderer, 64);
    mt_renderer_set_bounces(renderer, 4);
    mt_renderer_enable_progressive(renderer, 0);
    mt_renderer_enable_antialiasing(renderer, 1);

    MT_Material *mat_diffuse = mt_material_create();
    mat_diffuse->color = (MT_Vec3){1, 0, 0};
    MT_Material *mat_glossy = mt_material_create();
    mat_glossy->roughness = 0.1f;

    MT_Mesh *floor = mt_mesh_create_plane((MT_Vec3){0, 0, 0}, (MT_Vec3){0, 0, 0}, (MT_Vec3){50, 1, 50}, mat_glossy);
    mt_world_add_object(world, floor, MT_OBJECT_MESH);
    MT_Mesh *cube = mt_mesh_create_cube((MT_Vec3){0, -0.5f, 0}, (MT_Vec3){0, MT_PI / 4.0f, 0}, (MT_Vec3){1, 1, 1}, mat_diffuse);
    mt_world_add_object(world, cube, MT_OBJECT_MESH);

    mt_world_recalculate_bvh(world);

    mt_render(renderer);

    MT_Vec3 *pixels = (MT_Vec3 *)malloc(sizeof(MT_Vec3) * width * height);
    mt_renderer_get_pixels(renderer, pixels, 1.0f, 1);
    mt_bmp_write("render.bmp", pixels, width, height);

    return 0;
}
