#include "render.h"

#include <raylib.h>

void raylib_display(RaylibInstance instance)
{
    BeginTextureMode(instance.target);
    ClearBackground(BLACK);

    for (int y = 0; y < instance.display_height; ++y)
    {
        for (int x = 0; x < instance.display_width; ++x)
        {
            int index = (y * instance.display_width + x);
            instance.display_pixels[index] = (Color){
                (unsigned char)instance.render_pixels[index].x,
                (unsigned char)instance.render_pixels[index].y,
                (unsigned char)instance.render_pixels[index].z,
                (unsigned char)255U};
        }
    }
    UpdateTexture(instance.target.texture, instance.display_pixels);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle src = {0, 0, instance.display_width, instance.display_height};
    Rectangle dest = {0, 0, instance.display_width * instance.window_scale, instance.display_height * instance.window_scale};
    DrawTexturePro(instance.target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);

    char fpsText[32];
    sprintf(fpsText, "FPS: %d", GetFPS());
            DrawText(fpsText, 0, 0, 20, GREEN);

    EndDrawing();
}
