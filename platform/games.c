/* The player's games (Android): each in saves/game<N>/, with its own memory card
   image (card.mcd, the game's saves) and its two autosaves (auto_a.state,
   auto_b.state, written in turn: a bad one leaves the other). A new game from
   the title makes the next one; Continue lists them (STGMCARD's patches).
   The card first played, saves/card1.mcd, becomes game 1. */
#include <SDL3/SDL.h>
#include <stdio.h>

const char *data_dir(void);              /* main.c */
int pref_get(const char *key, int fallback); /* prefs.c */
void pref_set(const char *key, int value);
void memcard_open(const char *path);     /* memcard.c */
void log_once(const char *what);

#define MAX_GAMES 99   /* folders looked at (older builds made more) */
#define NATIVE_SLOTS 10 /* the MEMORY CARD slots offered */

static int s_games[MAX_GAMES], s_count = -1; /* the games' numbers, ascending */

static void saves_dir(char *out, size_t n) {
    SDL_snprintf(out, n, "%s/../saves", data_dir());
}

void native_game_dir(int game, char *out, size_t n) {
    SDL_snprintf(out, n, "%s/../saves/game%d", data_dir(), game);
}

static void scan(void) {
    char path[1200];

    s_count = 0;
    for (int g = 1; g <= MAX_GAMES; g++) {
        native_game_dir(g, path, sizeof path);
        if (SDL_GetPathInfo(path, NULL)) {
            s_games[s_count++] = g;
        }
    }
}

/* The card played before games existed becomes game 1 */
static void migrate(void) {
    char old[1200], dir[1200], card[1250];

    saves_dir(old, sizeof old);
    SDL_strlcat(old, "/card1.mcd", sizeof old);
    if (s_count == 0 && SDL_GetPathInfo(old, NULL)) {
        native_game_dir(1, dir, sizeof dir);
        SDL_CreateDirectory(dir);
        SDL_snprintf(card, sizeof card, "%s/card.mcd", dir);
        if (SDL_RenamePath(old, card)) {
            pref_set("game", 1);
        }
        scan();
    }
}

static void ready(void) {
    if (s_count < 0) {
        scan();
        migrate();
    }
}

int native_game_delete(int game);

/* Whether game `game` (a MEMORY CARD slot) has a folder */
int native_game_exists(int game) {
    ready();
    for (int i = 0; i < s_count; i++) {
        if (s_games[i] == game) {
            return 1;
        }
    }
    return 0;
}

int native_game_count(void) {
    ready();
    return s_count;
}

/* The number of the i-th game (0 first), as Continue lists them */
int native_game_number(int i) {
    ready();
    return i >= 0 && i < s_count ? s_games[i] : 0;
}

/* The game being played (its number), 0 if none */
int native_game_current(void) {
    int g;

    ready();
    g = pref_get("game", 0);
    for (int i = 0; i < s_count; i++) {
        if (s_games[i] == g) {
            return g;
        }
    }
    return 0;
}

/* Plays game `game` from now on: its memory card is the one the game reads and writes */
void native_game_select(int game) {
    char dir[1200], card[1250];

    native_game_dir(game, dir, sizeof dir);
    SDL_CreateDirectory(dir);
    SDL_snprintf(card, sizeof card, "%s/card.mcd", dir);
    pref_set("game", game);
    memcard_open(card);
    scan();
}

/* When a game was made: the time in its created.txt (seconds), else its card's last write, else 0 */
static SDL_Time created(int game) {
    char path[1250];
    SDL_PathInfo info;
    size_t len = 0;
    char *text;
    SDL_Time t = 0;

    native_game_dir(game, path, sizeof path);
    SDL_strlcat(path, "/created.txt", sizeof path);
    text = SDL_LoadFile(path, &len);
    if (text != NULL) {
        t = (SDL_Time)SDL_strtoll(text, NULL, 10) * SDL_NS_PER_SECOND;
        SDL_free(text);
        return t;
    }
    native_game_dir(game, path, sizeof path);
    SDL_strlcat(path, "/card.mcd", sizeof path);
    if (SDL_GetPathInfo(path, &info)) {
        return info.modify_time;
    }
    return 0;
}

