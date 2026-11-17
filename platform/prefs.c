/* The player's choices the game itself doesn't keep, in saves/prefs.txt beside
   the memory card: for now the language, so the language select shows once */
#include <SDL3/SDL.h>
#include <stdio.h>

const char *data_dir(void); /* main.c */

static int s_language = -2; /* -2 not read yet, -1 none */

static void prefs_path(char *out, size_t n) {
    SDL_snprintf(out, n, "%s/../saves/prefs.txt", data_dir());
}

int native_saved_language(void) {
    if (s_language == -2) {
        char path[1100];
        FILE *f;
        int v;

        prefs_path(path, sizeof path);
        s_language = -1;
        if ((f = fopen(path, "r")) != NULL) {
            if (fscanf(f, "language = %d", &v) == 1 && v >= 0 && v <= 6) {
                s_language = v;
            }
            fclose(f);
        }
    }
    return s_language;
}

void native_save_language(int language) {
    char path[1100], dir[1100];
    FILE *f;

    SDL_snprintf(dir, sizeof dir, "%s/../saves", data_dir());
    SDL_CreateDirectory(dir);
    prefs_path(path, sizeof path);
    if ((f = fopen(path, "w")) != NULL) {
        fprintf(f, "language = %d\n", language);
        fclose(f);
        s_language = language;
    }
}
