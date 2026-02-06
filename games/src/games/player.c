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

    // Only orbit if the player is far away
    if (dist < ORBIT_THRESHOLD) {
        Vector2 radial_dir = Vector2Scale(to_mouse, 1.0f / dist);
        Vector2 tangent_dir = (Vector2){ radial_dir.y, -radial_dir.x };

        float radial_error = (dist - ORBIT_RADIUS) / ORBIT_RADIUS;
        radial_error = Clamp(radial_error, -1.0f, 1.0f);

        Vector2 radial_force = Vector2Scale(radial_dir, -radial_error * RADIAL_WEIGHT);
        Vector2 tangent_force = Vector2Scale(tangent_dir, TANGENT_WEIGHT);

        Vector2 steering = Vector2Multiply(Vector2Add(tangent_force, radial_force), (Vector2){-1,-1});
        move_dir = Vector2Normalize(steering);
    } else {
        // Move directly toward the target if close
        move_dir = Vector2Normalize(to_mouse);
    }

    // Rotation
    float target_rotation = atan2f(move_dir.y, move_dir.x) * RAD2DEG + 90.0f;
    float delta_angle = target_rotation - rotation;

    while (delta_angle > 180.0f) delta_angle -= 360.0f;
    while (delta_angle < -180.0f) delta_angle += 360.0f;

    float max_rotation = ROTATION_SPEED * GetFrameTime();
    if (delta_angle > max_rotation) delta_angle = max_rotation;
    if (delta_angle < -max_rotation) delta_angle = -max_rotation;

    rotation += delta_angle;

    // Movement
    pos = Vector2Add(pos, Vector2Scale(move_dir, SPEED * GetFrameTime()));

    // Screen wrap
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
