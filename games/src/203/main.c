#include "raylib.h"
#include <math.h>
#include <colors.h>
#include <window.h>
#include "state.h"

#define MODEL_PATH "/tmp/assets/203_redesign.obj"

// World bounds on the horizontal plane (X / Z).
#define WORLD_MIN_X -5.5f
#define WORLD_MAX_X  4.9f
#define WORLD_MIN_Z -9.0f
#define WORLD_MAX_Z  1.6f

#define PLAYER_HEIGHT 1.7f
#define PLAYER_RADIUS 0.4f

#define BOUNDS_HEIGHT 3.0f

#define MOVE_SPEED        6.0f
#define MOUSE_SENSITIVITY 0.003f
#define LOOK_STICK_SPEED  5.0f   // radians/sec at full deflection
#define PITCH_LIMIT       1.5f

#define MODEL_SCALE 1.25f

State state;

static float move_dx = 0.0f;   // +1 = strafe right (D / Right / stick-right)
static float move_dy = 0.0f;   // +1 = forward    (W / Up    / stick-up)
static float look_dx = 0.0f;   // +1 = turn right
static float look_dy = 0.0f;   // +1 = look up

void on_dimension_update(int w, int h) {
    state.width = w;
    state.height = h;
}

void on_movement(float dx, float dy) {
    move_dx = dx;
    move_dy = dy;
}

void on_look(float dx, float dy) {
    look_dx = dx;
    look_dy = dy;
}

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void update_camera_target(void) {
    state.camera.target = (Vector3){
        state.camera.position.x + cosf(state.pitch) * sinf(state.yaw),
        state.camera.position.y + sinf(state.pitch),
        state.camera.position.z + cosf(state.pitch) * cosf(state.yaw),
    };
}

static void move(void) {
    if (!mouseLocked) return;

    float dt = GetFrameTime();
    float step = MOVE_SPEED * dt;

    if (!isTouchMode) {
        Vector2 mouseDelta = GetMouseDelta();
        state.yaw   -= mouseDelta.x * MOUSE_SENSITIVITY;
        state.pitch -= mouseDelta.y * MOUSE_SENSITIVITY;
    }
    state.yaw   -= look_dx * LOOK_STICK_SPEED * dt;
    state.pitch += look_dy * LOOK_STICK_SPEED * dt;
    state.pitch = clampf(state.pitch, -PITCH_LIMIT, PITCH_LIMIT);

    // Horizontal forward / right (ignore pitch so movement stays on ground plane).
    Vector3 forward = { sinf(state.yaw), 0.0f, cosf(state.yaw) };
    Vector3 right   = { forward.z,       0.0f, -forward.x };

    Vector3 next = state.camera.position;
    next.x += (forward.x * move_dy - right.x * move_dx) * step;
    next.z += (forward.z * move_dy - right.z * move_dx) * step;

    next.x = clampf(next.x, WORLD_MIN_X + PLAYER_RADIUS, WORLD_MAX_X - PLAYER_RADIUS);
    next.z = clampf(next.z, WORLD_MIN_Z + PLAYER_RADIUS, WORLD_MAX_Z - PLAYER_RADIUS);
    next.y = PLAYER_HEIGHT;

    state.camera.position = next;
    update_camera_target();
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);

    state = (State){
        .width  = screenWidth,
        .height = screenHeight,
        .camera = {0},
    };

    state.camera.position   = (Vector3){ 0.0f, PLAYER_HEIGHT, 0.0f };
    state.camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    state.camera.fovy       = 70.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;
    state.yaw   = 0.0f;
    state.pitch = 0.0f;
    update_camera_target();

    Model world = LoadModel(MODEL_PATH);

    static const char *vsCode =
        "#version 100\n"
        "attribute vec3 vertexPosition;\n"
        "attribute vec3 vertexNormal;\n"
        "uniform mat4 mvp;\n"
        "uniform mat4 matModel;\n"
        "varying vec3 fragNormal;\n"
        "void main() {\n"
        "    fragNormal = normalize(mat3(matModel) * vertexNormal);\n"
        "    gl_Position = mvp * vec4(vertexPosition, 1.0);\n"
        "}\n";

    static const char *fsCode =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec3 fragNormal;\n"
        "uniform vec4 colDiffuse;\n"
        "void main() {\n"
        "    vec3 L = normalize(vec3(0.4, 1.0, 0.3));\n"
        "    float diff = max(dot(normalize(fragNormal), L), 0.0);\n"
        "    float intensity = 0.35 + 0.65 * diff;\n"
        "    gl_FragColor = vec4(colDiffuse.rgb * intensity, colDiffuse.a);\n"
        "}\n";

    Shader lit = LoadShaderFromMemory(vsCode, fsCode);
    for (int i = 0; i < world.materialCount; i++) {
        world.materials[i].shader = lit;
    }

    while (!WindowShouldClose()) {
        sys_update();
        move();

        BeginDrawing();
            ClearBackground(COL_BG);
            BeginMode3D(state.camera);
                DrawModel(world, (Vector3){ 0.0f, 0.0f, 0.0f }, MODEL_SCALE, WHITE);
            EndMode3D();
            draw_overlay();
        EndDrawing();
    }

    UnloadShader(lit);
    UnloadModel(world);
    CloseWindow();
    return 0;
}