/* A new game from the title: the next number, an empty card, the time it was made */
void native_game_new(void) {
    int next;
    char path[1250], text[32];
    SDL_Time now = 0;

    ready();
    /* the first free of the ten MEMORY CARD slots; all ten taken, the one made longest ago is used again */
    next = 0;
    for (int g = 1; g <= NATIVE_SLOTS && next == 0; g++) {
        if (!native_game_exists(g)) {
            next = g;
        }
    }
    if (next == 0) {
        SDL_Time oldest = 0;

        next = 1;
        for (int g = 1; g <= NATIVE_SLOTS; g++) {
            SDL_Time t = created(g);
            if (oldest == 0 || t < oldest) {
                oldest = t, next = g;
            }
        }
        native_game_delete(next);
    }
    native_game_select(next);
    native_game_dir(next, path, sizeof path);
    SDL_strlcat(path, "/created.txt", sizeof path);
    SDL_GetCurrentTime(&now);
    SDL_snprintf(text, sizeof text, "%lld\n", (long long)(now / SDL_NS_PER_SECOND));
    SDL_SaveFile(path, text, SDL_strlen(text));
}

/* Deletes a game: its card, autosaves and the rest of its folder; the game played becomes none if it was this one.
   1 if it is gone */
static SDL_EnumerationResult remove_entry(void *userdata, const char *dirname, const char *fname) {
    char path[1300];

    (void)userdata;
    SDL_snprintf(path, sizeof path, "%s%s", dirname, fname);
    SDL_RemovePath(path);
    return SDL_ENUM_CONTINUE;
}

int native_game_delete(int game) {
    char dir[1200];

    ready();
    native_game_dir(game, dir, sizeof dir);
    SDL_EnumerateDirectory(dir, remove_entry, NULL);
    SDL_RemovePath(dir);
    if (pref_get("game", 0) == game) {
        pref_set("game", 0);
    }
    scan();
    return !SDL_GetPathInfo(dir, NULL);
}

int native_text_encode(const char *s, unsigned char *out, int room); /* savetexts.c */

/* The memory card screen's list (STGMCARD's patches): `rows` games from the `top`-th, each "MEMORY CARD N" with the
   day and time it was made, a blank line between them so that each row is two lines high for a finger. In the
   game's encoding, its 00 at the end; the length written */
int native_games_list_text(int top, int rows, unsigned char *out, int room) {
    int n = 0;

    ready();
    for (int i = top; i < top + rows && i < s_count && n < room - 64; i++) {
        char line[96]; /* |01|05 the game's full stop, |01|07 its colon */
        SDL_DateTime dt;
        SDL_Time t = created(s_games[i]);

        if (t != 0 && SDL_TimeToDateTime(t, &dt, true)) {
            SDL_snprintf(line, sizeof line, "%sMEMORY CARD %d   %02d|01|05%02d|01|05%04d  %02d|01|07%02d", i > top ? "\n\n" : "",
                         s_games[i], dt.day, dt.month, dt.year, dt.hour, dt.minute);
        } else {
            SDL_snprintf(line, sizeof line, "%sMEMORY CARD %d", i > top ? "\n\n" : "", s_games[i]);
        }
        n += native_text_encode(line, out + n, room - n) - 1; /* without its 00: the next goes on */
    }
    out[n++] = 0;
    return n;
}

/* The memory card at the start: the current game's, else none yet (a blank card that isn't written) */
void native_game_card_path(char *out, size_t n) {
    int g = native_game_current();

    if (g == 0) {
        out[0] = 0;
        return;
    }
    native_game_dir(g, out, n);
    SDL_strlcat(out, "/card.mcd", n);
}

/* The upright memory card screen's column (STGMCARD's patches): slots `first` to first + count - 1, two lines each:
   "MEMORY CARD N", then the day and time it was made or the word for an empty one (`empty`). Game-encoded, its 00 at
   the end; the length written */
int native_games_column_text(int first, int count, const char *empty, unsigned char *out, int room) {
    int n = 0;

    ready();
    for (int g = first; g < first + count && n < room - 64; g++) {
        char line[96]; /* |01|05 the game's full stop, |01|07 its colon */
        SDL_DateTime dt;
        SDL_Time t = native_game_exists(g) ? created(g) : 0;

        if (!native_game_exists(g)) {
            SDL_snprintf(line, sizeof line, "%sMEMORY CARD %d\n %s", g > first ? "\n" : "", g, empty);
        } else if (t != 0 && SDL_TimeToDateTime(t, &dt, true)) {
            SDL_snprintf(line, sizeof line, "%sMEMORY CARD %d\n %02d|01|05%02d|01|05%02d %02d|01|07%02d", g > first ? "\n" : "",
                         g, dt.day, dt.month, dt.year % 100, dt.hour, dt.minute);
        } else {
            SDL_snprintf(line, sizeof line, "%sMEMORY CARD %d\n", g > first ? "\n" : "", g);
        }
        n += native_text_encode(line, out + n, room - n) - 1;
    }
    out[n++] = 0;
    return n;
}
