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

#define MAX_GAMES 99

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

/* A new game from the title: the next number, an empty card */
void native_game_new(void) {
    int next;

    ready();
    next = s_count > 0 ? s_games[s_count - 1] + 1 : 1;
    if (next > MAX_GAMES) {
        next = MAX_GAMES; /* ponytail: the last one is used again; no player makes a hundred games */
    }
    native_game_select(next);
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
