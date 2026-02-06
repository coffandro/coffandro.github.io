#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <colors.h>
#include <window.h>
#include "player.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "");

    SetTargetFPS(60);

    init_player();

    // Main game loop
    while (!WindowShouldClose()) {
        sys_update();

        update_player();

        BeginDrawing();
            ClearBackground(COL_BG);

            draw_player();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
