#include <stdio.h>

#include <raylib.h>

#include "camera.h"
#include "object.h"
#include "render.h"

int main()
{
    Camera3 camera;
    camera.pos = (Vec3){0, 0, 0};
    camera.fov = 1.0f;
    camera.width = 30;
    camera.height = 20;

    Sphere sphere;
    sphere.pos = (Vec3){0, 0, -4};
    sphere.radius = 1.0f;

    int renderWidth = 50;
    int renderHeight = 40;
    int windowWidth = renderWidth * 16;
    int windowHeight = renderHeight * 16;

    InitWindow(windowWidth, windowHeight, "raytracer");
    RenderTexture2D target = LoadRenderTexture(50, 40);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
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