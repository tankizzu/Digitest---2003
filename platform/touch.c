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
static Uint64 s_stick_from;
static int s_stick_moved;
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
    if (t >= 0x7A00) {
        return -1; /* the port's own targets (0x7A00 on) are no option's: left for native_touch_tap_id */
    }
    s_tap = -1;
    return t;
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

/* A tap on the port's own target `id` (0x7A00 on), once */
int native_touch_tap_id(int id) {
    if (s_tap == id) {
        s_tap = -1;
        return 1;
    }
    return 0;
}

int native_auto_text(void); /* prefs.c */
static const int AUTO_TEXT_WAIT[4] = {0, 180, 90, 12}; /* vsyncs a page waits at each speed: 3 s, 1.5 s, 0.2 s */

/* Automatic dialogues: 1 once the text window `who` has waited AUTO_TEXT_WAIT for the player (text_window.c asks
   each frame it waits; a break of a few vsyncs starts the count again) */
int native_auto_text_due(void *who) {
    /* each window that waits keeps its own count: two balloons open at once ask in turn every frame */
    static struct {
        void *who;
        int since, last;
    } waits[4];
    int now = VSync(-1), i, free = -1;

    for (i = 0; i < 4 && waits[i].who != who; i++) {
        if (free < 0 && (waits[i].who == NULL || now - waits[i].last > 6)) {
            free = i;
        }
    }
    if (i == 4) { /* a new one (or one that stopped waiting a while ago) */
        i = free >= 0 ? free : 0;
        waits[i].who = who, waits[i].since = now;
    } else if (now - waits[i].last > 6) {
        waits[i].since = now;
    }
    waits[i].last = now;
    if (native_auto_text() && now - waits[i].since >= AUTO_TEXT_WAIT[native_auto_text()]) {
        waits[i].who = NULL;
        return 1;
    }
    return 0;
}

/* The phone's keyboard (the name entry): SDL's text input, what is typed queued as code points; 8 is backspace,
   13 enter. Shown while native_text_begin() is called each frame, hidden a moment after it stops */
static int s_typed[64], s_typed_n;
static Uint64 s_text_until;
static int s_text_on;

static void typed(int c) {
    if (s_typed_n < (int)SDL_arraysize(s_typed)) {
        s_typed[s_typed_n++] = c;
    }
}

static bool SDLCALL on_text(void *data, SDL_Event *e) {
    (void)data;
    if (!s_text_on) {
        return true;
    }
    if (e->type == SDL_EVENT_TEXT_INPUT) {
        const char *t = e->text.text;
        Uint32 c;
        while ((c = SDL_StepUTF8(&t, NULL)) != 0) {
            typed((int)c);
        }
    } else if (e->type == SDL_EVENT_KEY_DOWN && (e->key.key == SDLK_BACKSPACE || e->key.key == SDLK_RETURN)) {
        typed(e->key.key == SDLK_BACKSPACE ? 8 : 13);
    }
    return true;
}

int native_soft_keyboard(void) {
    return touch_enabled() || SDL_getenv("DW3_TYPE") != NULL;
}

void native_text_begin(void) {
    static int watching;
    if (!watching) {
        const char *test = SDL_getenv("DW3_TYPE"); /* tests: what to type, \n for enter, < for backspace */
        SDL_AddEventWatch(on_text, NULL);
        watching = 1;
        for (; test != NULL && *test != 0; test++) {
            typed(*test == '\n' || *test == '|' ? 13 : *test == '<' ? 8 : (unsigned char)*test);
        }
    }
    s_text_until = SDL_GetTicks() + 300;
    if (!s_text_on) {
        int count = 0;
        SDL_Window **wins = SDL_GetWindows(&count);
        if (wins != NULL && count > 0) {
            SDL_StartTextInput(wins[0]);
        }
        SDL_free(wins);
        s_text_on = 1;
    }
}

/* The keyboard shown again (a tap on the name: the player may have put it away) */
void native_text_reopen(void) {
    int count = 0;
    SDL_Window **wins = SDL_GetWindows(&count);
    if (wins != NULL && count > 0) {
        SDL_StopTextInput(wins[0]);
        SDL_StartTextInput(wins[0]);
    }
    SDL_free(wins);
}

