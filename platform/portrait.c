/* The field upright (TECHNICAL_NOTES, "Verticale, fase 1"): the field draws a 320 x native_field_h() screen at VRAM
   (0, 512), one buffer, in the rows the port adds below the PlayStation's 512 (VRAM_H; rows 496-511 hold the game's
   CLUTs, so it can't start higher). Its height follows the phone's shape, the width staying the game's 320: the
   whole screen, the pixels as big as they come. The map keeps 29 of its tiles in VRAM instead of 12 (the 17 more at
   x 384-1023, y 512-1023, their CLUTs at x 640, y 896 on) and streams 4 columns by up to 7 rows of them. On on
   Android, off on a computer unless DW3_PORTRAIT=1 (DW3_PORTRAIT_H sets the height there); prefs.txt's portrait =
   0/1 overrides. */
#include <SDL3/SDL.h>
#include "platform.h"

int pref_get(const char *key, int fallback); /* prefs.c */

#define FIELD_H_MIN 240
#define FIELD_H_MAX 720 /* the map draws up to 7 rows of 128 (map_streamer.c) */

static int s_portrait = -1, s_field_h;

int native_portrait(void) {
    if (s_portrait < 0) {
        const char *env = SDL_getenv("DW3_PORTRAIT");
#ifdef __ANDROID__
        const int fallback = 1; /* the phone: upright (the app starts upright, AndroidManifest.xml) */
#else
        const int fallback = 0;
#endif
        s_portrait = env != NULL ? SDL_atoi(env) != 0 : pref_get("portrait", fallback) != 0;
    }
    return s_portrait;
}

/* The field's screen height: upright, 320 wide in the screen's shape (the longer side over the shorter, as the
   phone may still be sideways when this is first asked); else the game's 240 */
int native_field_h(void) {
    if (!native_portrait()) {
        return 240;
    }
    if (s_field_h == 0) {
        const char *env = SDL_getenv("DW3_PORTRAIT_H");
        SDL_Rect r = {0, 0, 1080, 2400};

        if (env != NULL) {
            s_field_h = SDL_atoi(env);
        } else {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
            SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &r);
            s_field_h = 320 * SDL_max(r.w, r.h) / SDL_max(1, SDL_min(r.w, r.h));
        }
        s_field_h = SDL_clamp(s_field_h & ~1, FIELD_H_MIN, FIELD_H_MAX);
    }
    return s_field_h;
}

/* Where map image `slot` (12 and up, the port's) goes in VRAM: five to a row of 128-pixel cells from x 384, rows from
   y 512 (texture page bit 11); the fourth row has two, x 384 and 512 */
void native_map_image_pos(int slot, int *x, int *y) {
    int k = slot - 12;

    *x = 384 + k % 5 * 128;
    *y = 512 + k / 5 * 128;
}

/* Where map image `slot`'s CLUT goes, upright: x 640, a row each from 896 (beside the fourth row's two images; a
   CLUT's y has 10 bits) */
void native_map_clut_pos(int slot, int *x, int *y) {
    *x = 640;
    *y = 896 + slot;
}

/* The title's settings button, while the user chooses among the proposals (17/11/2026): 1 the game's cog icon alone,
   2 the cog in the game's small panel (top right), 3 the panel under the menu. DW3_SETTINGS_LOOK picks one */
int native_settings_look(void) {
    const char *env = SDL_getenv("DW3_SETTINGS_LOOK");
    int look = env != NULL ? SDL_atoi(env) : 2;

    return look >= 1 && look <= 3 ? look : 2;
}

/* The field filled with its scene zoomed where the map is lower than the tall screen (the prologue's street in Asuka
   City): a proposal to show, on with DW3_ZOOM_MAP=1 until the user approves it (TECHNICAL_NOTES.md) */
int native_field_zoom_on(void) {
    static int on = -1;

    if (on < 0) {
        const char *env = SDL_getenv("DW3_ZOOM_MAP");

        on = native_portrait() && env != NULL && SDL_atoi(env) != 0;
    }
    return on;
}
