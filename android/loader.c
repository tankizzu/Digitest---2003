/* libmain.so, the library SDL's activity starts: loads the game (libdw3.so) at a
   low address, below 16 MB for its heap, as the game keeps addresses in 32-bit
   variables and the GPU's ordering tables in 24 bits. Reads the player's disc
   on the first run. */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <android/dlext.h>
#include <android/log.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif

#define TITLE "Digimon World 2003"
/* Where the library may go: its heap starts ~4 MB in and takes 6 MB (build_unix.py puts it before the code),
   and has to end below 16 MB; the whole library takes ~20 MB */
#define LOW_FIRST ((uintptr_t)0x10000)
#define LOW_LAST ((uintptr_t)0x600000)
#define LOW_STEP ((uintptr_t)0x10000)
#define LOW_SIZE ((size_t)24 << 20)
#define HEAP_SIZE (6 * 1024 * 1024) /* include/native.h's NATIVE_HEAP_SIZE */

static void say(SDL_MessageBoxFlags kind, const char *text) {
    __android_log_print(ANDROID_LOG_INFO, "dw2003", "%s", text);
    SDL_ShowSimpleMessageBox(kind, TITLE, text, NULL);
}

/* The file picker's answer */
static SDL_AtomicInt s_picked; /* 0 waiting, 1 a file, 2 none */
static char s_uri[2048];

static void on_pick(void *userdata, const char *const *files, int filter) {
    (void)userdata, (void)filter;
    if (files != NULL && files[0] != NULL) {
        SDL_strlcpy(s_uri, files[0], sizeof s_uri);
        SDL_SetAtomicInt(&s_picked, 1);
    } else {
        SDL_SetAtomicInt(&s_picked, 2);
    }
}

static int s_last_percent = -1;
static int on_progress(int percent) {
    if (percent / 10 != s_last_percent / 10) {
        __android_log_print(ANDROID_LOG_INFO, "dw2003", "extract: %d%%", percent);
        s_last_percent = percent;
    }
    return 1;
}

/* The first run: the player picks their .bin, the game reads it into data/. 1 when the game is there. */
static int first_run(void *game, const char *data) {
    int (*extract)(const char *, const char *, int (*)(int)) = (int (*)(const char *, const char *, int (*)(int)))dlsym(game, "native_extract");
    const char *(*error)(int) = (const char *(*)(int))dlsym(game, "native_extract_error");
    char text[512];
    SDL_Event e;
    int result;

    if (extract == NULL || !SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }
    for (;;) {
        say(SDL_MESSAGEBOX_INFORMATION,
            "Benvenuto!\n\nPer giocare serve il tuo disco di Digimon World 2003 (versione europea).\n\n"
            "Nella schermata che si apre scegli il file .bin del disco (circa 660 MB). "
            "Il gioco lo legge una volta sola: dopo potrai cancellarlo dal telefono.");
        SDL_SetAtomicInt(&s_picked, 0);
        SDL_ShowOpenFileDialog(on_pick, NULL, NULL, NULL, 0, NULL, false);
        while (SDL_GetAtomicInt(&s_picked) == 0) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT) {
                    return 0;
                }
            }
            SDL_Delay(50);
        }
        if (SDL_GetAtomicInt(&s_picked) == 2) {
            say(SDL_MESSAGEBOX_WARNING, "Nessun file scelto. Senza il disco il gioco non può partire: riprova.");
            continue;
        }
        say(SDL_MESSAGEBOX_INFORMATION, "Ora leggo il disco: ci vuole circa un minuto.\nPremi OK e attendi senza chiudere l'app.");
        result = extract(s_uri, data, on_progress);
        if (result == 0) {
            say(SDL_MESSAGEBOX_INFORMATION, "Fatto! Il gioco è pronto.");
            SDL_Quit();
            return 1;
        }
        SDL_snprintf(text, sizeof text,
                     "Non sono riuscito a leggere il disco (%s).\n\n"
                     "Serve il file .bin di Digimon World 2003 (Europe), di 692.146.560 byte, e circa 900 MB liberi.",
                     error != NULL ? error(result) : "?");
        say(SDL_MESSAGEBOX_ERROR, text);
    }
}

int main(int argc, char *argv[]) {
    const char *base = SDL_GetAndroidExternalStoragePath();
    char data[1024], log[1024], exe[1100];
    android_dlextinfo info;
    void *low, *game;
    int (*game_main)(int, char **);

    if (base == NULL) {
        base = SDL_GetAndroidInternalStoragePath();
    }
    /* the game's log (stderr) to a file the player can send */
    SDL_snprintf(log, sizeof log, "%s/log.txt", base);
    freopen(log, "w", stderr);
    setvbuf(stderr, NULL, _IOLBF, 0);
    SDL_snprintf(data, sizeof data, "%s/data", base);
    setenv("DW3_DATA", data, 1);

    /* the first room free low in memory: MAP_FIXED_NOREPLACE where the kernel has it, a plain hint where not */
    low = NULL;
    {
        uintptr_t at;
        int first_errno = 0;
        void *first_got = NULL;

        for (at = LOW_FIRST; at <= LOW_LAST && low == NULL; at += LOW_STEP) {
            void *got = mmap((void *)at, LOW_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED_NOREPLACE, -1, 0);

            if (got == (void *)at) {
                low = got;
            } else {
                if (at == LOW_FIRST) {
                    first_errno = errno, first_got = got;
                }
                if (got != MAP_FAILED) {
                    munmap(got, LOW_SIZE);
                }
            }
        }
        if (low == NULL) {
            char text[400];
            FILE *maps = fopen("/proc/self/maps", "r");
            char line[256];

            fprintf(stderr, "loader: no room low in memory (first try: %p, errno %d); the map:\n", first_got, first_errno);
            for (int n = 0; maps != NULL && fgets(line, sizeof line, maps) != NULL && n < 12; n++) {
                fputs(line, stderr);
            }
            if (maps != NULL) {
                fclose(maps);
            }
            SDL_snprintf(text, sizeof text,
                         "Questo telefono non lascia al gioco la memoria che gli serve (indirizzi bassi).\n\n"
                         "Codice: %p / %d. Mandalo a chi sviluppa il gioco, con il file log.txt se puoi.",
                         first_got, first_errno);
            say(SDL_MESSAGEBOX_ERROR, text);
            return 1;
        }
    }
    SDL_zero(info);
    info.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
    info.reserved_addr = low;
    info.reserved_size = LOW_SIZE;
    game = android_dlopen_ext("libdw3.so", RTLD_NOW | RTLD_GLOBAL, &info);
    if (game == NULL || (game_main = (int (*)(int, char **))dlsym(game, "SDL_main")) == NULL) {
        fprintf(stderr, "loader: %s\n", dlerror());
        say(SDL_MESSAGEBOX_ERROR, "Non riesco a caricare il gioco. Manda il file log.txt.");
        return 1;
    }
    {
        uint8_t *heap = (uint8_t *)dlsym(game, "HEAP_BASE");

        fprintf(stderr, "loader: game at %p, heap at %p\n", low, (void *)heap);
        if (heap == NULL || (uintptr_t)heap + HEAP_SIZE > 0x01000000) {
            say(SDL_MESSAGEBOX_ERROR, "Il gioco è stato caricato troppo in alto in memoria. Manda il file log.txt.");
            return 1;
        }
    }
    SDL_snprintf(exe, sizeof exe, "%s/SLES_039.36", data);
    if (access(exe, R_OK) != 0 && !first_run(game, data)) {
        return 0;
    }
    return game_main(argc, argv);
}
