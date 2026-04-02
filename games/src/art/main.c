#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <colors.h>
#include <window.h>
#include "state.h"
#include "draw.h"
#include "map.h"

State state;

void on_dimension_update(int w, int h) {
    printf("%d, %d\n", w, h);
    state.width = w;
    state.height = h;
}

#define PLAYER_RADIUS 0.8f

static bool tile_blocked(int tx, int tz) {
    if (tx < 0 || tx >= MAP_SIZE || tz < 0 || tz >= MAP_SIZE)
        return true;
    return MAPDATA[tz * MAP_SIZE + tx] != 0;
}

bool can_move_to(float wx, float wz) {
    int min_tx = (int)floorf(wx / CUBE_SIZE + 0.5f - PLAYER_RADIUS / CUBE_SIZE);
    int max_tx = (int)floorf(wx / CUBE_SIZE + 0.5f + PLAYER_RADIUS / CUBE_SIZE);
    int min_tz = (int)floorf(wz / CUBE_SIZE + 0.5f - PLAYER_RADIUS / CUBE_SIZE);
    int max_tz = (int)floorf(wz / CUBE_SIZE + 0.5f + PLAYER_RADIUS / CUBE_SIZE);

    for (int tz = min_tz; tz <= max_tz; tz++)
        for (int tx = min_tx; tx <= max_tx; tx++)
            if (tile_blocked(tx, tz))
                return false;
    return true;
}

void clamp_movement(Vector3 next_pos) {
    if (can_move_to(next_pos.x, state.camera.position.z)) {
        state.camera.position.x = next_pos.x;
    }

    if (can_move_to(state.camera.position.x, next_pos.z)) {
        state.camera.position.z = next_pos.z;
    }
}

void move() {
    // Mouse lock
    if (!mouseLocked) {
        return;
    }

    float dt = GetFrameTime();
    float moveSpeed = 10.0f * dt;
    float mouseSensitivity = 0.003f;

    
    Vector2 mouseDelta = GetMouseDelta();
    state.yaw -= mouseDelta.x * mouseSensitivity;
    state.pitch -= mouseDelta.y * mouseSensitivity;

    // Clamp pitch to avoid flipping
    if (state.pitch > 1.5f)
        state.pitch = 1.5f;
    if (state.pitch < -1.5f)
        state.pitch = -1.5f;

    // Forward/right vectors (horizontal plane only)
    Vector3 forward = {
        cosf(state.pitch) * sinf(state.yaw),
        0.0f,
        cosf(state.pitch) * cosf(state.yaw),
    };
    float fLen = sqrtf(forward.x * forward.x + forward.z * forward.z);
    if (fLen > 0.0f)
    {
        forward.x /= fLen;
        forward.z /= fLen;
    }

    Vector3 right = {forward.z, 0.0f, -forward.x};
    Vector3 next_pos = {state.camera.position.x, state.camera.position.y, state.camera.position.z};

    // WASD movement
    if (IsKeyDown(KEY_W)) {
        next_pos.x += forward.x * moveSpeed;
        next_pos.z += forward.z * moveSpeed;
    }
    if (IsKeyDown(KEY_S)) {
        next_pos.x -= forward.x * moveSpeed;
        next_pos.z -= forward.z * moveSpeed;
    }
    if (IsKeyDown(KEY_A)) {
        next_pos.x += right.x * moveSpeed;
        next_pos.z += right.z * moveSpeed;
    }
    if (IsKeyDown(KEY_D)) {
        next_pos.x -= right.x * moveSpeed;
        next_pos.z -= right.z * moveSpeed;
    }

    clamp_movement(next_pos);

    // Update camera target from yaw/pitch
    state.camera.target = (Vector3){
        state.camera.position.x + cosf(state.pitch) * sinf(state.yaw),
        state.camera.position.y + sinf(state.pitch),
        state.camera.position.z + cosf(state.pitch) * cosf(state.yaw),
    };
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "");

    SetTargetFPS(60);

    state = (State){
        .width = screenWidth,
        .height = screenHeight,
        .camera = {0},
    };

    state.camera.position = PLAYER_LOCATION;
    state.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    state.camera.fovy = 45.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;

    // Initialize yaw/pitch looking toward the origin
    state.yaw = atan2f(0.0f, 0.0f);
    state.pitch = 0.0f;
    state.camera.target = (Vector3){
        state.camera.position.x + cosf(state.pitch) * sinf(state.yaw),
        state.camera.position.y + sinf(state.pitch),
        state.camera.position.z + cosf(state.pitch) * cosf(state.yaw),
    };

    GenerateMapModels();
    LoadWallTextures();

    // Main game loop
    while (!WindowShouldClose()) {
        sys_update();

        move();

        BeginDrawing();
        ClearBackground(COL_BG);

        BeginMode3D(state.camera);
        draw();
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
