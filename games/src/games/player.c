#include "player.h"
#include <raymath.h>
#include "girl_math.h"

Vector2 pos;
Texture2D texture;
float rotation = 0;

void init_player() {
    UnloadTexture(texture);
    texture = LoadTexture("/tmp/assets/ship.png");
}

void update_player() {
    Vector2 mousePos = GetMousePosition();

    Vector2 to_mouse = Vector2Subtract(mousePos, pos);
    float dist = Vector2Length(to_mouse);

    if (dist < 0.001f)
        return;

    Vector2 move_dir;

    // -------------------------------
    // 1. Compute smooth blend factor
    // -------------------------------
    // 0 = move directly, 1 = orbit
    float raw_blend = (dist - ORBIT_START) / (ORBIT_END - ORBIT_START);
    raw_blend = Clamp(raw_blend, 0.0f, 1.0f);

    // Apply easing for smoother transition (cubic ease-in/out)
    float blend = raw_blend * raw_blend * (3.0f - 2.0f * raw_blend);

    // -------------------------------
    // 2. Compute orbiting forces
    // -------------------------------
    Vector2 radial_dir = Vector2Scale(to_mouse, 1.0f / dist);
    Vector2 tangent_dir = (Vector2){ radial_dir.y, -radial_dir.x };

    float radial_error = (dist - ORBIT_RADIUS) / ORBIT_RADIUS;
    radial_error = Clamp(radial_error, -1.0f, 1.0f);

    Vector2 radial_force = Vector2Scale(radial_dir, -radial_error * RADIAL_WEIGHT);
    Vector2 tangent_force = Vector2Scale(tangent_dir, TANGENT_WEIGHT);

    Vector2 orbit_force = Vector2Normalize(Vector2Multiply(Vector2Add(radial_force, tangent_force), (Vector2){-1, -1}));

    // -------------------------------
    // 3. Compute direct movement
    // -------------------------------
    Vector2 direct_force = Vector2Normalize(to_mouse);

    // -------------------------------
    // 4. Blend the two
    // -------------------------------
    move_dir = Vector2Normalize(Vector2Add(
        Vector2Scale(direct_force, 1.0f - blend),
        Vector2Scale(orbit_force, blend)
    ));

    // -------------------------------
    // 5. Handle rotation
    // -------------------------------
    float target_rotation = atan2f(move_dir.y, move_dir.x) * RAD2DEG + 90.0f;
    float delta_angle = target_rotation - rotation;

    while (delta_angle > 180.0f) delta_angle -= 360.0f;
    while (delta_angle < -180.0f) delta_angle += 360.0f;

    float max_rotation = ROTATION_SPEED * GetFrameTime();
    if (delta_angle > max_rotation) delta_angle = max_rotation;
    if (delta_angle < -max_rotation) delta_angle = -max_rotation;

    rotation += delta_angle;

    // -------------------------------
    // 6. Move player
    // -------------------------------
    pos = Vector2Add(pos, Vector2Scale(move_dir, SPEED * GetFrameTime()));

    // -------------------------------
    // 7. Screen wrap
    // -------------------------------
    Vector2 screen = (Vector2){ GetScreenWidth(), GetScreenHeight() };
    if (pos.x < 0) pos.x = screen.x;
    else if (pos.x > screen.x) pos.x = 0;
    if (pos.y < 0) pos.y = screen.y;
    else if (pos.y > screen.y) pos.y = 0;
}

void draw_player() {
    DrawTextureEx(
        texture, 
        Vector2Subtract(pos,
            (Vector2){
                (texture.width * DRAW_SCALE)/2,
                (texture.height * DRAW_SCALE)/2
            }
        ),
        rotation,
        DRAW_SCALE,
        WHITE
    );
}
