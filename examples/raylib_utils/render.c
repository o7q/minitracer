#include "render.h"

#include <raylib.h>

void raylib_render_pixels(RaylibInstance instance, MT_Renderer *renderer)
{
    BeginTextureMode(instance.target);
    ClearBackground(BLACK);

    int render_width = mt_renderer_get_width(renderer);
    int render_height = mt_renderer_get_height(renderer);

    for (int y = 0; y < render_height; ++y)
    {
        for (int x = 0; x < render_width; ++x)
        {
            MT_Vec3 pixel_color = mt_renderer_get_pixel(renderer, x, y, 1.0f, 1);
            instance.pixels[y * render_width + x] = (Color){(unsigned char)pixel_color.x,
                                                            (unsigned char)pixel_color.y,
                                                            (unsigned char)pixel_color.z,
                                                            255U};
        }
    }
    UpdateTexture(instance.target.texture, instance.pixels);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle src = {0, 0, render_width, render_height};
    Rectangle dest = {0, 0, render_width * instance.window_scale, render_height * instance.window_scale};
    DrawTexturePro(instance.target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}
