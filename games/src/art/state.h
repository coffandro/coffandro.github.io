#pragma once
#include "raylib.h"

typedef struct {
    Texture2D framebuf;
} State;

void createFramebuf(State *state, int w, int h) {
    free(&state->framebuf);
}

extern State state;