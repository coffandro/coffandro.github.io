#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <colors.h>
#include <window.h>
#include "player.h"
#include "planet.h"

#define MAX_PLANETS 100

static Planet planets[MAX_PLANETS];
static int planetCount = 0;

int main(void) {
    InitWindow(screenWidth, screenHeight, "");

    SetTargetFPS(60);

    init_player();
    
    planetCount = Planets_Init(planets, MAX_PLANETS, "/tmp/assets/planets", 
                               0, screenWidth, 0, screenHeight, 150.0f);

    // Main game loop
    while (!WindowShouldClose()) {
        sys_update();
        
        float deltaTime = GetFrameTime();

        update_player();
        Planets_Update(planets, planetCount, deltaTime);

        BeginDrawing();
            ClearBackground(COL_BG);

            Planets_Draw(planets, planetCount);
            draw_player();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
