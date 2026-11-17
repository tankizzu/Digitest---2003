/* The memory card screen's texts (STGMCARD's MEMCRD files) as the games
   (games.c) want them: one MEMORY CARD slot for each game, "MEMORY CARD slot N", and three
   texts added for Continue's choice between the autosave and the saves
   (SAVE_TEXT_ASK, _AUTO, _SAVES). Italian and English are rewritten; the
   other languages get the three texts in English.

   The European texts are 1-byte: uppercase ASCII - 0x33, lowercase - 0x39,
   digits - 0x2C, 01 01 a space, E6 the full stop, 02 01 a new line, 02 05 01
   the number the window was given (setNumber, buffer 1), 00 the end. */
#include <SDL3/SDL.h>
#include <stdint.h>
#include <string.h>

long native_read_raw(int fid, uint8_t **out); /* files.c */

#define FID_EN 122 /* ESMEMCRD, LANGUAGE 2 */
#define FID_IT 124 /* ISMEMCRD, LANGUAGE 4 */
#define FID_FIRST 120
#define FID_LAST 126
#define ORIGINAL_COUNT 41
#define TEXT_COUNT 51

typedef struct {
    int id;
    const char *text; /* plain text: '\n' a new line, '#' the number, '|' a raw byte as two hex digits after it */
} Text;

/* Italian */
static const Text IT[] = {
    {0x03, "MEMORY CARD #"},
    {0x04, "Controllo MEMORY CARD\nAttendi"},
    {0x06, "Vuoi salvare qui."},              /* without the PlayStation buttons' line (touch: the Yes / No boxes) */
    {0x10, "Vuoi caricare questa partita."},
    {0x05, "Controllo del file\nAttendi"},
    {0x08, "Salvataggio\nAttendi"},
    {0x0A, "Nessuna MEMORY CARD salvata"},
    {0x0B, "Slot MEMORY CARD vuoto"},
    {0x0C, "Nessun salvataggio\nin questo slot"},
    {0x0D, "Spazio insufficiente"},
    {0x11, "Caricamento\nAttendi"},
    {0x19, "Preparare la partita."}, /* formatting a new game's card; the yes / no in the boxes (the line dropped) */
    {0x1B, "Slot MEMORY CARD non disponibile"},
    {0x1D, "Scegli lo slot MEMORY CARD"},
    {0x1E, "Preparazione\nAttendi"},
    {0x1F, "Salvataggio\nAttendi"},
    {0x26, "Errore di salvataggio"},
    {0x28, "MEMORY CARD #"},
    {0x29, "Da dove vuoi riprendere."},
    {0x2A, "Automatico"},
    {0x2B, "Salvataggi"},
    {0x2C, "Eliminare MEMORY CARD #."},
    {0x2D, "Elimina"},
    {0x2E, "Scegli lo slot da sostituire"},
    {0x2F, "Sostituire MEMORY CARD #."},
    {0x30, "Carica"},
    {0x31, "Dai un nome alla MEMORY CARD"},
    {0x32, "OK"},
    {0, NULL},
};

/* English */
static const Text EN[] = {
    {0x03, "MEMORY CARD #"},
    {0x04, "Checking MEMORY CARD\nPlease wait"},
    {0x06, "Save here."},
    {0x10, "Load this game."},
    {0x05, "Checking file\nPlease wait"},
    {0x08, "Saving\nPlease wait"},
    {0x0A, "No saved MEMORY CARD"},
    {0x0B, "Empty MEMORY CARD slot"},
    {0x0C, "No saves\nin this slot"},
    {0x0D, "Not enough space"},
    {0x11, "Loading\nPlease wait"},
    {0x19, "Prepare the game."},
    {0x1B, "MEMORY CARD slot not available"},
    {0x1D, "Choose MEMORY CARD slot"},
    {0x1E, "Preparing\nPlease wait"},
    {0x1F, "Saving\nPlease wait"},
    {0x26, "Save error"},
    {0x28, "MEMORY CARD #"},
    {0x29, "Continue from where."},
    {0x2A, "Autosave"},
    {0x2B, "Saves"},
    {0x2C, "Delete MEMORY CARD #."},
    {0x2D, "Delete"},
    {0x2E, "Choose a slot to replace"},
    {0x2F, "Replace MEMORY CARD #."},
    {0x30, "Load"},
    {0x31, "Name your MEMORY CARD"},
    {0x32, "OK"},
    {0, NULL},
};

/* Plain text to the game's bytes, with its 00: the length written (games.c uses it too) */
int native_text_encode(const char *s, uint8_t *out, int room);
int native_text_encode(const char *s, uint8_t *out, int room) {
    int n = 0;

    for (; *s != 0 && n < room - 3; s++) {
        char c = *s;
        if (c >= 'A' && c <= 'Z') {
            out[n++] = (uint8_t)(c - 0x33);
        } else if (c >= 'a' && c <= 'z') {
            out[n++] = (uint8_t)(c - 0x39);
        } else if (c >= '0' && c <= '9') {
            out[n++] = (uint8_t)(c - 0x2C);
        } else if (c == ' ') {
            out[n++] = 1, out[n++] = 1;
        } else if (c == '.') {
            out[n++] = 0xE6;
        } else if (c == '\n') {
            out[n++] = 2, out[n++] = 1;
        } else if (c == '#') {
            out[n++] = 2, out[n++] = 5, out[n++] = 1;
        } else if (c == '|' && s[1] != 0 && s[2] != 0) {
            char hex[3] = {s[1], s[2], 0};
            out[n++] = (uint8_t)SDL_strtol(hex, NULL, 16);
            s += 2;
        }
    }
    out[n++] = 0;
    return n;
}

