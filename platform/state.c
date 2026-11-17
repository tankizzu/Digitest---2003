/* The debug keys (native.ini: debug = 1), run at the frame's end:
     F1  the stage select (STAGSLCT), the developers' menu: stages, scenes,
         test battles
     F3  the sound test (SOUNDTST), F4 the vibration test (SHOCKTST): the
         European stage select has no lines for them
     F2  GAME (the save data and the mode) to game_<vsync>.bin, to compare
         with an emulator on the same save
     F5  savestate to saves/quick.state, F9 load it
   A savestate is the game's variables (main's and the overlays', one range:
   memory.c), the heap, the overlay areas, VRAM and the platform's console
   state (SAVED: GPU, GTE, SPU, sequencer). It belongs to the exe that wrote
   it: another build has other addresses, so it is refused. Not during a
   movie, whose stream is a host file. */
#include <SDL3/SDL.h>
#include <string.h>
#include "platform.h"
#include "touch.h"
#include "../include/native.h"

extern uint8_t game_vars_begin[], ovl_sub_end[];
extern uint32_t HEAP_BASE[];
extern uint8_t OVERLAY_VRAM[], STAGE_VRAM[];
extern uint8_t GAME[];  /* the game's GameState */
int getMode(void);      /* main/game/state.c */
void requestMode(int mode, int arg);
int VSync(int mode);    /* etc.c */
void loadFont(void);    /* main/text/font.c */

#define GAME_DUMP_SIZE 0x26F0 /* up to its methods (EU) */
#define MODE_STAGE_SELECT 0x1500
#define MODE_SOUND_TEST 0x800     /* SOUNDTST */
#define MODE_VIBRATION_TEST 0x900 /* SHOCKTST */

typedef struct {
    char magic[4];
    uint32_t begin, size, heap; /* where and how big: the exe's fingerprint */
} Header;

static int s_key;

/* --field MODE [--progress N] (main.c): the field to jump to once the game
   reaches the language select or the title, as the stage select does, at
   that story point (GAME.progress) if given; for the field editor's Test */
int g_field_mode, g_field_progress = -1, g_field_event, g_field_at_x = -1, g_field_at_y;
/* --set CODE=VALUE: the flags and items the editor's story point assumes, set as
   actions (main/game/events.c's codes) before the field */
int g_field_sets[64][2], g_field_set_count;
int g_field_arg; /* --arg N: the mode's argument (the stage select's scene test takes 1) */
int g_field_party; /* --party N: the starter set (0-2) given when there is no party */
void applyAction(int code, int value); /* main/game/events.c */
void setParty(int set);                /* main/game/party.c */
#define GAME_PARTY 0x70
#define GAME_PROGRESS 0x263C
#define GAME_MODE 0x26C4 /* EU */
#define MODE_TITLE 0xE00
#define MODE_LANGUAGE 0x1600

void state_key(int key) {
    if (key == SDLK_F1 || key == SDLK_F2 || key == SDLK_F3 || key == SDLK_F4 || key == SDLK_F5 || key == SDLK_F9) {
        s_key = key;
    }
}

typedef struct {
    void *p;
    size_t n;
} Part;

/* The parts, in file order */
static int parts(Part list[5]) {
    list[0] = (Part){game_vars_begin, (size_t)(ovl_sub_end - game_vars_begin)};
    list[1] = (Part){HEAP_BASE, NATIVE_HEAP_SIZE};
    list[2] = (Part){OVERLAY_VRAM, 512 * 1024};
    list[3] = (Part){STAGE_VRAM, 512 * 1024};
    list[4] = (Part){g_vram, VRAM_W * VRAM_H * 2};
    return 5;
}

static void state_path(char *path, size_t n) {
    snprintf(path, n, "%s/../saves/quick.state", data_dir());
}

/* The autosaves (games.c): each game's auto_a.state and auto_b.state, written in turn */
void native_game_dir(int game, char *out, size_t n); /* games.c */
int native_game_current(void);
static void auto_path(int game, int slot, char *path, size_t n) {
    char dir[1100];
    native_game_dir(game, dir, sizeof dir);
    snprintf(path, n, "%s/auto_%c.state", dir, 'a' + slot);
}

static Header header(void) {
    Header h = {{'D', 'W', '3', 'S'}, (uint32_t)(uintptr_t)game_vars_begin,
                (uint32_t)(ovl_sub_end - game_vars_begin), NATIVE_HEAP_SIZE};

    return h;
}

