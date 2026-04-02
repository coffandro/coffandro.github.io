#include "draw.h"
#include "rlgl.h"
#include "map.h"
#include <colors.h>
#include <math.h>

Cursors LoadCursorTextures() {
    return (Cursors){
        .regular = LoadTexture(REGULAR_CURSOR),
        .hover = LoadTexture(HOVER_CURSOR),
        .clicked = LoadTexture(CLICKED_CURSOR)
    };
}

void DrawCursor(Texture2D cursor) {
    float targetH = state.height / 4.0f;
    float scale = targetH / cursor.height;
    float drawW = cursor.width * scale;
    float margin = targetH/2;
    float x = state.width - drawW - margin;
    float y = state.height - targetH - margin;
    DrawTextureEx(cursor, (Vector2){x, y}, 0.0f, scale, WHITE);
}

void draw() {
    for (int i = 0; i < mapModelCount; i++)
        DrawModel(mapModels[i], (Vector3){0, 0, 0}, 1.0f, COL_BG);

    // Draw wall paintings
    for (int i = 0; i < wallPaintingCount; i++) {
        WallPainting *p = &wallPaintings[i];
        Texture2D tex = wallTextures[p->textureIndex];
        if (tex.id == 0) continue;

        float hw = p->width / 2.0f;
        float hh = p->height / 2.0f;

        Vector3 tl = { p->center.x + p->up.x*hh - p->right.x*hw,
                        p->center.y + p->up.y*hh - p->right.y*hw,
                        p->center.z + p->up.z*hh - p->right.z*hw };
        Vector3 tr = { p->center.x + p->up.x*hh + p->right.x*hw,
                        p->center.y + p->up.y*hh + p->right.y*hw,
                        p->center.z + p->up.z*hh + p->right.z*hw };
        Vector3 br = { p->center.x - p->up.x*hh + p->right.x*hw,
                        p->center.y - p->up.y*hh + p->right.y*hw,
                        p->center.z - p->up.z*hh + p->right.z*hw };
        Vector3 bl = { p->center.x - p->up.x*hh - p->right.x*hw,
                        p->center.y - p->up.y*hh - p->right.y*hw,
                        p->center.z - p->up.z*hh - p->right.z*hw };

        rlSetTexture(tex.id);
        rlBegin(RL_QUADS);
            rlColor4ub(255, 255, 255, 255);
            rlTexCoord2f(0, 0); rlVertex3f(tl.x, tl.y, tl.z);
            rlTexCoord2f(1, 0); rlVertex3f(tr.x, tr.y, tr.z);
            rlTexCoord2f(1, 1); rlVertex3f(br.x, br.y, br.z);
            rlTexCoord2f(0, 1); rlVertex3f(bl.x, bl.y, bl.z);
        rlEnd();
        rlSetTexture(0);

        // Draw title below painting
        if (p->titleTexture.id != 0) {
            float gap = 0.1f;
            float thw = p->titleWidth / 2.0f;
            float thh = p->titleHeight / 2.0f;
            float titleOffY = -(hh + gap + thh);

            Vector3 tc = { p->center.x + p->up.x*titleOffY,
                           p->center.y + p->up.y*titleOffY,
                           p->center.z + p->up.z*titleOffY };

            Vector3 ttl = { tc.x + p->up.x*thh - p->right.x*thw,
                            tc.y + p->up.y*thh - p->right.y*thw,
                            tc.z + p->up.z*thh - p->right.z*thw };
            Vector3 ttr = { tc.x + p->up.x*thh + p->right.x*thw,
                            tc.y + p->up.y*thh + p->right.y*thw,
                            tc.z + p->up.z*thh + p->right.z*thw };
            Vector3 tbr = { tc.x - p->up.x*thh + p->right.x*thw,
                            tc.y - p->up.y*thh + p->right.y*thw,
                            tc.z - p->up.z*thh + p->right.z*thw };
            Vector3 tbl = { tc.x - p->up.x*thh - p->right.x*thw,
                            tc.y - p->up.y*thh - p->right.y*thw,
                            tc.z - p->up.z*thh - p->right.z*thw };

            rlSetTexture(p->titleTexture.id);
            rlBegin(RL_QUADS);
                rlColor4ub(255, 255, 255, 255);
                rlTexCoord2f(0, 0); rlVertex3f(ttl.x, ttl.y, ttl.z);
                rlTexCoord2f(1, 0); rlVertex3f(ttr.x, ttr.y, ttr.z);
                rlTexCoord2f(1, 1); rlVertex3f(tbr.x, tbr.y, tbr.z);
                rlTexCoord2f(0, 1); rlVertex3f(tbl.x, tbl.y, tbl.z);
            rlEnd();
            rlSetTexture(0);
        }
    }

    rlCheckRenderBatchLimit(mapEdgeVertCount);
    rlSetLineWidth(EDGE_LINE_WIDTH);
    rlBegin(RL_LINES);
        rlColor4ub(COL_TEXT.r, COL_TEXT.g, COL_TEXT.b, COL_TEXT.a);
        for (int i = 0; i < mapEdgeVertCount; i += 2) {
            rlVertex3f(mapEdgeVerts[i].x, mapEdgeVerts[i].y, mapEdgeVerts[i].z);
            rlVertex3f(mapEdgeVerts[i+1].x, mapEdgeVerts[i+1].y, mapEdgeVerts[i+1].z);
        }
    rlEnd();
}