static uint8_t *s_made[FID_LAST - FID_FIRST + 1];
static long s_size[FID_LAST - FID_FIRST + 1];

/* The table rebuilt: the original strings but those `texts` gives, then the added ones */
static uint8_t *rebuild(const uint8_t *orig, long orig_size, const Text *texts, uint32_t original_count, int text_count, long *size) {
    uint8_t *out = SDL_calloc(1, (size_t)orig_size + 4096);
    uint32_t count, off;
    int pos;

    if (out == NULL || orig_size < 8) {
        SDL_free(out);
        return NULL;
    }
    SDL_memcpy(&count, orig, 4);
    if (count != original_count) {
        SDL_free(out);
        return NULL; /* not the file this was made for */
    }
    count = (uint32_t)text_count;
    SDL_memcpy(out, &count, 4);
    pos = 4 + 4 * text_count;
    for (int i = 0; i < text_count; i++) {
        const Text *t = texts;

        while (t->text != NULL && t->id != i) {
            t++;
        }
        off = (uint32_t)pos;
        SDL_memcpy(out + 4 + 4 * i, &off, 4);
        if (t->text != NULL) {
            pos += native_text_encode(t->text, out + pos, (int)(orig_size + 4096 - pos));
        } else if (i < (int)original_count) {
            uint32_t from;
            int len;

            SDL_memcpy(&from, orig + 4 + 4 * i, 4);
            for (len = 0; from + len < (uint32_t)orig_size && orig[from + len] != 0; len++) {
            }
            SDL_memcpy(out + pos, orig + from, (size_t)len);
            pos += len;
            out[pos++] = 0;
        } else {
            out[pos++] = 0;
        }
    }
    *size = pos;
    return out;
}

/* The field menu's texts (STATUS, STSTATUS's and the field menu's): its hints without the PlayStation buttons (touch:
   the back arrow goes back, the menu's own rows sort). Italian and English; the others as they are */
#define STATUS_EN 178
#define STATUS_IT 180
#define STATUS_COUNT 108

static const Text STATUS_IT_TEXTS[] = {
    {0x0F, "Cambia formazione Digimon"},
    {0x10, "Non puoi cambiare formazione"},
    {0x13, "Menu"},
    {0x15, "Menu"},
    {0x21, "Oggetti"},
    {0, NULL},
};

static const Text STATUS_EN_TEXTS[] = {
    {0x0F, "Change Digimon formation"},
    {0x10, "Can not change formation"},
    {0x13, "Menu"},
    {0x15, "Menu"},
    {0x21, "Items"},
    {0, NULL},
};

static uint8_t *s_status[2];
static long s_status_size[2];

static const uint8_t *status_file(int fid, long *size) {
    int i = fid == STATUS_IT;

    if (s_status[i] == NULL) {
        uint8_t *orig = NULL;
        long n = native_read_raw(fid, &orig);

        if (n <= 0) {
            return NULL;
        }
        s_status[i] = rebuild(orig, n, i ? STATUS_IT_TEXTS : STATUS_EN_TEXTS, STATUS_COUNT, STATUS_COUNT, &s_status_size[i]);
        free(orig);
        if (s_status[i] == NULL) {
            return NULL;
        }
    }
    *size = s_status_size[i];
    return s_status[i];
}

/* made_file's (mods.c): the rewritten MEMCRD and STATUS files, or NULL for any other file */
const uint8_t *savetexts_file(int fid, long *size) {
    int i = fid - FID_FIRST;

    if (fid == STATUS_EN || fid == STATUS_IT) {
        return status_file(fid, size);
    }
    if (fid < FID_FIRST || fid > FID_LAST) {
        return NULL;
    }
    if (s_made[i] == NULL) {
        uint8_t *orig = NULL;
        long n = native_read_raw(fid, &orig);
        static const Text ADDED_ONLY[] = {{0x29, "Continue from where."}, {0x2A, "Autosave"}, {0x2B, "Saves"}, {0x2C, "Delete MEMORY CARD #."}, {0x2D, "Delete"}, {0x2E, "Choose a slot to replace"}, {0x2F, "Replace MEMORY CARD #."}, {0x30, "Load"}, {0x31, "Name your MEMORY CARD"}, {0x32, "OK"}, {0, NULL}};

        if (n <= 0) {
            return NULL;
        }
        s_made[i] = rebuild(orig, n, fid == FID_IT ? IT : fid == FID_EN ? EN : ADDED_ONLY, ORIGINAL_COUNT, TEXT_COUNT, &s_size[i]);
        free(orig); /* native_read_raw's calloc */
    }
    *size = s_size[i];
    return s_made[i];
}
