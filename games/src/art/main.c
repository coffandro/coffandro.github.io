#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <colors.h>
#include <window.h>
#include "state.h"

State state;

void on_dimension_update(int w, int h) {
    printf("%d, %d\n", w, h);
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "");

    SetTargetFPS(60);

    state = (State){
        .framebuf = (Texture2D){},
    };

    // Main game loop
    while (!WindowShouldClose()) {
        sys_update();
        
        float deltaTime = GetFrameTime();

        

        BeginDrawing();
            ClearBackground(COL_BG);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
