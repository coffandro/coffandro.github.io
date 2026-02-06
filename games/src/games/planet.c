#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include "planet.h"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

#define SVG_DEFAULT_SIZE 256

static int IsSvgFile(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext && (strcmp(ext, ".svg") == 0 || strcmp(ext, ".SVG") == 0);
}

static Image LoadImageFromSvg(const char *path, int targetSize) {
    Image image = { 0 };
    
    NSVGimage *svg = nsvgParseFromFile(path, "px", 96.0f);
    if (svg == NULL) return image;
    
    // Calculate scale to fit target size
    float scale = (float)targetSize / fmaxf(svg->width, svg->height);
    int width = (int)(svg->width * scale);
    int height = (int)(svg->height * scale);
    
    NSVGrasterizer *rast = nsvgCreateRasterizer();
    if (rast == NULL) {
        nsvgDelete(svg);
        return image;
    }
    
    unsigned char *pixels = (unsigned char *)malloc(width * height * 4);
    if (pixels == NULL) {
        nsvgDeleteRasterizer(rast);
        nsvgDelete(svg);
        return image;
    }
    
    nsvgRasterize(rast, svg, 0, 0, scale, pixels, width, height, width * 4);
    
    image.data = pixels;
    image.width = width;
    image.height = height;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    
    nsvgDeleteRasterizer(rast);
    nsvgDelete(svg);
    
    return image;
}

void Planet_Init(Planet *planet, Texture2D texture, float minX, float maxX, float minY, float maxY) {
    planet->texture = texture;
    planet->scale = 0.15f;  // Scale down to 15% of original size
    planet->radius = (float)texture.width / 2.0f * planet->scale;
    planet->position.x = minX + (float)rand() / RAND_MAX * (maxX - minX);
    planet->position.y = minY + (float)rand() / RAND_MAX * (maxY - minY);
    planet->rotation = (float)rand() / RAND_MAX * 360.0f;
    planet->rotationSpeed = 5.0f + (float)rand() / RAND_MAX * 20.0f;
}

void Planet_Update(Planet *planet, float deltaTime) {
    planet->rotation += planet->rotationSpeed * deltaTime;
    if (planet->rotation >= 360.0f) planet->rotation -= 360.0f;
}

void Planet_Draw(Planet *planet) {
    float scaledW = (float)planet->texture.width * planet->scale;
    float scaledH = (float)planet->texture.height * planet->scale;
    Rectangle source = { 0, 0, (float)planet->texture.width, (float)planet->texture.height };
    Rectangle dest = { planet->position.x, planet->position.y, scaledW, scaledH };
    Vector2 origin = { scaledW / 2.0f, scaledH / 2.0f };
    DrawTexturePro(planet->texture, source, dest, origin, planet->rotation, WHITE);
}

static float Vector2Distance(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

int Planets_Init(Planet *planets, int maxCount, const char *dirPath, float minX, float maxX, float minY, float maxY, float minSpacing) {
    printf("Planets_Init: Opening directory '%s'\n", dirPath);
    
    DIR *dir = opendir(dirPath);
    if (!dir) {
        printf("Planets_Init: Failed to open directory\n");
        return 0;
    }

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL && count < maxCount) {
        // Skip . and ..
        if (entry->d_name[0] == '.') continue;

        // Build full path
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
        
        printf("Planets_Init: Loading '%s'\n", fullPath);

        // Load image - use nanosvg for SVG files, raylib for others
        Image image;
        if (IsSvgFile(entry->d_name)) {
            image = LoadImageFromSvg(fullPath, SVG_DEFAULT_SIZE);
        } else {
            image = LoadImage(fullPath);
        }
        if (image.data == NULL) {
            printf("Planets_Init: Failed to load image\n");
            continue;
        }

        // Pad to square if not already square
        if (image.width != image.height) {
            int size = (image.width > image.height) ? image.width : image.height;
            Image squareImage = GenImageColor(size, size, BLANK);
            int offsetX = (size - image.width) / 2;
            int offsetY = (size - image.height) / 2;
            Rectangle srcRec = { 0, 0, (float)image.width, (float)image.height };
            Rectangle dstRec = { (float)offsetX, (float)offsetY, (float)image.width, (float)image.height };
            ImageDraw(&squareImage, image, srcRec, dstRec, WHITE);
            UnloadImage(image);
            image = squareImage;
        }

        Texture2D texture = LoadTextureFromImage(image);
        UnloadImage(image);
        if (texture.id == 0) continue;

        int attempts = 0;
        int maxAttempts = 100;

        do {
            Planet_Init(&planets[count], texture, minX, maxX, minY, maxY);
            attempts++;

            int tooClose = 0;
            for (int j = 0; j < count; j++) {
                if (Vector2Distance(planets[count].position, planets[j].position) < minSpacing) {
                    tooClose = 1;
                    break;
                }
            }

            if (!tooClose) break;
        } while (attempts < maxAttempts);

        count++;
    }

    closedir(dir);
    printf("Planets_Init: Loaded %d planets\n", count);
    return count;
}

void Planets_Update(Planet *planets, int count, float deltaTime) {
    for (int i = 0; i < count; i++) {
        Planet_Update(&planets[i], deltaTime);
    }
}

void Planets_Draw(Planet *planets, int count) {
    for (int i = 0; i < count; i++) {
        Planet_Draw(&planets[i]);
    }
}
