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

void set_dimensions(int w, int h);

const char* inttostr(int n);

void sys_update();

#endif