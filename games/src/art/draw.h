#pragma once

#include "state.h"

#define EDGE_LINE_WIDTH 10.0f
#define REGULAR_CURSOR "/tmp/assets/CursorDefault.png"
#define HOVER_CURSOR "/tmp/assets/CursorHover.png"
#define CLICKED_CURSOR "/tmp/assets/CursorClick.png"

typedef struct {
    Texture2D regular;
    Texture2D hover;
    Texture2D clicked;
} Cursors;

void draw();
Cursors LoadCursorTextures();
void DrawCursor(Texture2D cursor);