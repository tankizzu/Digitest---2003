/* Touch controls (Android): a stick that appears where a finger lands and goes
   when it lifts, plus the PlayStation buttons as round keys at the right.
   The buttons are a stand-in for the first test builds: the touch interface
   proper (no buttons, context bubbles) comes later. pad.c asks touch_poll()
   each vsync; gl/touch.c draws what touch_shapes() lists. */
#include <SDL3/SDL.h>
#include <math.h>
#include "touch.h"

/* PlayStation button bits, as pad.c's NAMES[] */
enum { B_SELECT = 0, B_START = 3, B_UP = 4, B_RIGHT = 5, B_DOWN = 6, B_LEFT = 7, B_L2 = 8, B_R2 = 9, B_L1 = 10, B_R1 = 11,
       B_TRIANGLE = 12, B_CIRCLE = 13, B_CROSS = 14, B_SQUARE = 15 };

typedef struct Key {
    float x, y, r; /* pixels */
    int bit, kind;
    int down;
} Key;

static Key s_keys[8];
static int s_nkeys;
static int s_enabled = -1;
static int s_w, s_h;
static SDL_FingerID s_stick_id;
static int s_stick_on;
static float s_bx, s_by, s_fx, s_fy, s_rad; /* the stick's base, the finger, its reach */
static float s_vx, s_vy;                    /* -1..1 */
static Uint64 s_last_touch;

int touch_enabled(void) {
    if (s_enabled < 0) {
        const char *env = SDL_getenv("DW3_TOUCH");
#ifdef __ANDROID__
        s_enabled = env == NULL || SDL_atoi(env) != 0;
#else
        s_enabled = env != NULL && SDL_atoi(env) != 0;
#endif
    }
    return s_enabled;
}

static void layout(int w, int h) {
    float u = (float)SDL_min(w, h);
    float r = u * 0.075f, gap = r * 1.25f;
    float cx = w - r * 3.2f, cy = h - r * 3.4f;
    float sr = r * 0.55f;

    s_w = w, s_h = h, s_rad = u * 0.11f;
    s_nkeys = 0;
    s_keys[s_nkeys++] = (Key){cx, cy - gap, r, B_TRIANGLE, TOUCH_TRIANGLE, 0};
    s_keys[s_nkeys++] = (Key){cx + gap, cy, r, B_CIRCLE, TOUCH_CIRCLE, 0};
    s_keys[s_nkeys++] = (Key){cx, cy + gap, r, B_CROSS, TOUCH_CROSS, 0};
    s_keys[s_nkeys++] = (Key){cx - gap, cy, r, B_SQUARE, TOUCH_SQUARE, 0};
    s_keys[s_nkeys++] = (Key){w * 0.5f + sr * 1.6f, h - sr * 1.6f, sr, B_START, TOUCH_START, 0};
    s_keys[s_nkeys++] = (Key){w * 0.5f - sr * 1.6f, h - sr * 1.6f, sr, B_SELECT, TOUCH_SELECT, 0};
    s_keys[s_nkeys++] = (Key){sr * 1.8f, sr * 1.8f + h * 0.02f, sr, B_L1, TOUCH_L1, 0};
    s_keys[s_nkeys++] = (Key){w - sr * 1.8f, sr * 1.8f + h * 0.02f, sr, B_R1, TOUCH_R1, 0};
}

static Key *key_at(float x, float y) {
    for (int i = 0; i < s_nkeys; i++) {
        float dx = x - s_keys[i].x, dy = y - s_keys[i].y, r = s_keys[i].r * 1.25f;
        if (dx * dx + dy * dy <= r * r) {
            return &s_keys[i];
        }
    }
    return NULL;
}