static void save_state_to(const char *path) {
    char tmp[1210];
    Header h = header();
    Part list[5];
    int i, n = parts(list);
    FILE *f;
    int ok;

    soft_all();
    snprintf(tmp, sizeof tmp, "%s.tmp", path);
    if ((f = fopen(tmp, "wb")) == NULL) {
        LOG("state: can't write %s", tmp);
        return;
    }
    ok = fwrite(&h, sizeof h, 1, f) == 1;
    for (i = 0; i < n && ok; i++) {
        ok = fwrite(list[i].p, list[i].n, 1, f) == 1;
    }
    ok = fclose(f) == 0 && ok;
    if (ok && SDL_RenamePath(tmp, path)) { /* never a half-written state over a good one */
        LOG("state: saved %s", path);
    } else {
        LOG("state: saving %s failed", path);
        SDL_RemovePath(tmp);
    }
}

static int state_fits(const char *path);

static void load_state_from(const char *path) {
    Header h, want = header();
    Part list[5];
    int i, n = parts(list);
    FILE *f;
    long size = sizeof h;

    for (i = 0; i < n; i++) {
        size += (long)list[i].n;
    }
    if ((f = fopen(path, "rb")) == NULL) {
        LOG("state: no %s (F5 saves one)", path);
        return;
    }
    fseek(f, 0, SEEK_END);
    if (ftell(f) != size || fseek(f, 0, SEEK_SET) != 0 || fread(&h, sizeof h, 1, f) != 1 || memcmp(&h, &want, sizeof h) != 0) {
        LOG("state: %s is from another build, not loaded", path);
        fclose(f);
        return;
    }
    native_read_wait(); /* a disc read still in flight would write over what is loaded */
    for (i = 0; i < n; i++) {
        if (fread(list[i].p, list[i].n, 1, f) != 1) {
            LOG("state: %s cut short", path); /* can't happen: the size was checked */
        }
    }
    fclose(f);
    spu_cd_clear();
    soft_discard();
    gfx_sync(1);
    LOG("state: loaded %s", path);
}

static void save_state(void) {
    char path[1200];
    state_path(path, sizeof path);
    save_state_to(path);
}

static void load_state(void) {
    char path[1200];
    state_path(path, sizeof path);
    load_state_from(path);
}