int native_text_next(void) {
    int c;
    if (s_typed_n == 0) {
        return -1;
    }
    c = s_typed[0];
    SDL_memmove(s_typed, s_typed + 1, sizeof(int) * (size_t)--s_typed_n);
    return c;
}

/* the keyboard put away once the name entry stops asking for it (touch_poll, each vsync) */
static void text_input_idle(void) {
    if (s_text_on && SDL_GetTicks() > s_text_until) {
        int count = 0;
        SDL_Window **wins = SDL_GetWindows(&count);
        if (wins != NULL && count > 0) {
            SDL_StopTextInput(wins[0]);
        }
        SDL_free(wins);
        s_text_on = 0;
        s_typed_n = 0;
    }
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
/* Where the screen's 320x240 sits on a taller upright display (portrait.c): the targets are in its coordinates */
static int s_origin_y;

void native_touch_origin(int y) {
    s_origin_y = y;
}

static int target_at(float px, float py) {
    Uint64 now = SDL_GetTicks();

    if (s_disp[1] > 256) {
        py -= (float)s_origin_y;
    }
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
    if (SDL_getenv("DW3_TOUCH_KEYS") == NULL) {
        return; /* the game is played by touch alone: the PlayStation's buttons only for tests */
    }
    s_keys[s_nkeys++] = (Key){cx, cy - gap, r, B_TRIANGLE, TOUCH_TRIANGLE, 0};
    s_keys[s_nkeys++] = (Key){cx + gap, cy, r, B_CIRCLE, TOUCH_CIRCLE, 0};
    s_keys[s_nkeys++] = (Key){cx, cy + gap, r, B_CROSS, TOUCH_CROSS, 0};
    s_keys[s_nkeys++] = (Key){cx - gap, cy, r, B_SQUARE, TOUCH_SQUARE, 0};
    s_keys[s_nkeys++] = (Key){w * 0.5f + sr * 1.6f, h - sr * 1.6f, sr, B_START, TOUCH_START, 0};
    s_keys[s_nkeys++] = (Key){w * 0.5f - sr * 1.6f, h - sr * 1.6f, sr, B_SELECT, TOUCH_SELECT, 0};
    s_keys[s_nkeys++] = (Key){sr * 1.8f, h * 0.36f, sr, B_L1, TOUCH_L1, 0};
    s_keys[s_nkeys++] = (Key){w - sr * 1.8f, h * 0.36f, sr, B_R1, TOUCH_R1, 0};
}

static Uint64 s_dialog_until, s_no_stick_until, s_free_until, s_saved_at;
static int s_tap_cross;
static int s_auto_down;
void native_auto_text_toggle(void); /* prefs.c */

/* A dialogue is open: the automatic dialogues button shows, at the screen's bottom left (talk_box.c, message_box.c) */
void native_touch_dialog_open(void) {
    s_dialog_until = SDL_GetTicks() + 150;
}

/* The field leaves the player in control this frame (field.c): the stick is for walking, and a short tap is X (talk,
   look, open) */
void native_touch_field_free(void) {
    s_free_until = SDL_GetTicks() + 150;
}

/* How far the stick is pushed, for the field (actor_control.c, trigger.c): 0 not at all, 1 a little (the player walks),
   2 most of the way (the player runs) */
int native_touch_stick_level(void) {
    float len = sqrtf(s_vx * s_vx + s_vy * s_vy);

    if (!s_stick_on || len <= 0.35f) {
        return 0;
    }
    return len < 0.65f ? 1 : 2;
}

/* An autosave was written (state.c): a sign shows for a moment */
void touch_saved(void) {
    s_saved_at = SDL_GetTicks();
}

/* No stick on this screen (the name entry): a finger off the options does nothing */
void native_touch_no_stick(void) {
    s_no_stick_until = SDL_GetTicks() + 150;
}

static int dialog_shown(void) {
    return SDL_GetTicks() < s_dialog_until;
}

/* The balloons' arrow (the menu sprites' frame 7), read from VRAM as the game draws it: its parts' texels through
   their CLUT, the CLUT row cycling while automatic dialogues are on. talk_box.c / message_box.c pass the sheet */
#define ARROW_MAX 24
extern uint16_t *g_vram; /* gpu.c: 1024 x 512 */
static uint32_t s_arrow[ARROW_MAX * ARROW_MAX];
static int s_arrow_w, s_arrow_h, s_arrow_ok;

void native_touch_auto_sprite(const void *sheet_ptr, int tex_x, int tex_y, int clut_row) {
    const int32_t *sheet = (const int32_t *)sheet_ptr;
    const uint8_t *ids;
    const int16_t *parts, *p;
    int i, count, row, minx = 1000, miny = 1000, maxx = -1000, maxy = -1000;

    if (sheet == NULL || g_vram == NULL) {
        return;
    }
    ids = (const uint8_t *)sheet + sheet[1];
    parts = (const int16_t *)((const uint8_t *)sheet + sheet[0]);
    for (i = 0; i < 64 && ids[i] != 7; i++) {
    }
    if (i == 64) {
        return;
    }
    p = (const int16_t *)((const uint8_t *)sheet + sheet[i + 2]);
    count = p[0], row = p[1];
    p += 3;
    for (i = 0; i < count; i++) { /* the frame's bounds */
        const int16_t *part = parts + p[i * 3] * 7;
        minx = SDL_min(minx, p[i * 3 + 1]), miny = SDL_min(miny, p[i * 3 + 2]);
        maxx = SDL_max(maxx, p[i * 3 + 1] + part[2]), maxy = SDL_max(maxy, p[i * 3 + 2] + part[3]);
    }
    if (maxx - minx > ARROW_MAX || maxy - miny > ARROW_MAX || maxx <= minx) {
        return;
    }
    s_arrow_w = maxx - minx, s_arrow_h = maxy - miny;
    SDL_memset(s_arrow, 0, sizeof s_arrow);
    for (i = count - 1; i >= 0; i--) { /* drawn as the game does: the last part first */
        const int16_t *part = parts + p[i * 3] * 7; /* u, v, w, h, clutX, clutY, mode */
        int cx = tex_x + part[4], cy = tex_y + part[5] + row + clut_row;
        for (int y = 0; y < part[3]; y++) {
            for (int x = 0; x < part[2]; x++) {
                int u = part[0] + x, v = tex_y + part[1] + y, index;
                uint16_t c;
                if (part[6] == 0) {
                    index = g_vram[(v & 511) * 1024 + ((tex_x + u / 4) & 1023)] >> (u % 4 * 4) & 15;
                } else {
                    index = g_vram[(v & 511) * 1024 + ((tex_x + u / 2) & 1023)] >> (u % 2 * 8) & 255;
                }
                c = g_vram[(cy & 511) * 1024 + ((cx + index) & 1023)];
                if (c != 0) {
                    uint32_t r = (c & 31) << 3, g = (c >> 5 & 31) << 3, b = (c >> 10 & 31) << 3;
                    s_arrow[(p[i * 3 + 2] - miny + y) * ARROW_MAX + (p[i * 3 + 1] - minx + x)] = r | g << 8 | b << 16 | 0xFF000000u;
                }
            }
        }
    }
    s_arrow_ok = 1;
    if (SDL_getenv("DW3_ARROW_DUMP") != NULL) { /* tests: the arrow as read, a PPM */
        FILE *f = fopen("arrow.ppm", "wb");
        if (f != NULL) {
            fprintf(f, "P6 %d %d 255\n", s_arrow_w, s_arrow_h);
            for (int y = 0; y < s_arrow_h; y++) {
                for (int x = 0; x < s_arrow_w; x++) {
                    uint32_t c = s_arrow[y * ARROW_MAX + x];
                    fputc(c & 255, f), fputc(c >> 8 & 255, f), fputc(c >> 16 & 255, f);
                }
            }
            fclose(f);
        }
    }
}

/* For gl/touch.c: the arrow's pixels (ARROW_MAX wide rows), its size; 0 if not read yet */
int touch_arrow(const uint32_t **pixels, int *w, int *h) {
    /* the speed in the arrow's own three dashes (its rows 0-1, 3-4, 6-7, on its left once turned): none at x1 (and
       off), the two by the shaft at x2, all three at x3 */
    static uint32_t shown[ARROW_MAX * ARROW_MAX];
    static const int DASHES[3][2] = {{6, 7}, {3, 4}, {0, 1}};
    int speed = native_auto_text(), keep = speed >= 3 ? 3 : speed == 2 ? 2 : 0;

    SDL_memcpy(shown, s_arrow, sizeof shown);
    if (s_arrow_h >= 12) {
        for (int d = keep; d < 3; d++) {
            for (int k = 0; k < 2; k++) {
                SDL_memset(&shown[DASHES[d][k] * ARROW_MAX], 0, ARROW_MAX * sizeof shown[0]);
            }
        }
    }
    *pixels = shown, *w = s_arrow_w, *h = s_arrow_h;
    return s_arrow_ok;
}

/* The automatic dialogues button: the bottom left corner of the window, over the black band */
static void auto_button(float *x, float *y, float *r) {
    float u = (float)SDL_min(s_w, s_h);
    *r = u * 0.1f;
    *x = *r * 1.3f;
    *y = s_h - *r * 1.3f;
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

static int dialog_shown(void);

/* A scrolling list (the memory card screen's games), in display pixels: a finger dragged on it scrolls it, a tap
   without moving picks the row under it, a finger held still for LIST_HOLD_MS asks for the row's own choice */
#define LIST_HOLD_MS 600
static float s_list[4], s_list_y0, s_list_ylast, s_list_dy;
static Uint64 s_list_until, s_list_from;
static SDL_FingerID s_list_id;
static int s_list_on, s_list_moved, s_list_held, s_list_event, s_list_event_y;

void native_touch_list(int x, int y, int w, int h) {
    s_list[0] = (float)x, s_list[1] = (float)y, s_list[2] = (float)w, s_list[3] = (float)h;
    s_list_until = SDL_GetTicks() + 150;
}

static int list_shown(void) {
    return SDL_GetTicks() < s_list_until;
}

/* What the list's finger did since the last call: 0 nothing, 1 a tap, 2 a hold, with the display row it was at in
   *y; *dy the display pixels it dragged the list by (up negative) */
int native_touch_list_event(int *y, int *dy) {
    int ev = s_list_event;

    *dy = (int)s_list_dy;
    s_list_dy -= (float)*dy;
    *y = s_list_event_y;
    s_list_event = 0;
    return ev;
}

/* The corner buttons, in pixels: the menu at the top right; back at the bottom left, where a thumb reaches it with
   the phone in one hand (over the automatic dialogues button when that one is there too) */
static void corner(int right, float *x, float *y, float *r) {
    float u = (float)SDL_min(s_w, s_h);
    if (right) {
        *r = u * 0.065f;
        *x = s_w - *r * 1.6f;
        *y = *r * 1.6f;
        return;
    }
    *r = u * 0.08f;
    *x = *r * 1.5f;
    *y = s_h - *r * 1.5f;
    if (dialog_shown()) {
        float ax, ay, ar;
        auto_button(&ax, &ay, &ar);
        *y = ay - ar * 1.4f - *r * 1.2f;
    }
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

/* Testing on a computer: DW3_STICK="from:to:x:y,..." pushes the stick (x, y from -1 to 1) from one poll to
   another, as a finger would */
static void test_stick(void) {
    static int polls;
    const char *env = SDL_getenv("DW3_STICK");

    polls++;
    while (env != NULL && *env != 0) {
        int from = 0, to = 0;
        float x = 0, y = 0;

        if (SDL_sscanf(env, "%d:%d:%f:%f", &from, &to, &x, &y) == 4 && polls >= from && polls < to) {
            if (s_rad <= 0) {
                s_rad = 100;
            }
            s_stick_on = 1, s_stick_moved = 1;
            s_bx = s_by = 0;
            s_fx = x * s_rad, s_fy = y * s_rad;
            return;
        }
        env = SDL_strchr(env, ',');
        env = env != NULL ? env + 1 : NULL;
    }
}

void touch_poll(int w, int h, uint16_t *held, uint8_t *lstick) {
    int ndev = 0, seen_stick = 0, seen_skip = 0, seen_list = 0, text_held = 0;
    SDL_TouchID *devs;

    text_input_idle();
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

    s_menu_down = s_back_down = s_auto_down = 0;
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
                if (dialog_shown()) {
                    float bx, by, br;
                    auto_button(&bx, &by, &br);
                    if ((x - bx) * (x - bx) + (y - by) * (y - by) <= br * br * 1.6f) {
                        if (!was) {
                            native_auto_text_toggle();
                        }
                        s_auto_down = 1;
                        continue;
                    }
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
                if (list_shown() || (s_list_on && fingers[i]->id == s_list_id)) {
                    float px = (x - s_pic[0]) / s_pic[2] * s_disp[0], py = (y - s_pic[1]) / s_pic[3] * s_disp[1];

                    if (s_list_on && fingers[i]->id == s_list_id) {
                        if (!s_list_moved && SDL_fabsf(py - s_list_y0) > 4) {
                            s_list_moved = 1;
                        }
                        if (s_list_moved) {
                            s_list_dy += py - s_list_ylast;
                        }
                        s_list_ylast = py;
                        seen_list = 1;
                        continue;
                    }
                    if (!was && !s_list_on && px >= s_list[0] && px < s_list[0] + s_list[2] && py >= s_list[1]
                        && py < s_list[1] + s_list[3]) {
                        s_list_on = 1, s_list_id = fingers[i]->id;
                        s_list_y0 = s_list_ylast = py, s_list_from = SDL_GetTicks();
                        s_list_moved = s_list_held = 0;
                        seen_list = 1;
                        continue;
                    }
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
            } else if (SDL_GetTicks() < s_no_stick_until || SDL_GetTicks() >= s_free_until) {
                /* no stick: only the field, while the player has the control, walks */
            } else if (!s_stick_on) { /* a new finger off the keys: the stick appears under it */
                s_stick_on = seen_stick = 1;
                s_stick_id = fingers[i]->id;
                s_bx = s_fx = x, s_by = s_fy = y;
                s_stick_from = SDL_GetTicks(), s_stick_moved = 0;
            }
        }
        SDL_free(fingers);
    }
    SDL_free(devs);
    if (s_list_on && !seen_list) { /* lifted: a tap if it neither moved nor was held */
        if (!s_list_moved && !s_list_held) {
            s_list_event = 1, s_list_event_y = (int)s_list_y0;
        }
        s_list_on = 0;
    } else if (s_list_on && !s_list_moved && !s_list_held && SDL_GetTicks() - s_list_from >= LIST_HOLD_MS) {
        s_list_held = 1;
        s_list_event = 2, s_list_event_y = (int)s_list_y0;
    }
    s_text_held = text_held;
    if (s_text_tap > 0 && s_text_tap_frame < 0) { /* not read in time: dropped */
        s_text_tap--;
    }
    SDL_memcpy(s_down, now_down, sizeof(SDL_FingerID) * (size_t)now_n);
    s_ndown = now_n;
    forget_targets(now_down, now_n);
    if (!seen_stick) {
        if (s_stick_on && !s_stick_moved && SDL_GetTicks() - s_stick_from < 300) {
            native_touch_hold(13, 3); /* a short tap where the player walks: X (the game's PAD_CROSS) */
        }
        s_stick_on = 0;
    } else if ((s_fx - s_bx) * (s_fx - s_bx) + (s_fy - s_by) * (s_fy - s_by) > s_rad * s_rad * 0.04f) {
        s_stick_moved = 1;
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
    test_stick();
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
    if (s_saved_at != 0 && SDL_GetTicks() - s_saved_at < 1500 && n < max) {
        /* the autosave's sign: a ring filled at the top centre */
        float u = (float)SDL_min(s_w, s_h);
        out[n++] = (TouchShape){s_w * 0.5f, u * 0.08f, u * 0.045f, TOUCH_PROGRESS, 0.85f, 1.0f};
    }
    if (dialog_shown() && n < max) {
        float x, y, r;
        int speed = native_auto_text();
        auto_button(&x, &y, &r);
        /* value: the arrows (the speed, one dim when off) and the animation's phase, while on */
        out[n++] = (TouchShape){x, y, r, TOUCH_AUTO, speed ? 1.0f : (s_auto_down ? 0.9f : 0.6f), (float)speed};
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
