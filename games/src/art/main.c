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

    // Raycast (DDA on XZ grid)
    float ray_dx = sinf(state.yaw);
    float ray_dz = cosf(state.yaw);

    // Player position in grid space (tile boundaries at integers)
    float pos_gx = state.camera.position.x / CUBE_SIZE + 0.5f;
    float pos_gz = state.camera.position.z / CUBE_SIZE + 0.5f;

    int map_x = (int)floorf(pos_gx);
    int map_z = (int)floorf(pos_gz);

    int step_x = (ray_dx >= 0) ? 1 : -1;
    int step_z = (ray_dz >= 0) ? 1 : -1;

    float delta_dist_x = (ray_dx == 0) ? 1e30f : fabsf(1.0f / ray_dx);
    float delta_dist_z = (ray_dz == 0) ? 1e30f : fabsf(1.0f / ray_dz);

    float side_dist_x = (ray_dx < 0)
        ? (pos_gx - map_x) * delta_dist_x
        : (map_x + 1.0f - pos_gx) * delta_dist_x;
    float side_dist_z = (ray_dz < 0)
        ? (pos_gz - map_z) * delta_dist_z
        : (map_z + 1.0f - pos_gz) * delta_dist_z;

    int hit = 0;
    float dist = 1e30f;

    for (int i = 0; i < MAP_SIZE * 2; i++) {
        if (side_dist_x < side_dist_z) {
            dist = side_dist_x;
            side_dist_x += delta_dist_x;
            map_x += step_x;
        } else {
            dist = side_dist_z;
            side_dist_z += delta_dist_z;
            map_z += step_z;
        }

        if (map_x < 0 || map_x >= MAP_SIZE || map_z < 0 || map_z >= MAP_SIZE)
            break;

        if (MAPDATA[map_z * MAP_SIZE + map_x] != 0) {
            hit = MAPDATA[map_z * MAP_SIZE + map_x];
            break;
        }
    }

    state.target_id = hit;
    state.target_dist = dist * CUBE_SIZE;
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
    Cursors cursors = LoadCursorTextures();

    bool mouseDown;

    // Main game loop
    while (!WindowShouldClose()) {
        sys_update();

        move();

        mouseDown = IsMouseButtonDown(0);
        
        if (IsMouseButtonPressed(0) && state.target_id > 1) {
            emit_game_event(0, (int)state.target_id); // The currently targetted wall
        }

        BeginDrawing();
        ClearBackground(COL_BG);

            BeginMode3D(state.camera);
                draw();
            EndMode3D();

            if (mouseDown) {
                DrawCursor(cursors.clicked);
            } else if (state.target_dist < TARGET_THRESHOLD && state.target_id > 1) {
                DrawCursor(cursors.hover);
            } else {
                DrawCursor(cursors.regular);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