void touch_poll(int w, int h, uint16_t *held, uint8_t *lstick) {
    int ndev = 0, seen_stick = 0;
    SDL_TouchID *devs;

    if (!touch_enabled() || w <= 0 || h <= 0) {
        return;
    }
    if (w != s_w || h != s_h) {
        layout(w, h);
    }
    for (int i = 0; i < s_nkeys; i++) {
        s_keys[i].down = 0;
    }
    devs = SDL_GetTouchDevices(&ndev);
    for (int d = 0; devs != NULL && d < ndev; d++) {
        int n = 0;
        SDL_Finger **fingers = SDL_GetTouchFingers(devs[d], &n);

        for (int i = 0; fingers != NULL && i < n; i++) {
            float x = fingers[i]->x * w, y = fingers[i]->y * h;
            Key *k;

            s_last_touch = SDL_GetTicks();
            if (s_stick_on && fingers[i]->id == s_stick_id) {
                seen_stick = 1;
                s_fx = x, s_fy = y;
                continue;
            }
            if ((k = key_at(x, y)) != NULL) {
                k->down = 1;
            } else if (!s_stick_on) { /* a new finger off the keys: the stick appears under it */
                s_stick_on = seen_stick = 1;
                s_stick_id = fingers[i]->id;
                s_bx = s_fx = x, s_by = s_fy = y;
            }
        }
        SDL_free(fingers);
    }
    SDL_free(devs);
    if (!seen_stick) {
        s_stick_on = 0;
    }
    s_vx = s_vy = 0;
    if (s_stick_on) {
        float dx = s_fx - s_bx, dy = s_fy - s_by, len = sqrtf(dx * dx + dy * dy);

        if (len > s_rad * 1.4f) { /* the base follows a finger that runs past it */
            float k = (len - s_rad * 1.4f) / len;
            s_bx += dx * k, s_by += dy * k;
            dx = s_fx - s_bx, dy = s_fy - s_by, len = sqrtf(dx * dx + dy * dy);
        }
        if (len > s_rad) {
            dx *= s_rad / len, dy *= s_rad / len, len = s_rad;
        }
        s_vx = dx / s_rad, s_vy = dy / s_rad;
        if (len / s_rad > 0.12f) {
            int c;
            float a = atan2f(s_vy, s_vx); /* 8 sectors of 45 degrees, 0 = right */

            lstick[0] = (uint8_t)SDL_clamp(128 + s_vx * 127, 0, 255);
            lstick[1] = (uint8_t)SDL_clamp(128 + s_vy * 127, 0, 255);
            if (len / s_rad > 0.35f) {
                c = ((int)floorf((a + (float)M_PI / 8) / ((float)M_PI / 4)) + 8) % 8;
                if (c == 7 || c == 0 || c == 1) *held |= 1 << B_RIGHT;
                if (c == 1 || c == 2 || c == 3) *held |= 1 << B_DOWN;
                if (c == 3 || c == 4 || c == 5) *held |= 1 << B_LEFT;
                if (c == 5 || c == 6 || c == 7) *held |= 1 << B_UP;
            }
        }
    }
    for (int i = 0; i < s_nkeys; i++) {
        if (s_keys[i].down) {
            *held |= 1 << s_keys[i].bit;
        }
    }
}

int touch_shapes(TouchShape *out, int max) {
    int n = 0;
    /* the keys fade out after a while without a touch (a gamepad in use) */
    float fade = s_last_touch == 0 ? 1.0f : SDL_clamp(1.0f - (float)(SDL_GetTicks() - s_last_touch - 8000) / 1000.0f, 0.0f, 1.0f);

    if (!touch_enabled() || s_w == 0) {
        return 0;
    }
    for (int i = 0; i < s_nkeys && n < max; i++) {
        float a = (s_keys[i].down ? 0.55f : 0.32f) * fade;
        out[n++] = (TouchShape){s_keys[i].x, s_keys[i].y, s_keys[i].r, s_keys[i].kind, a};
    }
    if (s_stick_on && n + 2 <= max) {
        out[n++] = (TouchShape){s_bx, s_by, s_rad * 1.25f, TOUCH_RING, 0.45f};
        out[n++] = (TouchShape){s_bx + s_vx * s_rad, s_by + s_vy * s_rad, s_rad * 0.5f, TOUCH_KNOB, 0.75f};
    }
    return n;
}
