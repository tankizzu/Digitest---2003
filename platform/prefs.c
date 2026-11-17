/* The player's choices the game itself doesn't keep, in saves/prefs.txt beside
   the memory card, as "key = number" lines: the language (so the language
   select shows once) and automatic dialogues */
#include <SDL3/SDL.h>
#include <stdio.h>

const char *data_dir(void); /* main.c */

#define KEYS 8
static char s_keys[KEYS][24];
static int s_values[KEYS], s_count = -1;

static void prefs_path(char *out, size_t n) {
    SDL_snprintf(out, n, "%s/../saves/prefs.txt", data_dir());
}

static void load(void) {
    char path[1100], line[128], key[24];
    FILE *f;
    int v;

    if (s_count >= 0) {
        return;
    }
    s_count = 0;
    prefs_path(path, sizeof path);
    if ((f = fopen(path, "r")) == NULL) {
        return;
    }
    while (s_count < KEYS && fgets(line, sizeof line, f) != NULL) {
        if (SDL_sscanf(line, " %23[a-z_] = %d", key, &v) == 2) {
            SDL_strlcpy(s_keys[s_count], key, sizeof s_keys[0]);
            s_values[s_count++] = v;
        }
    }
    fclose(f);
}

static void save(void) {
    char path[1100], dir[1100];
    FILE *f;

    SDL_snprintf(dir, sizeof dir, "%s/../saves", data_dir());
    SDL_CreateDirectory(dir);
    prefs_path(path, sizeof path);
    if ((f = fopen(path, "w")) != NULL) {
        for (int i = 0; i < s_count; i++) {
            fprintf(f, "%s = %d\n", s_keys[i], s_values[i]);
        }
        fclose(f);
    }
}

int pref_get(const char *key, int fallback) {
    load();
    for (int i = 0; i < s_count; i++) {
        if (SDL_strcmp(s_keys[i], key) == 0) {
            return s_values[i];
        }
    }
    return fallback;
}

void pref_set(const char *key, int value) {
    int i;

    load();
    for (i = 0; i < s_count && SDL_strcmp(s_keys[i], key) != 0; i++) {
    }
    if (i == s_count) {
        if (s_count == KEYS) {
            return;
        }
        SDL_strlcpy(s_keys[s_count++], key, sizeof s_keys[0]);
    }
    s_values[i] = value;
    save();
}

int native_saved_language(void) {
    int v = pref_get("language", -1);
    return v >= 0 && v <= 6 ? v : -1;
}

void native_save_language(int language) {
    pref_set("language", language);
}

/* Automatic dialogues: 0 off, else the speed (1-3) at which a text waiting for the player goes on by itself
   (text_window.c); the button turns it off, x1, x2, x3, off again */
int native_auto_text(void) {
    int v = pref_get("auto_text", 0);
    return v >= 0 && v <= 3 ? v : 0;
}

void native_auto_text_toggle(void) {
    pref_set("auto_text", (native_auto_text() + 1) % 4);
}
