/* Prepare player script 9/13 launch slots from the owner's extracted disc.
   The release contains only offsets and expected values, never game archives.
   User data/mods overrides take precedence over these generated copies. */
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#include <SDL3/SDL.h>
#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#else
#include <stdint.h>
/* BCrypt's SHA-256, the only hash used here, for Linux and Android */
typedef void *BCRYPT_ALG_HANDLE;
typedef unsigned long ULONG;
#define BCRYPT_SHA256_ALGORITHM "SHA256"
static const uint32_t K256[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
#define ROR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
static void sha256_block(uint32_t h[8], const uint8_t *p) {
    uint32_t w[64], a, b, c, d, e, f, g, k, t1, t2;
    int i;
    for (i = 0; i < 16; i++) w[i] = (uint32_t)p[i * 4] << 24 | (uint32_t)p[i * 4 + 1] << 16 | (uint32_t)p[i * 4 + 2] << 8 | p[i * 4 + 3];
    for (i = 16; i < 64; i++)
        w[i] = w[i - 16] + (ROR(w[i - 15], 7) ^ ROR(w[i - 15], 18) ^ (w[i - 15] >> 3)) + w[i - 7] +
               (ROR(w[i - 2], 17) ^ ROR(w[i - 2], 19) ^ (w[i - 2] >> 10));
    a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], k = h[7];
    for (i = 0; i < 64; i++) {
        t1 = k + (ROR(e, 6) ^ ROR(e, 11) ^ ROR(e, 25)) + ((e & f) ^ (~e & g)) + K256[i] + w[i];
        t2 = (ROR(a, 2) ^ ROR(a, 13) ^ ROR(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        k = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
    }
    h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += k;
}
static int BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE *alg, const char *id, void *impl, int flags) {
    (void)id, (void)impl, (void)flags;
    *alg = (void *)1;
    return 0;
}
static void BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE alg, int flags) { (void)alg, (void)flags; }
static int BCryptHash(BCRYPT_ALG_HANDLE alg, void *secret, ULONG secret_len, const uint8_t *in, ULONG len, uint8_t *out, ULONG out_len) {
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    uint8_t tail[128] = {0};
    uint64_t bits = (uint64_t)len * 8;
    size_t i, rest = len % 64, n;
    (void)alg, (void)secret, (void)secret_len;
    if (out_len < 32) return -1;
    for (i = 0; i + 64 <= len; i += 64) sha256_block(h, in + i);
    memcpy(tail, in + i, rest);
    tail[rest] = 0x80;
    n = rest < 56 ? 64 : 128;
    for (i = 0; i < 8; i++) tail[n - 1 - i] = (uint8_t)(bits >> (i * 8));
    sha256_block(h, tail);
    if (n == 128) sha256_block(h, tail + 64);
    for (i = 0; i < 8; i++) out[i * 4] = h[i] >> 24, out[i * 4 + 1] = h[i] >> 16, out[i * 4 + 2] = h[i] >> 8, out[i * 4 + 3] = h[i];
    return 0;
}
#endif
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "launch_slots.h"
#include "launch_manifest.h"

const char *data_dir(void);
void native_log(const char *fmt, ...);

static int s_ready;

int native_launch_archives_ready(void) {
    return s_ready;
}

int native_launch_archive_listed(int file_id) {
    for (int i = 0; i < NATIVE_LAUNCH_ARCHIVE_COUNT; i++) {
        if (NATIVE_LAUNCH_ARCHIVES[i].file_id == file_id) return 1;
    }
    return 0;
}

int native_launch_archive_matches(int archive) {
    unsigned int file_id = (unsigned int)archive >> 16;
    unsigned int entry = (unsigned int)archive & 0xffff;

    for (int i = 0; i < NATIVE_LAUNCH_ARCHIVE_COUNT; i++) {
        if (NATIVE_LAUNCH_ARCHIVES[i].file_id == file_id &&
            NATIVE_LAUNCH_ARCHIVES[i].archive_entry == entry) return 1;
    }
    return 0;
}

static int16_t word(const uint8_t *bytes, uint32_t at) {
    return (int16_t)(bytes[at] | ((uint16_t)bytes[at + 1] << 8));
}

static void put_word(uint8_t *bytes, uint32_t at, int16_t value) {
    bytes[at] = (uint8_t)value;
    bytes[at + 1] = (uint8_t)((uint16_t)value >> 8);
}

static int mark(uint8_t *bytes, const NativeLaunchScript *script,
                uint32_t at, int opcode, int mode, int original) {
    uint32_t operand = at + (mode < 0 ? 2 : 4);
    if (at < script->start || operand + 2 > script->end ||
        word(bytes, at) != opcode ||
        (mode >= 0 && word(bytes, at + 2) != mode) ||
        word(bytes, operand) != original) return 0;
    put_word(bytes, operand, (int16_t)-original);
    return 1;
}

