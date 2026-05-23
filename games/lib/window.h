#ifndef SYS_H
#define SYS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define default_width 800
#define default_height 450

extern int screenWidth;
extern int screenHeight;

extern Vector2 mousePos;
extern bool mouseLocked;
extern bool isTouchMode;

void set_touch_mode(int v);

void set_dimensions(int w, int h);

void cursor_lock();
void cursor_unlock();

const char* inttostr(int n);

void sys_update();
void draw_overlay();

void emit_game_event(int type, int data);

// Game-defined movement callback. Engine calls it once per frame from sys_update.
// dx in [-1, 1]: +1 = right (D / RightArrow / left-stick-right).
// dy in [-1, 1]: +1 = forward (W / UpArrow / left-stick-up).
extern void on_movement(float dx, float dy);

// Game-defined look/turn callback. Engine calls it once per frame from sys_update.
// Driven by the right touch joystick only (desktop mouse-look stays in-game).
// dx in [-1, 1]: +1 = turn right. dy in [-1, 1]: +1 = look up.
extern void on_look(float dx, float dy);

#endif