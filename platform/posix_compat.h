/* Linux and Android: the few MSVCRT names the platform layer uses, mapped to
   their POSIX equivalents. Force-included by build_unix.py, never on Windows. */
#ifndef POSIX_COMPAT_H
#define POSIX_COMPAT_H
#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#define _fseeki64 fseeko
#define _ftelli64 ftello
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#ifndef MAX_PATH
#define MAX_PATH 4096
#endif
#endif
#endif
