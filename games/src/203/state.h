#pragma once
#include "raylib.h"

#define TARGET_THRESHOLD 10.0f

typedef struct {
    int width, height;

    Camera3D camera;
    float yaw;
    float pitch;
    float target_id, target_dist;
} State;

extern State state;