static int edit_script(uint8_t *bytes, size_t size, const NativeLaunchScript *script) {
    uint32_t first, last, wait_at;
    uint8_t wait_bytes[4];
    int i;

    if (script->start >= script->end || script->end > size ||
        script->spawn_count < 1 || script->spawn_count > NATIVE_LAUNCH_SPAWNS ||
        !mark(bytes, script, script->model_preload, 6, 1, script->model) ||
        !mark(bytes, script, script->sprite_preload, 3, 1, script->sprite)) return 0;
    first = script->spawns[0].model_offset;
    for (i = 0; i < script->spawn_count; i++) {
        const NativeLaunchSpawn *spawn = &script->spawns[i];
        if (spawn->model_offset != first + 30u * i ||
            spawn->sprite_offset != spawn->model_offset + 18 ||
            !mark(bytes, script, spawn->model_offset, 6, 0, script->model) ||
            !mark(bytes, script, spawn->sprite_offset, 3, 0, script->sprite)) return 0;
    }
    if (!mark(bytes, script, script->sound_offset, 10, -1, script->sound) ||
        script->sound_offset + 6 > script->end ||
        word(bytes, script->sound_offset + 4) != script->duration) return 0;
    last = script->spawns[script->spawn_count - 1].sprite_offset;
    wait_at = script->wait_offset;
    if (wait_at == NATIVE_NO_WAIT) {
        return script->spawn_count == 1 && script->sound_offset == first + 30;
    }
    if (first < script->start || wait_at + 4 != script->sound_offset ||
        wait_at - first != 30u * script->spawn_count ||
        last + 12 != wait_at ||
        word(bytes, wait_at) != 11 ||
        word(bytes, wait_at + 2) != script->wait_frames ||
        script->wait_frames <= 0) return 0;

    memcpy(wait_bytes, bytes + wait_at, sizeof wait_bytes);
    memmove(bytes + first + 4, bytes + first, wait_at - first);
    memcpy(bytes + first, wait_bytes, sizeof wait_bytes);
    put_word(bytes, first + 2, (int16_t)-script->wait_frames);
    return 1;
}

static uint8_t *read_original(int file_id, size_t *size) {
    char path[1200];
    FILE *file;
    long length;
    uint8_t *bytes;

    snprintf(path, sizeof path, "%s/files/%04d.bin", data_dir(), file_id);
    file = fopen(path, "rb");
    if (file == NULL) return NULL;
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) <= 0 ||
        length > 1024 * 1024 || fseek(file, 0, SEEK_SET) != 0 ||
        (bytes = malloc((size_t)length)) == NULL) {
        fclose(file);
        return NULL;
    }
    if (fread(bytes, 1, (size_t)length, file) != (size_t)length) {
        free(bytes);
        fclose(file);
        return NULL;
    }
    fclose(file);
    *size = (size_t)length;
    return bytes;
}

/* Recheck three megabytes on startup so new builds and interrupted writes
   repair themselves; unchanged files are not written again. */
static int write_if_changed(int file_id, const uint8_t *bytes, size_t size) {
    char path[1200], temp[1210];
    uint8_t chunk[4096];
    FILE *file;
    size_t offset = 0;
    int same = 1;

    snprintf(path, sizeof path, "%s/generated/%04d.bin", data_dir(), file_id);
    file = fopen(path, "rb");
    if (file != NULL) {
        while (offset < size) {
            size_t count = size - offset < sizeof chunk ? size - offset : sizeof chunk;
            if (fread(chunk, 1, count, file) != count ||
                memcmp(chunk, bytes + offset, count) != 0) {
                same = 0;
                break;
            }
            offset += count;
        }
        if (same && fgetc(file) != EOF) same = 0;
        fclose(file);
        if (same) return 1;
    }
    snprintf(temp, sizeof temp, "%s.tmp", path);
    file = fopen(temp, "wb");
    if (file == NULL) return 0;
    size_t written = fwrite(bytes, 1, size, file);
    int closed = fclose(file);
    if (written != size || closed != 0) {
        SDL_RemovePath(temp);
        return 0;
    }
    if (!SDL_RenamePath(temp, path)) {
        SDL_RemovePath(temp);
        return 0;
    }
    return 1;
}

int native_prepare_launch_archives(void) {
    BCRYPT_ALG_HANDLE algorithm = NULL;
    char dir[1200];
    int i;

    s_ready = 0;
    snprintf(dir, sizeof dir, "%s/generated", data_dir());
    if (!SDL_CreateDirectory(dir) ||
        BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, NULL, 0) < 0) {
        native_log("technique archives: cannot initialize local generation");
        return 0;
    }
    for (i = 0; i < NATIVE_LAUNCH_ARCHIVE_COUNT; i++) {
        const NativeLaunchArchive *entry = &NATIVE_LAUNCH_ARCHIVES[i];
        uint8_t digest[32];
        size_t size = 0;
        uint8_t *bytes = read_original(entry->file_id, &size);
        int good = bytes != NULL &&
                   BCryptHash(algorithm, NULL, 0, bytes, (ULONG)size,
                              digest, sizeof digest) >= 0 &&
                   memcmp(digest, entry->source_sha256, sizeof digest) == 0 &&
                   edit_script(bytes, size, &entry->scripts[0]) &&
                   edit_script(bytes, size, &entry->scripts[1]) &&
                   write_if_changed(entry->file_id, bytes, size);
        free(bytes);
        if (!good) {
            native_log("technique archives: could not prepare file %04d from this disc",
                       entry->file_id);
            BCryptCloseAlgorithmProvider(algorithm, 0);
            return 0;
        }
    }
    BCryptCloseAlgorithmProvider(algorithm, 0);
    s_ready = 1;
    return 1;
}
