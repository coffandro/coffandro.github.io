#pragma once
#include "raylib.h"

typedef struct {
    int width, height;

    Camera3D camera;
    float yaw;
    float pitch;
} State;

extern State state;
