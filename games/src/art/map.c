#include "map.h"

int MAPDATA[MAP_SIZE * MAP_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 7, 0, 0, 0, 0, 0, 3, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 6, 0, 0, 0, 0, 0, 4, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1,
};

Model mapModels[MAX_MAP_MODELS];
int mapModelCount = 0;
Vector3 *mapEdgeVerts = NULL;
int mapEdgeVertCount = 0;

Texture2D wallTextures[MAX_WALL_TEXTURES];
int wallTextureCount = 0;
WallPainting wallPaintings[MAX_WALL_PAINTINGS];
int wallPaintingCount = 0;

static const char *wallTextureFiles[] = {
    "/tmp/assets/guide.png",
    "/tmp/assets/monkey_art.png",
    "/tmp/assets/space_suit.png",
    "/tmp/assets/sitting_at_desk.png",
    "/tmp/assets/coffandro.png",
    "/tmp/assets/torture_hardware.png",
};
static const char *wallTextureTitles[] = {
    "Guide",
    "Someone forgot their plushie\nin a cave :(",
    "Remember your god damned\nspace suit!",
    "Sitting at desk",
    "A little voxel avatar of me",
    "\"Torture Hardware\"\na metal album cover",
};
#define WALL_TEXTURE_FILE_COUNT 6
#define TITLE_FONT_SIZE 20
#define TITLE_PADDING 4

static int mapFilled(int x, int z) {
    if (x < 0 || x >= MAP_SIZE || z < 0 || z >= MAP_SIZE)
        return 0;
    return MAPDATA[z * MAP_SIZE + x];
}

static void mapFloodFill(int *labels, int startX, int startZ, int label) {
    int stackSize = MAP_SIZE * MAP_SIZE;
    int *stackX = (int *)malloc(stackSize * sizeof(int));
    int *stackZ = (int *)malloc(stackSize * sizeof(int));
    int top = 0;

    stackX[top] = startX;
    stackZ[top] = startZ;
    top++;

    while (top > 0) {
        top--;
        int x = stackX[top];
        int z = stackZ[top];

        if (x < 0 || x >= MAP_SIZE || z < 0 || z >= MAP_SIZE)
            continue;
        if (!MAPDATA[z * MAP_SIZE + x] || labels[z * MAP_SIZE + x])
            continue;

        labels[z * MAP_SIZE + x] = label;

        stackX[top] = x + 1; stackZ[top] = z; top++;
        stackX[top] = x - 1; stackZ[top] = z; top++;
        stackX[top] = x;     stackZ[top] = z + 1; top++;
        stackX[top] = x;     stackZ[top] = z - 1; top++;
    }

    free(stackX);
    free(stackZ);
}

static int mapAddQuad(float *v, float *n, int i, Vector3 a, Vector3 b, Vector3 c, Vector3 d, Vector3 norm) {
    float *vp = v + i * 3, *np = n + i * 3;
    vp[0] = a.x;
    vp[1] = a.y;
    vp[2] = a.z;
    np[0] = norm.x;
    np[1] = norm.y;
    np[2] = norm.z;
    vp[3] = b.x;
    vp[4] = b.y;
    vp[5] = b.z;
    np[3] = norm.x;
    np[4] = norm.y;
    np[5] = norm.z;
    vp[6] = c.x;
    vp[7] = c.y;
    vp[8] = c.z;
    np[6] = norm.x;
    np[7] = norm.y;
    np[8] = norm.z;
    vp[9] = a.x;
    vp[10] = a.y;
    vp[11] = a.z;
    np[9] = norm.x;
    np[10] = norm.y;
    np[11] = norm.z;
    vp[12] = c.x;
    vp[13] = c.y;
    vp[14] = c.z;
    np[12] = norm.x;
    np[13] = norm.y;
    np[14] = norm.z;
    vp[15] = d.x;
    vp[16] = d.y;
    vp[17] = d.z;
    np[15] = norm.x;
    np[16] = norm.y;
    np[17] = norm.z;
    return i + 6;
}

