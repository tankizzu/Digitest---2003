/* Windows crash report (separate from platform.h: windows.h has its own RECT) */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
/* A crash prints the faulting address and the frame-pointer chain (the game
   is built -O0); `llvm-addr2line -fe dw2003.exe <addr>...` names them. The
   same goes to crash.txt next to the exe, for a bug report */
#ifdef _WIN64
#define CRASH_FP Rbp
#define CRASH_IP Rip
#define CRASH_SP Rsp
#else
#define CRASH_FP Ebp
#define CRASH_IP Eip
#define CRASH_SP Esp
#endif
static LONG WINAPI on_crash(EXCEPTION_POINTERS *e) {
    uintptr_t *fp = (uintptr_t *)e->ContextRecord->CRASH_FP;
    char report[1024], path[MAX_PATH + 16];
    int n, i;
    FILE *f;

    n = snprintf(report, sizeof report, "CRASH %08lx at %08llx (access %08llx)\nstack:", e->ExceptionRecord->ExceptionCode,
                 (unsigned long long)e->ContextRecord->CRASH_IP, (unsigned long long)e->ExceptionRecord->ExceptionInformation[1]);
    for (i = 0; i < 24 && fp != NULL && !IsBadReadPtr(fp, 2 * sizeof *fp); i++, fp = (uintptr_t *)fp[0]) {
        n += snprintf(report + n, sizeof report - n, " %08llx", (unsigned long long)fp[1]);
    }
    /* a jump into data breaks the chain: also list the words on the stack that point into the code */
    n += snprintf(report + n, sizeof report - n, "\ncode on stack:");
    fp = (uintptr_t *)e->ContextRecord->CRASH_SP;
    for (i = 0; i < 512 && fp != NULL && !IsBadReadPtr(fp, sizeof *fp) && n < (int)sizeof report - 12; i++, fp++) {
        if (*fp >= 0x401000 && *fp < 0x5c0000) {
            n += snprintf(report + n, sizeof report - n, " %08llx", (unsigned long long)*fp);
        }
    }
    fprintf(stderr, "%s\n", report);
    fflush(stderr);
    GetModuleFileNameA(NULL, path, MAX_PATH);
    strcpy(strrchr(path, '\\') + 1, "crash.txt");
    if ((f = fopen(path, "a")) != NULL) {
        fprintf(f, "%s\n", report);
        fclose(f);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

void install_crash_handler(void) {
    SetUnhandledExceptionFilter(on_crash);
}
#else
/* POSIX (Linux, Android): the faulting address and the frame-pointer chain on stderr */
#include <signal.h>
#include <unistd.h>
static void on_signal(int sig, siginfo_t *info, void *uc) {
    uintptr_t *fp = (uintptr_t *)__builtin_frame_address(0);
    char report[1024];
    int n = snprintf(report, sizeof report, "CRASH signal %d (access %p)\nstack:", sig, info->si_addr);
    (void)uc;
    for (int i = 0; i < 24 && fp != NULL && ((uintptr_t)fp & 7) == 0 && n < (int)sizeof report - 24; i++) {
        uintptr_t *next = (uintptr_t *)fp[0];
        n += snprintf(report + n, sizeof report - n, " %llx", (unsigned long long)fp[1]);
        if (next <= fp || (uintptr_t)next - (uintptr_t)fp > (1 << 20)) {
            break; /* a frame chain goes up the stack, a little at a time */
        }
        fp = next;
    }
    fprintf(stderr, "%s\n", report);
    fflush(stderr);
    signal(sig, SIG_DFL);
    raise(sig);
}

void install_crash_handler(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_signal;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
}
#endif
