/* Touch controls (Android): a stick that appears where a finger lands and goes
   when it lifts, plus the PlayStation buttons as round keys at the right.
   The buttons are a stand-in for the first test builds: the touch interface
   proper (no buttons, context bubbles) comes later. pad.c asks touch_poll()
   each vsync; gl/touch.c draws what touch_shapes() lists. */
#include <SDL3/SDL.h>
#include <math.h>
#include "touch.h"

int movie_playing(void); /* movie.c */
int getMode(void);       /* the game's mode (engine/game/state.c) */

/* A finger held on a movie this long skips it (Start, as the game takes) */
#define SKIP_HOLD_MS 2000

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
static Uint64 s_menu_until, s_back_until; /* the menu and back buttons show until then */
static int s_press_start, s_press_triangle;  /* vsyncs a button is still held for a tap or the system's back */
static int s_menu_down, s_back_down;
static int s_text_tap, s_text_held;
static int s_text_tap_frame = -1; /* the vsync the tap was first read at: it holds for that whole game frame */
int VSync(int mode); /* native_touch_text_*: a finger landed (counts down, latched), one down */
static SDL_FingerID s_skip_id;
static int s_skip_on, s_skip_press;
static Uint64 s_skip_from;
static float s_skip_x, s_skip_y;

/* The screens' targets (native_touch_target), in the PSX display's pixels; each lasts a moment unless marked again */
typedef struct Target {
    int id, x, y, w, h;
    Uint64 until;
} Target;
static Target s_targets[32];
static int s_ntargets;
static int s_tap = -1;
static float s_pic[4] = {0, 0, 1, 1}, s_disp[2] = {320, 240};
static SDL_FingerID s_down[10];
static int s_ndown;

void touch_set_picture(int x, int y, int w, int h, int disp_w, int disp_h) {
    s_pic[0] = (float)x, s_pic[1] = (float)y, s_pic[2] = (float)SDL_max(w, 1), s_pic[3] = (float)SDL_max(h, 1);
    s_disp[0] = (float)SDL_max(disp_w, 1), s_disp[1] = (float)SDL_max(disp_h, 1);
}

void native_touch_target(int id, int x, int y, int w, int h) {
    Uint64 now = SDL_GetTicks();
    int i;

    for (i = 0; i < s_ntargets && s_targets[i].id != id; i++) {
    }
    if (i == s_ntargets) {
        for (i = 0; i < s_ntargets && s_targets[i].until >= now; i++) { /* an expired slot, or a new one */
        }
        if (i == s_ntargets) {
            if (s_ntargets == (int)SDL_arraysize(s_targets)) {
                return;
            }
            s_ntargets++;
        }
    }
    s_targets[i] = (Target){id, x, y, w, h, now + 120};
}

int native_touch_tap(void) {
    int t = s_tap;
    s_tap = -1;
    return t == 0x7A11 ? -1 : t; /* a tap meant for native_touch_anywhere is no option's */
}

static int s_logical[16]; /* native_touch_hold: game frames each logical button is still held for */

void native_touch_hold(int button, int frames) {
    if (button >= 0 && button < 16) {
        s_logical[button] = frames;
    }
}

/* Held for its last two frames only: the frames before let a list take the line picked first (the battle's
   technique and item lists ignore X the frame their cursor moves) */
int native_touch_buttons(void) {
    int mask = 0;
    for (int i = 0; i < 16; i++) {
        if (s_logical[i] > 0) {
            if (s_logical[i] <= 2) {
                mask |= 1 << i;
            }
            s_logical[i]--;
        }
    }
    return mask;
}

#define ANYWHERE_ID 0x7A11
int native_touch_anywhere(void) {
    native_touch_target(ANYWHERE_ID, -10000, -10000, 20000, 20000); /* the whole display and the bars around it */
    if (s_tap == ANYWHERE_ID) {
        s_tap = -1;
        return 1;
    }
    return 0;
}

/* The target under a point of the PSX display, or -1 */
static int target_at(float px, float py) {
    Uint64 now = SDL_GetTicks();
    for (int i = 0; i < s_ntargets; i++) {
        const Target *t = &s_targets[i];
        if (t->until >= now && px >= t->x && px < t->x + t->w && py >= t->y && py < t->y + t->h) {
            return t->id;
        }
    }
    return -1;
}