void GenerateMapModels() {
    float H = CUBE_SIZE / 2.0f;
    float HY = CUBE_HEIGHT / 2.0f;

    // Find connected components via flood fill
    int labels[MAP_SIZE * MAP_SIZE] = {0};
    int labelCount = 0;
    for (int z = 0; z < MAP_SIZE; z++)
        for (int x = 0; x < MAP_SIZE; x++)
            if (MAPDATA[z * MAP_SIZE + x] && !labels[z * MAP_SIZE + x])
                mapFloodFill(labels, x, z, ++labelCount);

    // Build one mesh per connected component (exposed faces only)
    for (int comp = 1; comp <= labelCount && mapModelCount < MAX_MAP_MODELS; comp++)
    {
        int faceCount = 0;
        for (int z = 0; z < MAP_SIZE; z++)
            for (int x = 0; x < MAP_SIZE; x++)
                if (labels[z * MAP_SIZE + x] == comp)
                {
                    faceCount += 2; // top + bottom always exposed
                    if (!mapFilled(x + 1, z))
                        faceCount++;
                    if (!mapFilled(x - 1, z))
                        faceCount++;
                    if (!mapFilled(x, z + 1))
                        faceCount++;
                    if (!mapFilled(x, z - 1))
                        faceCount++;
                }

        int vertCount = faceCount * 6;
        float *verts = (float *)calloc(vertCount * 3, sizeof(float));
        float *norms = (float *)calloc(vertCount * 3, sizeof(float));
        int vi = 0;

        for (int z = 0; z < MAP_SIZE; z++)
            for (int x = 0; x < MAP_SIZE; x++)
                if (labels[z * MAP_SIZE + x] == comp)
                {
                    float cx = x * CUBE_SIZE, cz = z * CUBE_SIZE;
                    // Top (y+)
                    vi = mapAddQuad(verts, norms, vi,
                                    (Vector3){cx - H, HY, cz + H}, (Vector3){cx + H, HY, cz + H},
                                    (Vector3){cx + H, HY, cz - H}, (Vector3){cx - H, HY, cz - H},
                                    (Vector3){0, 1, 0});
                    // Bottom (y-)
                    vi = mapAddQuad(verts, norms, vi,
                                    (Vector3){cx - H, -HY, cz - H}, (Vector3){cx + H, -HY, cz - H},
                                    (Vector3){cx + H, -HY, cz + H}, (Vector3){cx - H, -HY, cz + H},
                                    (Vector3){0, -1, 0});
                    // Right (x+)
                    if (!mapFilled(x + 1, z))
                        vi = mapAddQuad(verts, norms, vi,
                                        (Vector3){cx + H, -HY, cz - H}, (Vector3){cx + H, HY, cz - H},
                                        (Vector3){cx + H, HY, cz + H}, (Vector3){cx + H, -HY, cz + H},
                                        (Vector3){1, 0, 0});
                    // Left (x-)
                    if (!mapFilled(x - 1, z))
                        vi = mapAddQuad(verts, norms, vi,
                                        (Vector3){cx - H, -HY, cz + H}, (Vector3){cx - H, HY, cz + H},
                                        (Vector3){cx - H, HY, cz - H}, (Vector3){cx - H, -HY, cz - H},
                                        (Vector3){-1, 0, 0});
                    // Front (z+)
                    if (!mapFilled(x, z + 1))
                        vi = mapAddQuad(verts, norms, vi,
                                        (Vector3){cx + H, -HY, cz + H}, (Vector3){cx + H, HY, cz + H},
                                        (Vector3){cx - H, HY, cz + H}, (Vector3){cx - H, -HY, cz + H},
                                        (Vector3){0, 0, 1});
                    // Back (z-)
                    if (!mapFilled(x, z - 1))
                        vi = mapAddQuad(verts, norms, vi,
                                        (Vector3){cx - H, -HY, cz - H}, (Vector3){cx - H, HY, cz - H},
                                        (Vector3){cx + H, HY, cz - H}, (Vector3){cx + H, -HY, cz - H},
                                        (Vector3){0, 0, -1});
                }

        Mesh mesh = {0};
        mesh.vertexCount = vi;
        mesh.triangleCount = vi / 3;
        mesh.vertices = verts;
        mesh.normals = norms;
        UploadMesh(&mesh, false);
        mapModels[mapModelCount++] = LoadModelFromMesh(mesh);
    }

    // Compute outline edges (only at actual shape corners, not between adjacent cubes)
    int maxVerts = (MAP_SIZE * (MAP_SIZE + 1) * 2 + (MAP_SIZE + 1) * (MAP_SIZE + 1)) * 4;
    mapEdgeVerts = (Vector3 *)malloc(maxVerts * sizeof(Vector3));
    mapEdgeVertCount = 0;

    // Horizontal edges parallel to X at y=+H and y=-H
    // Draw where a filled/empty boundary exists in Z direction
    for (int cx = 0; cx < MAP_SIZE; cx++)
    {
        for (int bz = 0; bz <= MAP_SIZE; bz++)
        {
            if (mapFilled(cx, bz - 1) != mapFilled(cx, bz))
            {
                float wx0 = cx * CUBE_SIZE - H, wx1 = cx * CUBE_SIZE + H;
                float wz = bz * CUBE_SIZE - H;
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx0, HY, wz};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx1, HY, wz};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx0, -HY, wz};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx1, -HY, wz};
            }
        }
    }

    // Horizontal edges parallel to Z at y=+H and y=-H
    // Draw where a filled/empty boundary exists in X direction
    for (int cz = 0; cz < MAP_SIZE; cz++)
    {
        for (int bx = 0; bx <= MAP_SIZE; bx++)
        {
            if (mapFilled(bx - 1, cz) != mapFilled(bx, cz))
            {
                float wx = bx * CUBE_SIZE - H;
                float wz0 = cz * CUBE_SIZE - H, wz1 = cz * CUBE_SIZE + H;
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, HY, wz0};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, HY, wz1};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, -HY, wz0};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, -HY, wz1};
            }
        }
    }

    // Vertical edges at grid corners
    // Draw at convex/concave corners (odd filled count or diagonal pair)
    for (int bx = 0; bx <= MAP_SIZE; bx++)
    {
        for (int bz = 0; bz <= MAP_SIZE; bz++)
        {
            int a = mapFilled(bx - 1, bz - 1);
            int b = mapFilled(bx, bz - 1);
            int c = mapFilled(bx - 1, bz);
            int d = mapFilled(bx, bz);
            if ((a ^ b ^ c ^ d) || (a == d && a != b && b == c))
            {
                float wx = bx * CUBE_SIZE - H;
                float wz = bz * CUBE_SIZE - H;
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, -HY, wz};
                mapEdgeVerts[mapEdgeVertCount++] = (Vector3){wx, HY, wz};
            }
        }
    }
}

