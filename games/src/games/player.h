#ifndef PLAYER_H
#define PLAYER_H

#include <window.h>

#define SPEED 100
#define DRAW_SCALE 0.1
#define ORBIT_RADIUS   50.0f
#define TANGENT_WEIGHT 1.0f
#define RADIAL_WEIGHT  1.25f
#define ROTATION_SPEED 180.0f // degrees per second
#define ORBIT_THRESHOLD 100

void init_player();
void update_player();
void draw_player();

#endif