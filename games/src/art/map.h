#pragma once

#include "raylib.h"
#include <stdlib.h>

#define MAP_SIZE 10
#define CUBE_SIZE 4.0f
#define CUBE_HEIGHT (CUBE_SIZE * 1.5f)
#define MAX_MAP_MODELS 32

extern int MAPDATA[MAP_SIZE * MAP_SIZE];
extern Model mapModels[MAX_MAP_MODELS];
extern int mapModelCount;
extern Vector3 *mapEdgeVerts;
extern int mapEdgeVertCount;

static int mapFilled(int x, int z);
static void mapFloodFill(int *labels, int x, int z, int label);
static int mapAddQuad(float *v, float *n, int i, Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 norm);
void GenerateMapModels();
