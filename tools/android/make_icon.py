"""The app's launcher icon: an original pixel-art drawing (a digital egg glowing orange and yellow on a violet-blue
field of binary digits; no character, logo or title of the game), drawn on a 32x32 grid and scaled up without
smoothing, into android/app/src/main/res/mipmap-*/ic_launcher.png

    python3 tools/android/make_icon.py
"""
import math
from pathlib import Path
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[2]
SIZES = {"mdpi": 48, "hdpi": 72, "xhdpi": 96, "xxhdpi": 144, "xxxhdpi": 192}
N = 32

# 3x5 digits for the binary rain
DIGITS = {"0": ["111", "101", "101", "101", "111"], "1": ["010", "110", "010", "010", "111"]}


def lerp(a, b, t):
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(3))


def draw():
    im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
    p = im.load()
    # the field: violet at the top to blue below, a lighter ring around the middle
    for y in range(N):
        for x in range(N):
            d = math.hypot(x - 15.5, y - 15.5)
            c = lerp((70, 40, 150), (20, 60, 170), y / (N - 1))
            if 12.5 < d < 14.5:
                c = lerp(c, (140, 170, 255), 0.45)
            p[x, y] = c + (255,)
    # binary digits, faint
    for (x0, y0, s) in ((1, 2, "10"), (24, 3, "01"), (1, 24, "01"), (25, 25, "10"), (2, 13, "1"), (28, 14, "0")):
        for k, ch in enumerate(s):
            for j, row in enumerate(DIGITS[ch]):
                for i, bit in enumerate(row):
                    x, y = x0 + k * 4 + i, y0 + j
                    if bit == "1" and 0 <= x < N and 0 <= y < N:
                        p[x, y] = lerp(p[x, y][:3], (120, 230, 170), 0.55) + (255,)
    # the egg: an outline narrower at the top, shaded from yellow to orange, a dark rim
    for y in range(N):
        for x in range(N):
            v = (y - 16.5) / 10.5
            if -1 <= v <= 1:
                half = 7.2 * math.sqrt(max(0.0, 1 - v * v)) * (1 - 0.16 * (1 - v) / 2)
                dx = abs(x + 0.5 - 16)
                if dx <= half:
                    t = min(1.0, max(0.0, (dx / max(half, 1)) * 0.6 + (v + 1) / 2 * 0.5))
                    c = lerp((255, 236, 120), (235, 110, 20), t)
                    if dx > half - 1.0 or abs(v) > 0.93:
                        c = (120, 40, 10)
                    p[x, y] = c + (255,)
    # its digital band: a zigzag of dark squares
    for i, x in enumerate(range(10, 23)):
        y = 17 if i % 2 else 16
        if p[x, y][0] > 200:
            p[x, y] = (40, 30, 110, 255)
    # a glint and four sparks
    for x, y in ((13, 10), (13, 11), (14, 10)):
        p[x, y] = (255, 255, 230, 255)
    for x, y in ((6, 8), (26, 9), (7, 22), (25, 21)):
        for dx, dy in ((0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)):
            p[x + dx, y + dy] = (180, 240, 255, 255)
    # rounded corners
    mask = Image.new("L", (N, N), 0)
    ImageDraw.Draw(mask).rounded_rectangle((0, 0, N - 1, N - 1), radius=5, fill=255)
    im.putalpha(mask)
    return im


def main():
    im = draw()
    for name, size in SIZES.items():
        out = ROOT / f"android/app/src/main/res/mipmap-{name}/ic_launcher.png"
        im.resize((size, size), Image.NEAREST).save(out)
        print(out.relative_to(ROOT))
    im.resize((512, 512), Image.NEAREST).save(ROOT / "android/icon_preview.png")


if __name__ == "__main__":
    main()
