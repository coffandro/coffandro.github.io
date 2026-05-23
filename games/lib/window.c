#include "window.h"
#include "colors.h"
#include <emscripten.h>

int screenWidth = default_width;
int screenHeight = default_height;

Vector2 mousePos;
bool mouseLocked = false;
bool isTouchMode = false;

void set_touch_mode(int v) {
    isTouchMode = v ? true : false;
}

void on_dimension_update(int w, int h);

void set_dimensions(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    on_dimension_update(w, h);
}

void cursor_lock() {
    DisableCursor();
    mouseLocked = true;
}

void cursor_unlock() {
    EnableCursor();
    mouseLocked = false;
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

// --- Touch joysticks / movement + look input -------------------------------

#define JOY_RADIUS      60.0f
#define JOY_DEADZONE     8.0f
#define JOY_MARGIN     75.0f
#define JOY_GRAB_SLACK   1.5f   // bigger touch-grab area than the visible ring

typedef struct {
    bool    active;
    int     touch_id;
    Vector2 origin;   // screen-space anchor
    Vector2 stick;    // current touch position
} Joystick;

static Joystick move_joy = { false, -1, {0}, {0} };
static Joystick look_joy = { false, -1, {0}, {0} };

static Vector2 move_anchor(void) {
    return (Vector2){ JOY_MARGIN, screenHeight - JOY_MARGIN };
}

static Vector2 look_anchor(void) {
    return (Vector2){ screenWidth - JOY_MARGIN, screenHeight - JOY_MARGIN };
}

static Vector2 read_keyboard_movement(void) {
    Vector2 m = { 0, 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    m.y += 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  m.y -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) m.x += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  m.x -= 1.0f;
    float len = sqrtf(m.x * m.x + m.y * m.y);
    if (len > 0.0f) { m.x /= len; m.y /= len; }
    return m;
}

// Returns normalized (dx, dy) in [-1, 1]. dy convention: +1 = "up" / forward.
// claimed_ids[] are touch IDs already owned by another joystick this frame.
static Vector2 update_joystick(Joystick *joy, Vector2 anchor, int *claimed_ids, int claimed_count) {
    int n = GetTouchPointCount();
    if (n == 0) {
        joy->active = false;
        joy->touch_id = -1;
        return (Vector2){0, 0};
    }

    if (joy->active && joy->touch_id >= 0) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (GetTouchPointId(i) == joy->touch_id) {
                joy->stick = GetTouchPosition(i);
                found = true;
                break;
            }
        }
        if (!found) {
            joy->active = false;
            joy->touch_id = -1;
            return (Vector2){0, 0};
        }
    } else {
        for (int i = 0; i < n; i++) {
            int id = GetTouchPointId(i);
            bool taken = false;
            for (int c = 0; c < claimed_count; c++) {
                if (claimed_ids[c] == id) { taken = true; break; }
            }
            if (taken) continue;

            Vector2 p = GetTouchPosition(i);
            float dx = p.x - anchor.x;
            float dy = p.y - anchor.y;
            if (sqrtf(dx * dx + dy * dy) < JOY_RADIUS * JOY_GRAB_SLACK) {
                joy->active = true;
                joy->touch_id = id;
                joy->origin = anchor;
                joy->stick = p;
                break;
            }
        }
        if (!joy->active) return (Vector2){0, 0};
    }

    float dx = joy->stick.x - joy->origin.x;
    float dy = joy->origin.y - joy->stick.y;   // flip: screen-down -> negative dy
    float len = sqrtf(dx * dx + dy * dy);
    if (len < JOY_DEADZONE) return (Vector2){0, 0};
    if (len > JOY_RADIUS) { dx *= JOY_RADIUS / len; dy *= JOY_RADIUS / len; }
    return (Vector2){ dx / JOY_RADIUS, dy / JOY_RADIUS };
}

void sys_update() {
    SetWindowSize(screenWidth, screenHeight);
    mousePos = GetMousePosition();

    int claimed[2];
    int claimed_count = 0;
    if (move_joy.active) claimed[claimed_count++] = move_joy.touch_id;

    Vector2 stick_move = update_joystick(&move_joy, move_anchor(), claimed, claimed_count);

    claimed_count = 0;
    if (move_joy.active) claimed[claimed_count++] = move_joy.touch_id;
    if (look_joy.active) claimed[claimed_count++] = look_joy.touch_id;

    Vector2 stick_look = update_joystick(&look_joy, look_anchor(), claimed, claimed_count);

    Vector2 kbd = read_keyboard_movement();
    Vector2 m = (kbd.x != 0.0f || kbd.y != 0.0f) ? kbd : stick_move;
    on_movement(m.x, m.y);
    on_look(stick_look.x, stick_look.y);
}

static void draw_joystick(Joystick *joy, Vector2 anchor) {
    
    DrawCircle((int)anchor.x, (int)anchor.y, JOY_RADIUS, COL_BG);
    DrawCircleLines((int)anchor.x, (int)anchor.y, JOY_RADIUS, COL_TEXT);

    Vector2 stickPos = anchor;
    if (joy->active) {
        float dx = joy->stick.x - joy->origin.x;
        float dy = joy->stick.y - joy->origin.y;
        float len = sqrtf(dx * dx + dy * dy);
        if (len > JOY_RADIUS) { dx *= JOY_RADIUS / len; dy *= JOY_RADIUS / len; }
        stickPos.x = joy->origin.x + dx;
        stickPos.y = joy->origin.y + dy;
    }
    DrawCircle((int)stickPos.x, (int)stickPos.y, 22, COL_TEXT);
}

void draw_overlay() {
    if (!isTouchMode) return;
    draw_joystick(&move_joy, move_anchor());
    draw_joystick(&look_joy, look_anchor());
}

void emit_game_event(int type, int data) {
    EM_ASM({
        Module.canvas.dispatchEvent(new CustomEvent('game-event', {
            detail: { type: $0, data: $1 },
            bubbles: true
        }));
    }, type, data);
}