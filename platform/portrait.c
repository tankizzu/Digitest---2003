/* The field upright (TECHNICAL_NOTES, "Verticale, fase 1"): the field draws a 320x512 screen at VRAM (0, 512), one
   buffer, in the rows a 2 MB GPU adds (VRAM_H; rows 496-511 hold the game's CLUTs, so it can't start higher); the map keeps 27 of its tiles in VRAM instead of 12, the 15 more at
   x 384-1023, y 512-895, and streams 4 columns by 7 rows of them. Off unless DW3_PORTRAIT=1 or prefs.txt's
   portrait = 1, while it is tried out. */
#include <SDL3/SDL.h>
#include "platform.h"

int pref_get(const char *key, int fallback); /* prefs.c */

static int s_portrait = -1;

int native_portrait(void) {
    if (s_portrait < 0) {
        const char *env = SDL_getenv("DW3_PORTRAIT");
        s_portrait = env != NULL ? SDL_atoi(env) != 0 : pref_get("portrait", 0) != 0;
    }
    return s_portrait;
}

/* The field's screen height: 512 upright, else the game's 240 */
int native_field_h(void) {
    return native_portrait() ? 512 : 240;
}

/* Where map image `slot` (12 and up, the port's) goes in VRAM: five to a row of 128-pixel cells from x 384, rows from
   y 512 (texture page bit 11) */
void native_map_image_pos(int slot, int *x, int *y) {
    int k = slot - 12;

    *x = 384 + k % 5 * 128;
    *y = 512 + k / 5 * 128;
}
