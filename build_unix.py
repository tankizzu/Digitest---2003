"""Builds the native port for Linux (x86_64) or Android (arm64), reusing
build.py's game-code rewrites (patches, widened returns, MIPS divisions,
overlay sections, stage prefixes).

    python build_unix.py                  Linux x86_64, into build-linux/
    python build_unix.py --android        Android arm64 shared library, into build-android/
    python build_unix.py --clean          from scratch

Environment: SDL3_PREFIX (an SDL3 install: include/ and lib/), ANDROID_NDK
for --android.
"""
import collections
import concurrent.futures as cf
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ANDROID = "--android" in sys.argv
GLES = ANDROID or "--gles" in sys.argv  # OpenGL ES 3.0, as on Android; --gles tries it on Linux

# build.py looks for llvm-mingw at import: point it anywhere, its tools are replaced below
os.environ.setdefault("LLVM_MINGW", str(HERE))
sys.argv_saved = sys.argv
sys.argv = [sys.argv[0]] + (["--x64"])  # the 64-bit paths of build.py (no cdecl underscores)
sys.path.insert(0, str(HERE))
import build as B  # noqa: E402
sys.argv = sys.argv_saved

if ANDROID:
    NDK = Path(os.environ["ANDROID_NDK"])
    TC = NDK / "toolchains/llvm/prebuilt/linux-x86_64/bin"
    API = os.environ.get("ANDROID_API", "26")
    CC = TC / f"aarch64-linux-android{API}-clang"
    NM, OBJCOPY = TC / "llvm-nm", TC / "llvm-objcopy"
    OUT = B.NATIVE / "build-android"
else:
    CC = Path(shutil.which("clang"))
    NM = Path(shutil.which("llvm-nm-18") or shutil.which("llvm-nm"))
    OBJCOPY = Path(shutil.which("llvm-objcopy-18") or shutil.which("llvm-objcopy"))
    OUT = B.NATIVE / ("build-linux-gles" if GLES else "build-linux")
SDL = Path(os.environ.get("SDL3_PREFIX", B.NATIVE.parent / ("sdl3-android" if ANDROID else "sdl3-linux")))

PIC = ["-fPIC"] if ANDROID else ["-fno-pic", "-fno-pie"]
# the game's flags, as build.py has them, for this compiler
B.CC, B.NM, B.OBJCOPY, B.OUT, B.PATCHED = CC, NM, OBJCOPY, OUT, OUT / "patched"
B.GAME_CFLAGS = [f.replace(str(B.NATIVE / "build-x64"), str(OUT)) for f in B.GAME_CFLAGS] + PIC + ["-fno-stack-protector",
    "-Dlong=int"]  # the PSX's long is 32 bits, as on Windows (LLP64); Linux and Android are LP64 (patches/include/common.h)
# patched shared .inc.c files (patches/src/shared) before the decomp's: they are found through -I, not beside their includer
B.GAME_CFLAGS.insert(B.GAME_CFLAGS.index(f"-I{B.DECOMP / 'src'}"), f"-I{OUT / 'patched' / 'src'}")
B.GAME_OPT = B.GAME_OPT + PIC
B.SAN_FLAGS = []
PLATFORM_CFLAGS = ["-std=gnu11", "-c", "-O2", "-g", "-Wall", "-Wno-unused-parameter", "-Wno-unused-function",
                   f"-I{SDL / 'include'}", f"-I{OUT}", *PIC, "-D_GNU_SOURCE", *(["-DDW3_GLES"] if GLES else []),
                   "-include", "stdlib.h", "-include", "string.h", "-include", "stdio.h",
                   "-include", "stddef.h", "-include", str(B.NATIVE / "platform/posix_compat.h")]


def run(cmd, **kw):
    return subprocess.run([str(c) for c in cmd], capture_output=True, text=True, **kw)


