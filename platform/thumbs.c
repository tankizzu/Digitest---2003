/* The saves' pictures (the memory card screen's lists, STGMCARD's patches): the field as the player last saw it in
   control, THUMB_W x THUMB_H, a 4:3 piece around the screen's middle at half size. Taken every few seconds while the
   player has the control (state.c's native_autosave_point calls native_thumb_field), and at once for an autosave;
   written beside the save: auto_a.thumb / auto_b.thumb with the autosaves, save_0..2.thumb with the card's slots
   (native_thumb_save_slot, once a slot's save is written). Raw 15-bit pixels with bit 15 set (a texel of 0 is clear
   on the PlayStation: a black one stays black). Shown by native_thumb_show, which loads one into VRAM */
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"

#define THUMB_W 160
#define THUMB_H 120
#define THUMB_EVERY 240 /* vsyncs between two pictures while the player walks */

extern uint16_t *g_vram;
int LoadImage(RECT *r, uint32_t *p);                     /* gpu.c */
int VSync(int mode);
void soft_read(int x, int y, int w, int h);              /* gpu_draw.c: the software VRAM made current there */
void native_game_dir(int game, char *out, size_t n);     /* games.c */
int native_game_current(void);

static uint16_t s_last[THUMB_W * THUMB_H];
static int s_last_ok, s_last_at = -100000, s_last_mode = -1;

/* The picture of the area shown now: its middle 320 x 240 at half size, each pixel the mean of four */
static void capture(void) {
    int x0 = g_disp.disp.x, w = SDL_min(g_disp.disp.w, 320), h = g_disp.disp.h;
    int y0 = g_disp.disp.y + SDL_max(0, (h - 240) / 2);

    if (g_vram == NULL || w < 2 * THUMB_W || h < 2 * THUMB_H) {
        return;
    }
    x0 += (w - 2 * THUMB_W) / 2;
    soft_read(x0, y0, 2 * THUMB_W, 2 * THUMB_H);
    for (int y = 0; y < THUMB_H; y++) {
        for (int x = 0; x < THUMB_W; x++) {
            int r = 0, g = 0, b = 0;

            for (int k = 0; k < 4; k++) {
                uint16_t c = g_vram[((y0 + 2 * y + k / 2) & (VRAM_H - 1)) * VRAM_W + ((x0 + 2 * x + k % 2) & 1023)];
                r += c & 31, g += c >> 5 & 31, b += c >> 10 & 31;
            }
            s_last[y * THUMB_W + x] = (uint16_t)(0x8000 | (r / 4) | (g / 4) << 5 | (b / 4) << 10);
        }
    }
    s_last_ok = 1;
}

/* The field, a frame the player has the control (state.c): a picture now and then, and at once in a new place */
void native_thumb_field(int mode) {
    int now = VSync(-1);

    if (now - s_last_at >= THUMB_EVERY || mode != s_last_mode || now < s_last_at) {
        capture();
        s_last_at = now, s_last_mode = mode;
    }
}

static void thumb_path(int game, const char *name, char *out, size_t n) {
    char dir[1100];

    native_game_dir(game, dir, sizeof dir);
    SDL_snprintf(out, n, "%s/%s.thumb", dir, name);
}

static void write_thumb(const char *path) {
    FILE *f;

    if (!s_last_ok) {
        remove(path); /* none taken: no old picture under a new save */
        return;
    }
    if ((f = fopen(path, "wb")) != NULL) {
        fwrite(s_last, sizeof s_last, 1, f);
        fclose(f);
    }
}

/* An autosave was written to slot 0 (auto_a) or 1 (auto_b): its picture, taken now */
void native_thumb_autosave(int game, int slot) {
    char path[1200];

    capture();
    s_last_at = VSync(-1);
    thumb_path(game, slot == 0 ? "auto_a" : "auto_b", path, sizeof path);
    write_thumb(path);
}

/* The current game's card slot (0-2) was saved: the last picture of the field */
void native_thumb_save_slot(int slot) {
    char path[1200], name[16];

    if (native_game_current() == 0 || slot < 0 || slot > 2) {
        return;
    }
    SDL_snprintf(name, sizeof name, "save_%d", slot);
    thumb_path(native_game_current(), name, path, sizeof path);
    write_thumb(path);
}

/* The picture of the current game's card slot (kind 0, slot 0-2) or autosave (kind 1, slot 0 auto_a, 1 auto_b) into
   VRAM at x, y (THUMB_W x THUMB_H): 1 if there is one */
int native_thumb_show(int kind, int slot, int x, int y) {
    static uint16_t buf[THUMB_W * THUMB_H];
    char path[1200], name[16];
    RECT r = {(int16_t)x, (int16_t)y, THUMB_W, THUMB_H};
    FILE *f;
    int ok;

    if (native_game_current() == 0) {
        return 0;
    }
    if (kind == 0) {
        SDL_snprintf(name, sizeof name, "save_%d", slot);
    } else {
        SDL_strlcpy(name, slot == 0 ? "auto_a" : "auto_b", sizeof name);
    }
    thumb_path(native_game_current(), name, path, sizeof path);
    if ((f = fopen(path, "rb")) == NULL) {
        return 0;
    }
    ok = fread(buf, sizeof buf, 1, f) == 1;
    fclose(f);
    if (ok) {
        LoadImage(&r, (uint32_t *)buf);
    }
    return ok;
}