void touch_script_tap(int px, int py) {
    int id = target_at((float)px, (float)py);
    if (id >= 0) {
        s_tap = id;
    } else {
        s_text_tap = 5, s_text_tap_frame = -1; /* off the options: a tap on the text, as touch_poll counts one */
    }
}

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
    s_keys[s_nkeys++] = (Key){sr * 1.8f, h * 0.36f, sr, B_L1, TOUCH_L1, 0};
    s_keys[s_nkeys++] = (Key){w - sr * 1.8f, h * 0.36f, sr, B_R1, TOUCH_R1, 0};
}

void native_touch_menu_available(void) {
    s_menu_until = SDL_GetTicks() + 150;
}

void native_touch_in_menu(void) {
    s_back_until = SDL_GetTicks() + 150;
}

/* The screens that are menus by themselves: the field menu's screens, shops, the lab, the card album and shop, the
   deck editor, the training and naming screens (engine/game_state.h's modes) */
static int menu_mode(void) {
    int m = getMode() >> 8;
    return m == 0x10 || m == 0x0F || m == 0x0D || m == 0x12 || m == 0x13 || m == 0x04 || m == 0x0A || m == 0x0B;
}

static int menu_shown(void) {
    return SDL_GetTicks() < s_menu_until;
}

static int back_shown(void) {
    return SDL_GetTicks() < s_back_until || menu_mode();
}

/* The corner buttons, in pixels: the menu at the top right, back at the top left */
static void corner(int right, float *x, float *y, float *r) {
    float u = (float)SDL_min(s_w, s_h);
    *r = u * 0.065f;
    *x = right ? s_w - *r * 1.6f : *r * 1.6f;
    *y = *r * 1.6f;
}

static int in_corner(int right, float px, float py) {
    float x, y, r;
    corner(right, &x, &y, &r);
    return (px - x) * (px - x) + (py - y) * (py - y) <= r * r * 1.7f;
}

/* Fingers that tapped an option: they stay off the stick and the keys until they lift */
static SDL_FingerID s_on_target[10];
static int s_non_target;
static void hold_target(SDL_FingerID id) {
    if (s_non_target < (int)SDL_arraysize(s_on_target)) {
        s_on_target[s_non_target++] = id;
    }
}
static int held_target(SDL_FingerID id) {
    for (int i = 0; i < s_non_target; i++) {
        if (s_on_target[i] == id) {
            return 1;
        }
    }
    return 0;
}
static void forget_targets(const SDL_FingerID *down, int n) {
    int k = 0;
    for (int i = 0; i < s_non_target; i++) {
        int still = 0;
        for (int j = 0; j < n; j++) {
            still |= down[j] == s_on_target[i];
        }
        if (still) {
            s_on_target[k++] = s_on_target[i];
        }
    }
    s_non_target = k;
}

static int was_down(SDL_FingerID id) {
    for (int j = 0; j < s_ndown; j++) {
        if (s_down[j] == id) {
            return 1;
        }
    }
    return 0;
}


int native_touch_text_tap(void) {
    int now = VSync(-1);

    if (s_text_tap > 0 && s_text_tap_frame < 0) {
        s_text_tap_frame = now;
    }
    if (s_text_tap_frame >= 0) {
        if (now == s_text_tap_frame) {
            return 1;
        }
        s_text_tap = 0;
        s_text_tap_frame = -1;
    }
    return 0;
}