/* A state file this build wrote (its size and fingerprint) */
static int state_fits(const char *path) {
    Header h, want = header();
    Part list[5];
    int i, n = parts(list), ok;
    long size = sizeof h;
    FILE *f = fopen(path, "rb");

    for (i = 0; i < n; i++) {
        size += (long)list[i].n;
    }
    if (f == NULL) {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    ok = ftell(f) == size && fseek(f, 0, SEEK_SET) == 0 && fread(&h, sizeof h, 1, f) == 1 && memcmp(&h, &want, sizeof h) == 0;
    fclose(f);
    return ok;
}

#define AUTOSAVE_EVERY (60 * 60 * 3) /* vsyncs: three minutes */
static int s_auto_last, s_auto_load;

/* When a state file was written (0: none, or not this build's) */
static Sint64 state_time(const char *path) {
    SDL_PathInfo info;
    return state_fits(path) && SDL_GetPathInfo(path, &info) ? info.modify_time : 0;
}

/* The game's newest autosave this build can load (its slot), or -1 */
static int newest_autosave(int game) {
    char a[1200], b[1200];
    Sint64 ta, tb;

    auto_path(game, 0, a, sizeof a);
    auto_path(game, 1, b, sizeof b);
    ta = state_time(a), tb = state_time(b);
    if (ta == 0 && tb == 0) {
        return -1;
    }
    return ta >= tb ? 0 : 1;
}

int native_autosave_exists(int game) {
    return game > 0 && newest_autosave(game) >= 0;
}

/* Loads the game's newest autosave at this frame's end (Continue, STGMCARD's patch) */
void native_autosave_load(int game) {
    s_auto_load = game;
}

/* The current game's autosave, into the slot not written last (the older, or one that doesn't load) */
static void autosave(void) {
    int game = native_game_current(), newest;
    char path[1200];

    if (game == 0) {
        return;
    }
    newest = newest_autosave(game);
    auto_path(game, newest == 0 ? 1 : 0, path, sizeof path);
    save_state_to(path);
    s_auto_last = VSync(-1);
}

/* The app is about to go to the background: called from SDL's event pump, on the game's thread at a frame's end */
static bool SDLCALL on_lifecycle(void *data, SDL_Event *e) {
    (void)data;
    if (e->type == SDL_EVENT_WILL_ENTER_BACKGROUND && touch_enabled() && !movie_playing() && getMode() >> 8 != MODE_TITLE >> 8 &&
        getMode() >> 8 != MODE_LANGUAGE >> 8 && getMode() >> 8 != 0x0C) {
        autosave();
    }
    return true;
}

/* Once a frame: an autosave Continue asked for, and (where touch is on) a save every few minutes of play, not at
   the title, the language select, the memory card screen (0x0C) or a movie */
static void autosave_frame(void) {
    static int watching;
    int mode = getMode() >> 8, every;

    if (s_auto_load > 0) {
        int game = s_auto_load, slot = newest_autosave(game);
        char path[1200];

        s_auto_load = 0;
        if (slot >= 0) {
            auto_path(game, slot, path, sizeof path);
            load_state_from(path);
            s_auto_last = VSync(-1);
        }
        return;
    }
    if (!touch_enabled()) {
        return;
    }
    if (!watching) {
        SDL_AddEventWatch(on_lifecycle, NULL);
        watching = 1;
        s_auto_last = VSync(-1);
    }
    every = SDL_getenv("DW3_AUTOSAVE_VSYNCS") ? SDL_atoi(SDL_getenv("DW3_AUTOSAVE_VSYNCS")) : AUTOSAVE_EVERY;
    if (mode != MODE_TITLE >> 8 && mode != MODE_LANGUAGE >> 8 && mode != 0x0C && !movie_playing() && VSync(-1) - s_auto_last >= every) {
        autosave();
    }
}

static void dump_game(void) {
    char path[64];
    FILE *f;

    snprintf(path, sizeof path, "game_%05d.bin", VSync(-1));
    if ((f = fopen(path, "wb")) == NULL || fwrite(GAME, GAME_DUMP_SIZE, 1, f) != 1) {
        LOG("dump: can't write %s", path);
    } else {
        LOG("dump: %s, mode %04X", path, getMode());
    }
    if (f != NULL) {
        fclose(f);
    }
}

void state_frame(void) {
    int key = s_key;

    s_key = 0;
    autosave_frame();
    if (g_field_mode != 0 && (getMode() >> 8 == MODE_TITLE >> 8 || getMode() >> 8 == MODE_LANGUAGE >> 8)) {
        int title = MODE_TITLE;

        LOG("field: mode %04X%s", g_field_mode, g_field_progress >= 0 ? ", with a story point" : "");
        if (getMode() >> 8 == MODE_LANGUAGE >> 8) {
            loadFont(); /* as F1: the language select wipes the font */
        }
        if (g_field_progress >= 0) {
            memcpy(GAME + GAME_PROGRESS, &g_field_progress, 4);
        }
        /* no save: the party a new game is given (a starter set), which battles need */
        if (*(int32_t *)(GAME + GAME_PARTY) < 0) {
            setParty(g_field_party);
        }
        for (int i = 0; i < g_field_set_count; i++) {
            applyAction(g_field_sets[i][0], g_field_sets[i][1]);
        }
        /* as if from the title: from another mode the field puts the player
           back where a field was left (GAME.fieldPos, 0, 0 here), not at
           its start (fieldstg/field.c startField) */
        memcpy(GAME + GAME_MODE, &title, 4);
        requestMode(g_field_mode, g_field_arg);
        g_field_mode = 0;
    }
    if (key == SDLK_F1 || key == SDLK_F3 || key == SDLK_F4) {
        int mode = key == SDLK_F1 ? MODE_STAGE_SELECT : key == SDLK_F3 ? MODE_SOUND_TEST : MODE_VIBRATION_TEST;

        LOG("debug: mode %04X (vsync %d, from mode %04X)", mode, VSync(-1), getMode());
        if (getMode() >> 8 == 0x16) {
            /* the language select wipes the font, which the next movie's end
               loads again (stdwtitl/movie.c): the same here */
            loadFont();
        }
        requestMode(mode, 0);
    } else if (key == SDLK_F2) {
        dump_game();
    } else if ((key == SDLK_F5 || key == SDLK_F9) && movie_playing()) {
        LOG("state: not during a movie");
    } else if (key == SDLK_F5) {
        save_state();
    } else if (key == SDLK_F9) {
        load_state();
    }
}
