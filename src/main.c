#include <stdio.h>

#include <raylib.h>

#include "camera.h"
#include "object.h"
#include "render.h"

int main()
{
    int renderWidth = 100;
    int renderHeight = 80;
    int windowWidth = renderWidth * 8;
    int windowHeight = renderHeight * 8;

    Cam3 camera;
    camera.pos = (Vec3){0, 0, 0};
    camera.fov = 1.0f;
    camera.width = renderWidth;
    camera.height = renderHeight;

    Sphere sphere;
    sphere.pos = (Vec3){0, 0, -4};
    sphere.radius = 1.0f;

    InitWindow(windowWidth, windowHeight, "raytracer");
    RenderTexture2D target = LoadRenderTexture(renderWidth, renderHeight);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_SPACE))
            camera.pos.y -= 0.1f;
        if (IsKeyDown(KEY_LEFT_SHIFT))
            camera.pos.y += 0.1f;
        if (IsKeyDown(KEY_A))
            camera.pos.x -= 0.1f;
        if (IsKeyDown(KEY_D))
            camera.pos.x += 0.1f;
        if (IsKeyDown(KEY_W))
            camera.pos.z -= 0.1f;
        if (IsKeyDown(KEY_S))
            camera.pos.z += 0.1f;

        BeginTextureMode(target);

        ClearBackground(BLACK);
        render(&camera, &sphere);

        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);
        Rectangle src = {0, 0, (float)renderWidth, -(float)renderHeight};
        Rectangle dest = {0, 0, (float)windowWidth, (float)windowHeight};
        DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}