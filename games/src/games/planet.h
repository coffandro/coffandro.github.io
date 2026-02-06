#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    float radius;
    float scale;
    Texture2D texture;
    float rotation;
    float rotationSpeed;
} Planet;

void Planet_Init(Planet *planet, Texture2D texture, float minX, float maxX, float minY, float maxY);
void Planet_Update(Planet *planet, float deltaTime);
void Planet_Draw(Planet *planet);

int Planets_Init(Planet *planets, int maxCount, const char *dirPath, float minX, float maxX, float minY, float maxY, float minSpacing);
void Planets_Update(Planet *planets, int count, float deltaTime);
void Planets_Draw(Planet *planets, int count);

#endif // PLANET_H