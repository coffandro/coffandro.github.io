#include "window.h"

int screenWidth = default_width;
int screenHeight = default_height;

Vector2 mousePos;

void on_dimension_update(int w, int h);

void set_dimensions(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    on_dimension_update(w, h);
}

const char* inttostr(int n) {
    char* result;
    if (n >= 0)
        result = malloc(floor(log10(n)) + 2);
    else
        result = malloc(floor(log10(n)) + 3);
    sprintf(result, "%d", n);
    return result;
}

void sys_update() {
    SetWindowSize(screenWidth, screenHeight);
    mousePos = GetMousePosition();
}