int native_touch_text_held(void) {
    return s_text_held;
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
    int ndev = 0, seen_stick = 0, seen_skip = 0, text_held = 0;
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
    SDL_FingerID now_down[10];
    int now_n = 0;

    s_menu_down = s_back_down = 0;
    {
        /* the system's back (gesture or key, SDL_ANDROID_TRAP_BACK_BUTTON) is Triangle, the game's cancel */
        static int back_was;
        const bool *keys = SDL_GetKeyboardState(NULL);
        int back = keys[SDL_SCANCODE_AC_BACK];
        if (back && !back_was) {
            s_press_triangle = 6;
        }
        back_was = back;
    }

    devs = SDL_GetTouchDevices(&ndev);
    for (int d = 0; devs != NULL && d < ndev; d++) {
        int n = 0;
        SDL_Finger **fingers = SDL_GetTouchFingers(devs[d], &n);

        for (int i = 0; fingers != NULL && i < n; i++) {
            float x = fingers[i]->x * w, y = fingers[i]->y * h;
            Key *k;

            s_last_touch = SDL_GetTicks();
            {
                int was = 0, id;
                for (int j = 0; j < s_ndown; j++) {
                    was |= s_down[j] == fingers[i]->id;
                }
                if (now_n < (int)SDL_arraysize(now_down)) {
                    now_down[now_n++] = fingers[i]->id;
                }
                if (was && held_target(fingers[i]->id)) {
                    continue; /* still on the option it tapped */
                }
                if (menu_shown() && in_corner(1, x, y)) {
                    if (!was) {
                        s_press_start = 6;
                    }
                    s_menu_down = 1;
                    continue;
                }
                if (back_shown() && in_corner(0, x, y)) {
                    if (!was) {
                        s_press_triangle = 6;
                    }
                    s_back_down = 1;
                    continue;
                }
                if (!was && (id = target_at((x - s_pic[0]) / s_pic[2] * s_disp[0], (y - s_pic[1]) / s_pic[3] * s_disp[1])) >= 0) {
                    s_tap = id;
                    hold_target(fingers[i]->id);
                    continue;
                }
            }
            if (s_stick_on && fingers[i]->id == s_stick_id) {
                seen_stick = 1;
                text_held = 1;
                s_fx = x, s_fy = y;
                continue;
            }
            if ((k = key_at(x, y)) != NULL) {
                k->down = 1;
                continue;
            }
            text_held = 1;
            if (!was_down(fingers[i]->id)) {
                s_text_tap = 5, s_text_tap_frame = -1; /* a few vsyncs: the game reads the pad at 30 frames a second */
            }
            if (0) {
            } else if (movie_playing()) { /* held on a movie: skips it after SKIP_HOLD_MS */
                if (!s_skip_on || s_skip_id != fingers[i]->id) {
                    s_skip_on = 1, s_skip_id = fingers[i]->id, s_skip_from = SDL_GetTicks();
                }
                seen_skip = 1;
                s_skip_x = x, s_skip_y = y;
            } else if (!s_stick_on) { /* a new finger off the keys: the stick appears under it */
                s_stick_on = seen_stick = 1;
                s_stick_id = fingers[i]->id;
                s_bx = s_fx = x, s_by = s_fy = y;
            }
        }
        SDL_free(fingers);
    }
    SDL_free(devs);
    s_text_held = text_held;
    if (s_text_tap > 0 && s_text_tap_frame < 0) { /* not read in time: dropped */
        s_text_tap--;
    }
    SDL_memcpy(s_down, now_down, sizeof(SDL_FingerID) * (size_t)now_n);
    s_ndown = now_n;
    forget_targets(now_down, now_n);
    if (!seen_stick) {
        s_stick_on = 0;
    }
    if (!seen_skip) {
        s_skip_on = 0;
    } else if (s_skip_on == 1 && SDL_GetTicks() - s_skip_from >= SKIP_HOLD_MS) {
        s_skip_on = 2; /* done: Start for a few vsyncs, once per hold */
        s_skip_press = 8;
    }
    if (s_skip_press > 0) {
        s_skip_press--;
        *held |= 1 << B_START;
    }
    if (s_press_start > 0) {
        s_press_start--;
        *held |= 1 << B_START;
    }
    if (s_press_triangle > 0) {
        s_press_triangle--;
        *held |= 1 << B_TRIANGLE;
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
        out[n++] = (TouchShape){s_keys[i].x, s_keys[i].y, s_keys[i].r, s_keys[i].kind, a, 0};
    }
    if (s_stick_on && n + 2 <= max) {
        out[n++] = (TouchShape){s_bx, s_by, s_rad * 1.25f, TOUCH_RING, 0.45f, 0};
        out[n++] = (TouchShape){s_bx + s_vx * s_rad, s_by + s_vy * s_rad, s_rad * 0.5f, TOUCH_KNOB, 0.75f, 0};
    }
    if (menu_shown() && n < max) {
        float x, y, r;
        corner(1, &x, &y, &r);
        out[n++] = (TouchShape){x, y, r, TOUCH_MENU, s_menu_down ? 0.95f : 0.75f, 0};
    }
    if (back_shown() && n < max) {
        float x, y, r;
        corner(0, &x, &y, &r);
        out[n++] = (TouchShape){x, y, r, TOUCH_BACK, s_back_down ? 0.95f : 0.75f, 0};
    }
    if (s_skip_on == 1 && n < max) {
        float f = SDL_min(1.0f, (float)(SDL_GetTicks() - s_skip_from) / SKIP_HOLD_MS);
        out[n++] = (TouchShape){s_skip_x, s_skip_y, s_rad * 0.9f, TOUCH_PROGRESS, 0.9f, f};
    }
    return n;
}