def host_symbols():
    if ANDROID:
        sysroot = TC.parent / "sysroot/usr/lib/aarch64-linux-android" / API
        libs = [sysroot / f"lib{n}.so" for n in ("c", "m", "dl", "log", "android", "GLESv3", "EGL")]
    else:
        libs = [Path(p) for p in ("/lib/x86_64-linux-gnu/libc.so.6", "/lib/x86_64-linux-gnu/libm.so.6")]
    out = run([NM, "-D", "--format=just-symbols", "--defined-only", *[l for l in libs if l.exists()]]).stdout.split()
    return {s.split("@")[0] for s in out}


def main():
    clean = "--clean" in sys.argv
    if clean and OUT.exists():
        shutil.rmtree(OUT, ignore_errors=True)
    OUT.mkdir(parents=True, exist_ok=True)
    manifest = OUT / "launch_manifest.h"
    r = run([sys.executable, B.NATIVE / "tools/generate_launch_manifest.py", manifest])
    if r.returncode != 0:
        sys.exit(f"launch inventory failed:\n{r.stdout}{r.stderr}")
    B.apply_patches()
    smaa = (B.NATIVE / "platform/smaa/SMAA.hlsl").read_bytes()
    (OUT / "smaa_src.inc").write_text(",".join(str(b) for b in smaa) + "\n")

    srcs = B.game_sources()
    stale_stages = set()
    with cf.ThreadPoolExecutor(os.cpu_count()) as ex:
        results = list(ex.map(lambda s: B.compile_game(s, clean), srcs))
    for rel, obj, err, built in results:
        m = B.STAGE.match(rel.name)
        if built and m:
            stale_stages.add(m.group(1))
    redo = [s for s in srcs if (m := B.STAGE.match(s.name)) and m.group(1) in stale_stages]
    with cf.ThreadPoolExecutor(os.cpu_count()) as ex:
        again = {r[0]: r for r in ex.map(lambda s: B.compile_game(s, True), redo)}
    results = [again.get(r[0], r) for r in results]
    B.prefix_stages([obj for rel, obj, err, built in results if obj is not None and B.STAGE.match(rel.name)
                     and B.STAGE.match(rel.name).group(1) in stale_stages])
    failed = [(rel, err) for rel, obj, err, _ in results if obj is None]
    for rel, err in failed[:20]:
        print(f"FAILED {rel}\n{err[:1500]}")
    if failed:
        sys.exit(f"{len(failed)} game files failed")

    platform = []
    for src in sorted((B.NATIVE / "platform").glob("*.c")) + sorted((B.NATIVE / "platform/gl").glob("*.c")):
        rel = src.relative_to(B.NATIVE / "platform")
        obj = OUT / "platform" / rel.with_suffix(".o")
        obj.parent.mkdir(parents=True, exist_ok=True)
        deps = [src, *(B.NATIVE / "platform").glob("*.h"), *src.parent.glob("*.h"), Path(__file__)]
        if clean or not obj.exists() or obj.stat().st_mtime < max(h.stat().st_mtime for h in deps):
            r = run([CC, *PLATFORM_CFLAGS, src, "-o", obj])
            if r.returncode != 0:
                sys.exit(f"FAILED {rel}\n{r.stderr[:6000]}")
        platform.append(obj)

    stage_list = OUT / "stage_list.c"
    names = sorted(p.stem for p in (OUT / "obj").glob("wstag*.syms") if re.search(r"^_?setupStage ", p.read_text(), re.M))
    with open(stage_list, "w") as f:
        f.writelines(f"void {n.upper()}_setupStage(void); void {n.upper()}_startStage(void);\n" for n in names)
        f.write("struct { const char *name; void (*setup)(void); void *start; } native_stage_list[] = {\n")
        f.writelines(f'    {{"{n}", {n.upper()}_setupStage, (void *){n.upper()}_startStage}},\n' for n in names)
        f.write("    {0, 0, 0},\n};\n")
    r = run([CC, *PLATFORM_CFLAGS, stage_list, "-o", OUT / "stage_list.o"])
    if r.returncode != 0:
        sys.exit(r.stderr)
    platform.append(OUT / "stage_list.o")

    game_objs = [obj for _, obj, _, _ in results]
    defined, undefined = set(), collections.defaultdict(int)
    with cf.ThreadPoolExecutor(os.cpu_count()) as ex:
        syms = list(ex.map(B.symbols, game_objs + platform))
    for d, u in syms:
        defined |= d
        for s in u:
            undefined[s] += 1
    host = host_symbols()
    missing = sorted(s for s in undefined if s not in defined and s not in host and not s.startswith(("SDL_", "_", "gl", "egl")))
    stubs = OUT / "stubs.c"
    with open(stubs, "w") as f:
        f.write('void log_once(const char *what);\n')
        for n in missing:
            f.write(f'int {n}(void) {{ log_once("stub: {n}"); return 0; }}\n')
    r = run([CC, *PLATFORM_CFLAGS, stubs, "-o", OUT / "stubs.o"])
    if r.returncode != 0:
        sys.exit(r.stderr)

    # build.py's .data$dw3<group><n> sections, in name order, as the PE linker sorts them (platform/memory.c, state.c)
    lds = OUT / "dw3.lds"
    lds.write_text("SECTIONS {\n  .dw3data : { KEEP(*(SORT_BY_NAME(.data$dw3*))) }\n} INSERT AFTER .data;\n")
    if ANDROID:  # the heap (platform/memory.c) before the code: it has to end below 16 MB (android/loader.c)
        lds.write_text(lds.read_text() + "SECTIONS {\n  .dw3heap (NOLOAD) : { *(.bss.dw3heap) }\n} INSERT BEFORE .text;\n")
    rsp = OUT / "objs.rsp"
    rsp.write_text("\n".join(f'"{o}"' for o in game_objs + platform + [OUT / "stubs.o"]))
    if ANDROID:
        out = OUT / "libdw3.so"
        ld = ["-shared", "-fuse-ld=lld", f"-Wl,-T,{lds}", f"-L{SDL / 'lib'}", "-lSDL3", "-lGLESv3", "-lEGL", "-llog", "-landroid", "-lm",
              "-Wl,--no-undefined", "-Wl,-z,max-page-size=16384", "-Wl,-soname,libdw3.so"]
    else:
        out = OUT / "dw2003"
        ld = ["-no-pie", "-fuse-ld=lld", f"-Wl,-T,{lds}", f"-L{SDL / 'lib'}", f"-Wl,-rpath,{SDL / 'lib'}", "-lSDL3", *(["-lGLESv2"] if GLES else []), "-lm", "-lpthread", "-ldl"]
    r = run([CC, f"@{rsp}", "-o", out, *ld])
    if r.returncode != 0:
        sys.exit(f"link failed:\n{r.stderr[:6000]}")
    shutil.copy2(B.NATIVE / "config/technique_visuals.ini", OUT / "technique_visuals.ini")
    if ANDROID:  # android/loader.c: libmain.so, which loads libdw3.so low
        r = run([CC, "-std=gnu11", "-O2", "-g", "-fPIC", "-shared", f"-I{SDL / 'include'}", B.NATIVE / "android/loader.c",
                 "-o", OUT / "libmain.so", f"-L{SDL / 'lib'}", "-lSDL3", "-ldl", "-llog", "-fuse-ld=lld",
                 "-Wl,--no-undefined", "-Wl,-z,max-page-size=16384"])
        if r.returncode != 0:
            sys.exit(f"loader failed:\n{r.stderr[:4000]}")
    (OUT / "report.md").write_text("# Stubbed\n\n" + "".join(f"- {n}\n" for n in missing))
    print(f"OK {out} ({len(missing)} stubs)")


if __name__ == "__main__":
    main()