void LoadWallTextures() {
    for (int i = 0; i < WALL_TEXTURE_FILE_COUNT; i++) {
        Image img = LoadImage(wallTextureFiles[i]);
        ImageFlipHorizontal(&img);
        wallTextures[i] = LoadTextureFromImage(img);
        UnloadImage(img);
        if (wallTextures[i].id != 0) wallTextureCount = i + 1;
    }

    // Pre-render title textures (one per texture index)
    Texture2D titleTextures[WALL_TEXTURE_FILE_COUNT] = {0};
    float titleWorldW[WALL_TEXTURE_FILE_COUNT] = {0};
    float titleWorldH[WALL_TEXTURE_FILE_COUNT] = {0};
    Font font = GetFontDefault();

    for (int i = 0; i < WALL_TEXTURE_FILE_COUNT; i++) {
        if (wallTextures[i].id == 0) continue;
        Vector2 size = MeasureTextEx(font, wallTextureTitles[i], TITLE_FONT_SIZE, 1);
        int imgW = (int)(size.x + 0.5f) + TITLE_FONT_SIZE * 2;
        int imgH = (int)(size.y + 0.5f) + TITLE_FONT_SIZE * 2;

        Image img = GenImageColor(imgW, imgH, BLANK);
        int drawX = (imgW - (int)(size.x + 0.5f)) / 2;
        int drawY = (imgH - (int)(size.y + 0.5f)) / 2;
        ImageDrawTextEx(&img, font, wallTextureTitles[i],
                        (Vector2){drawX, drawY},
                        TITLE_FONT_SIZE, 1, WHITE);
        ImageFlipHorizontal(&img);
        titleTextures[i] = LoadTextureFromImage(img);
        UnloadImage(img);

        float tAspect = (float)imgW / (float)imgH;
        float twH = CUBE_HEIGHT * 0.15f;
        float twW = twH * tAspect;
        if (twW > CUBE_SIZE * 0.9f) {
            twW = CUBE_SIZE * 0.9f;
            twH = twW / tAspect;
        }
        titleWorldW[i] = twW;
        titleWorldH[i] = twH;
    }

    float H = CUBE_SIZE / 2.0f;
    float paintMaxW = CUBE_SIZE * 0.7f;
    float paintMaxH = CUBE_HEIGHT * 0.7f;
    float offset = 0.01f;

    for (int z = 0; z < MAP_SIZE; z++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            int val = MAPDATA[z * MAP_SIZE + x];
            if (val < 2) continue;

            int texIdx = val - 2;
            if (texIdx >= wallTextureCount || wallTextures[texIdx].id == 0) continue;

            float cx = x * CUBE_SIZE;
            float cz = z * CUBE_SIZE;

            float texW = (float)wallTextures[texIdx].width;
            float texH = (float)wallTextures[texIdx].height;
            float aspect = texW / texH;

            float pw, ph;
            if (aspect > paintMaxW / paintMaxH) {
                pw = paintMaxW;
                ph = pw / aspect;
            } else {
                ph = paintMaxH;
                pw = ph * aspect;
            }

            Texture2D tTex = titleTextures[texIdx];
            float tW = titleWorldW[texIdx];
            float tH = titleWorldH[texIdx];

            // Right face (x+)
            if (!mapFilled(x + 1, z) && wallPaintingCount < MAX_WALL_PAINTINGS) {
                wallPaintings[wallPaintingCount++] = (WallPainting){
                    .center = {cx + H + offset, 0, cz},
                    .right = {0, 0, 1},
                    .up = {0, 1, 0},
                    .width = pw, .height = ph,
                    .textureIndex = texIdx,
                    .titleTexture = tTex, .titleWidth = tW, .titleHeight = tH,
                };
            }
            // Left face (x-)
            if (!mapFilled(x - 1, z) && wallPaintingCount < MAX_WALL_PAINTINGS) {
                wallPaintings[wallPaintingCount++] = (WallPainting){
                    .center = {cx - H - offset, 0, cz},
                    .right = {0, 0, -1},
                    .up = {0, 1, 0},
                    .width = pw, .height = ph,
                    .textureIndex = texIdx,
                    .titleTexture = tTex, .titleWidth = tW, .titleHeight = tH,
                };
            }
            // Front face (z+)
            if (!mapFilled(x, z + 1) && wallPaintingCount < MAX_WALL_PAINTINGS) {
                wallPaintings[wallPaintingCount++] = (WallPainting){
                    .center = {cx, 0, cz + H + offset},
                    .right = {-1, 0, 0},
                    .up = {0, 1, 0},
                    .width = pw, .height = ph,
                    .textureIndex = texIdx,
                    .titleTexture = tTex, .titleWidth = tW, .titleHeight = tH,
                };
            }
            // Back face (z-)
            if (!mapFilled(x, z - 1) && wallPaintingCount < MAX_WALL_PAINTINGS) {
                wallPaintings[wallPaintingCount++] = (WallPainting){
                    .center = {cx, 0, cz - H - offset},
                    .right = {1, 0, 0},
                    .up = {0, 1, 0},
                    .width = pw, .height = ph,
                    .textureIndex = texIdx,
                    .titleTexture = tTex, .titleWidth = tW, .titleHeight = tH,
                };
            }
        }
    }
}