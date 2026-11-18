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

void native_game_dir(int game, char *out, size_t n); /* games.c */
int native_game_current(void);
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

/* The autosaves (games.c): a game save as the memory card has it (GAME's first GAME_SAVE_SIZE bytes, its checksum
   and version), written to the game's auto_a.sav and auto_b.sav in turn: the older, or one that doesn't load. Not a
   savestate: it loads in any build, and at any address. Taken where the field leaves the player in control, as the
   field menu's save would: every few minutes, and when the app goes to the background. Continue's "Automatic"
   loads the newest good one as a memory card load would (STGMCARD saves.c) */
#define GAME_SAVE_SIZE 0x26C4      /* EU */
#define SAVE_VERSION 4             /* MEMCARD_SAVE_VERSION, EU */
#define GAME_FIELD_MODE 0x34       /* GameState.fieldMode, fieldPos (x, y), fieldDir: where a load goes back to */
#define AUTOSAVE_EVERY (60 * 60 * 2) /* vsyncs: two minutes */
uint8_t computeChecksum(void *data, int32_t size); /* main/memcard */
void touch_saved(void);                           /* touch.c: the "saved" sign */
void native_thumb_autosave(int game, int slot);   /* thumbs.c */
void native_thumb_field(int mode);

static int s_auto_last = -1, s_point[4];
/* The game as it was the last frame the player was in control (native_autosave_point), which the app going to the
   background saves even in the middle of a scene; s_snap_at when it was taken, s_written_at the last one written */
static uint8_t s_snap[GAME_SAVE_SIZE];
static int s_snap_point[4], s_snap_at = -1, s_written_at = -1;

static void auto_path(int game, int slot, char *path, size_t n) {
    char dir[1100];
    native_game_dir(game, dir, sizeof dir);
    snprintf(path, n, "%s/auto_%c.sav", dir, 'a' + slot);
}

/* A good save in that file (its size, checksum and version): when it was written, else 0 */
static Sint64 save_time(const char *path, uint8_t *into) {
    static uint8_t buf[GAME_SAVE_SIZE];
    SDL_PathInfo info;
    FILE *f = fopen(path, "rb");
    int ok;

    if (f == NULL) {
        return 0;
    }
    ok = fread(buf, GAME_SAVE_SIZE, 1, f) == 1 && fgetc(f) == EOF;
    fclose(f);
    ok = ok && buf[2] == SAVE_VERSION && (computeChecksum(buf + 4, GAME_SAVE_SIZE - 4) & ~buf[0]) == 0;
    if (!ok || !SDL_GetPathInfo(path, &info)) {
        return 0;
    }
    if (into != NULL) {
        memcpy(into, buf, GAME_SAVE_SIZE);
    }
    return info.modify_time > 0 ? info.modify_time : 1;
}

/* The game's newest good autosave (its slot), or -1 */
static int newest_autosave(int game) {
    char a[1200], b[1200];
    Sint64 ta, tb;

    auto_path(game, 0, a, sizeof a);
    auto_path(game, 1, b, sizeof b);
    ta = save_time(a, NULL), tb = save_time(b, NULL);
    if (ta == 0 && tb == 0) {
        return -1;
    }
    return ta >= tb ? 0 : 1;
}

int native_autosave_exists(int game) {
    return game > 0 && newest_autosave(game) >= 0;
}

/* The game's newest autosave into `buf` (GAME_SAVE_SIZE bytes): 1 if there is one */
int native_autosave_read(int game, void *buf) {
    int slot = game > 0 ? newest_autosave(game) : -1;
    char path[1200];

    if (slot < 0) {
        return 0;
    }
    auto_path(game, slot, path, sizeof path);
    return save_time(path, buf) != 0;
}

/* One of the game's two autosaves into `buf`: which 0 the newest, 1 the other (Continue's list of the two). Its slot
   (0 auto_a, 1 auto_b), or -1 if that one isn't a good save */
int native_autosave_read_slot(int game, int which, void *buf) {
    int newest = game > 0 ? newest_autosave(game) : -1, slot;
    char path[1200];

    if (newest < 0) {
        return -1;
    }
    slot = which == 0 ? newest : 1 - newest;
    auto_path(game, slot, path, sizeof path);
    return save_time(path, buf) != 0 ? slot : -1;
}

/* `game` (GAME_SAVE_SIZE bytes of GAME) as a save, with the field and place `point` (where the player was in
   control), into the slot not written last */
static void write_autosave(const uint8_t *state, const int *point) {
    static uint8_t buf[GAME_SAVE_SIZE];
    int game = native_game_current(), newest;
    char path[1200], tmp[1210];
    FILE *f;

    if (game == 0) {
        return;
    }
    memcpy(buf, state, GAME_SAVE_SIZE);
    memcpy(buf + GAME_FIELD_MODE, point, 4 * sizeof *point);
    buf[2] = SAVE_VERSION;
    buf[0] = computeChecksum(buf + 4, GAME_SAVE_SIZE - 4);
    newest = newest_autosave(game);
    auto_path(game, newest == 0 ? 1 : 0, path, sizeof path);
    snprintf(tmp, sizeof tmp, "%s.tmp", path);
    if ((f = fopen(tmp, "wb")) != NULL && fwrite(buf, GAME_SAVE_SIZE, 1, f) == 1 && fclose(f) == 0 && SDL_RenamePath(tmp, path)) {
        LOG("autosave: %s", path);
        native_thumb_autosave(game, newest == 0 ? 1 : 0); /* its picture (thumbs.c) */
        touch_saved();
    } else {
        LOG("autosave: can't write %s", path);
    }
    s_auto_last = VSync(-1);
}

/* GAME now, the player in control */
static void autosave(void) {
    write_autosave(GAME, s_point);
    s_written_at = s_snap_at;
}

/* The app is about to go to the background (SDL's event pump, on the game's thread): the game as it was the last
   time the player was in control, if that wasn't saved yet (in a scene or a dialog too: the prologue has few
   minutes in control) */
static bool SDLCALL on_lifecycle(void *data, SDL_Event *e) {
    (void)data;
    if (e->type == SDL_EVENT_WILL_ENTER_BACKGROUND && s_snap_at >= 0 && s_snap_at != s_written_at) {
        write_autosave(s_snap, s_snap_point);
        s_written_at = s_snap_at;
    }
    return true;
}

/* The field, each frame the player has the control (field.c): where a load would bring them back, and a save when
   it is due */
void native_autosave_point(int mode, int x, int y, int dir) {
    static int watching;
    int every = SDL_getenv("DW3_AUTOSAVE_VSYNCS") ? SDL_atoi(SDL_getenv("DW3_AUTOSAVE_VSYNCS")) : AUTOSAVE_EVERY;

    if (!watching) {
        SDL_AddEventWatch(on_lifecycle, NULL);
        watching = 1;
    }
    s_point[0] = mode, s_point[1] = x, s_point[2] = y, s_point[3] = dir;
    native_thumb_field(mode); /* a picture of the field now and then, for a save's (thumbs.c) */
    memcpy(s_snap, GAME, GAME_SAVE_SIZE); /* what the background save writes */
    memcpy(s_snap_point, s_point, sizeof s_point);
    s_snap_at = VSync(-1);
    if (s_auto_last < 0) {
        s_auto_last = VSync(-1); /* the first time in control: the count starts */
    }
    /* every few minutes of play, scenes and dialogs included: at the first moment in control once it is due (the
       prologue seldom leaves the player two minutes in control in a row) */
    if (VSync(-1) - s_auto_last >= every) {
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
