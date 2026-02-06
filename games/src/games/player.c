#include "player.h"
#include <raymath.h>

Vector2 pos;
Texture2D texture;

void init_player() {
    UnloadTexture(texture);
    texture = LoadTexture("/tmp/assets/ship.png");
}

void update_player() {
    pos = Vector2Lerp(pos, mousePos, SPEED);
}

void draw_player() {
    DrawTextureEx(texture, pos, 0, 0.5, WHITE);
}
