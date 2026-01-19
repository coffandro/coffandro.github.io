#include "raylib.h"
#include <stdio.h>

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);
    Vector2 mouse_pos = (Vector2) {0,0};

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            mouse_pos = GetMousePosition();
            char* pos_x;
            char* pos_y;
            sprintf(pos_x, "%f", mouse_pos.x);
            sprintf(pos_y, "%f", mouse_pos.y);

            DrawText(TextFormat("Mouse Pos:", pos_x, pos_y), 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}