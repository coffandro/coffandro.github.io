#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HOVER_COL CLITERAL(Color){171, 108, 132}
#define FOCUS_COL CLITERAL(Color){97, 56, 84}
#define BG_COL CLITERAL(Color){41, 43, 48}
#define TEXT_COL CLITERAL(Color){255, 196, 209}

int screenWidth = 800;
int screenHeight = 450;

void set_dimensions(int w, int h) {
    screenWidth = w;
    screenHeight = h;
}

const char* inttostr(int n) {
    char* result;
    if (n >= 0)
        result = malloc(floor(log10(n)) + 2);
    else
        result = malloc(floor(log10(n)) + 3);
    sprintf(result, "%d", n);
    return result;
}

void DrawSphereBasic(Color color);

int main(void) {
    const float sunRadius = 4.0f;
    const float earthRadius = 0.6f;
    const float earthOrbitRadius = 8.0f;
    const float moonRadius = 0.16f;
    const float moonOrbitRadius = 1.5f;

    InitWindow(screenWidth, screenHeight, "");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 16.0f, 16.0f, 16.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    float rotationSpeed = 0.2f;         // General system rotation speed

    float earthRotation = 0.0f;         // Rotation of earth around itself (days) in degrees
    float earthOrbitRotation = 0.0f;    // Rotation of earth around the Sun (years) in degrees
    float moonRotation = 0.0f;          // Rotation of moon around itself
    float moonOrbitRotation = 0.0f;     // Rotation of moon around earth in degrees

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {
        SetWindowSize(screenWidth, screenHeight);
        UpdateCamera(&camera, CAMERA_ORBITAL);

        earthRotation += (5.0f*rotationSpeed);
        earthOrbitRotation += (365/360.0f*(5.0f*rotationSpeed)*rotationSpeed);
        moonRotation += (2.0f*rotationSpeed);
        moonOrbitRotation += (8.0f*rotationSpeed);

        BeginDrawing();
            ClearBackground(BG_COL);

            BeginMode3D(camera);

                rlPushMatrix();
                    rlScalef(sunRadius, sunRadius, sunRadius);          // Scale Sun
                    DrawSphereBasic(GOLD);                              // Draw the Sun
                rlPopMatrix();

                rlPushMatrix();
                    rlRotatef(earthOrbitRotation, 0.0f, 1.0f, 0.0f);    // Rotation for Earth orbit around Sun
                    rlTranslatef(earthOrbitRadius, 0.0f, 0.0f);         // Translation for Earth orbit

                    rlPushMatrix();
                        rlRotatef(earthRotation, 0.25, 1.0, 0.0);       // Rotation for Earth itself
                        rlScalef(earthRadius, earthRadius, earthRadius);// Scale Earth

                        DrawSphereBasic(BLUE);                          // Draw the Earth
                    rlPopMatrix();

                    rlRotatef(moonOrbitRotation, 0.0f, 1.0f, 0.0f);     // Rotation for Moon orbit around Earth
                    rlTranslatef(moonOrbitRadius, 0.0f, 0.0f);          // Translation for Moon orbit
                    rlRotatef(moonRotation, 0.0f, 1.0f, 0.0f);          // Rotation for Moon itself
                    rlScalef(moonRadius, moonRadius, moonRadius);       // Scale Moon

                    DrawSphereBasic(LIGHTGRAY);                         // Draw the Moon
                rlPopMatrix();

                // Some reference elements (not affected by previous matrix transformations)
                DrawCircle3D((Vector3){ 0.0f, 0.0f, 0.0f }, earthOrbitRadius, (Vector3){ 1, 0, 0 }, 90.0f, Fade(TEXT_COL, 1.0f));
                DrawGrid(20, 1.0f);

            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void DrawSphereBasic(Color color)
{
    int rings = 16;
    int slices = 16;

    // Make sure there is enough space in the internal render batch
    // buffer to store all required vertex, batch is reseted if required
    rlCheckRenderBatchLimit((rings + 2)*slices*6);

    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        for (int i = 0; i < (rings + 2); i++)
        {
            for (int j = 0; j < slices; j++)
            {
                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*i))*sinf(DEG2RAD*(j*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*i)),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*i))*cosf(DEG2RAD*(j*360/slices)));
                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*sinf(DEG2RAD*((j+1)*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*(i+1))),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*cosf(DEG2RAD*((j+1)*360/slices)));
                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*sinf(DEG2RAD*(j*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*(i+1))),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*cosf(DEG2RAD*(j*360/slices)));

                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*i))*sinf(DEG2RAD*(j*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*i)),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*i))*cosf(DEG2RAD*(j*360/slices)));
                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*(i)))*sinf(DEG2RAD*((j+1)*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*(i))),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*(i)))*cosf(DEG2RAD*((j+1)*360/slices)));
                rlVertex3f(cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*sinf(DEG2RAD*((j+1)*360/slices)),
                           sinf(DEG2RAD*(270+(180/(rings + 1))*(i+1))),
                           cosf(DEG2RAD*(270+(180/(rings + 1))*(i+1)))*cosf(DEG2RAD*((j+1)*360/slices)));
            }
        }
    rlEnd();
}
