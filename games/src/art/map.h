#pragma once

#include "raylib.h"
#include <raymath.h>
#include <stdlib.h>

#define MAP_SIZE 11
#define CUBE_SIZE 4.0f
#define CUBE_HEIGHT (CUBE_SIZE * 1.5f)
#define MAX_MAP_MODELS 32
#define PLAYER_LOCATION Vector3Scale((Vector3){ 5.0f, 0.0f, 1.0f }, CUBE_SIZE)
#define PLAYER_INIT_LOOK Vector3Scale((Vector3){ 5.0f, 0.0f, 1.0f }, CUBE_SIZE)
#define MAX_WALL_TEXTURES 16
#define MAX_WALL_PAINTINGS 64

typedef struct {
    Vector3 center;
    Vector3 right;
    Vector3 up;
    float width;
    float height;
    int textureIndex;
    Texture2D titleTexture;
    float titleWidth;
    float titleHeight;
} WallPainting;

extern int MAPDATA[MAP_SIZE * MAP_SIZE];
extern Model mapModels[MAX_MAP_MODELS];
extern int mapModelCount;
extern Vector3 *mapEdgeVerts;
extern int mapEdgeVertCount;
extern Texture2D wallTextures[MAX_WALL_TEXTURES];
extern int wallTextureCount;
extern WallPainting wallPaintings[MAX_WALL_PAINTINGS];
extern int wallPaintingCount;

static int mapFilled(int x, int z);
static void mapFloodFill(int *labels, int x, int z, int label);
static int mapAddQuad(float *v, float *n, int i, Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 norm);
void GenerateMapModels();
void LoadWallTextures();
