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


def draw(layer="both"):
    """the whole icon, or one layer of the adaptive one: "back" the field and its digits, "fore" the egg and sparks"""
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
    if layer == "fore":
        im = Image.new("RGBA", (N, N), (0, 0, 0, 0))
        p = im.load()
    # binary digits, faint
    for (x0, y0, s) in ((1, 2, "10"), (24, 3, "01"), (1, 24, "01"), (25, 25, "10"), (2, 13, "1"), (28, 14, "0")):
        for k, ch in enumerate(s):
            for j, row in enumerate(DIGITS[ch]):
                for i, bit in enumerate(row):
                    x, y = x0 + k * 4 + i, y0 + j
                    if bit == "1" and 0 <= x < N and 0 <= y < N and layer != "fore":
                        p[x, y] = lerp(p[x, y][:3], (120, 230, 170), 0.55) + (255,)
    if layer == "back":
        return im
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
    return im  # full: square, no corners of its own (the launcher's shape cuts it)


ADAPTIVE = """<?xml version="1.0" encoding="utf-8"?>
<!-- the launcher icon in two layers that fill whatever shape the phone gives it (tools/android/make_icon.py) -->
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@mipmap/ic_launcher_background" />
    <foreground android:drawable="@mipmap/ic_launcher_foreground" />
</adaptive-icon>
"""


def main():
    im = draw()
    back, fore = draw("back"), draw("fore")
    for name, size in SIZES.items():
        res = ROOT / f"android/app/src/main/res/mipmap-{name}"
        im.resize((size, size), Image.NEAREST).save(res / "ic_launcher.png")
        # adaptive layers: 108 dp of which the middle 72 dp show; the egg, in 32 cells, keeps to that middle
        big = size * 108 // 48
        back.resize((big, big), Image.NEAREST).save(res / "ic_launcher_background.png")
        f = Image.new("RGBA", (big, big), (0, 0, 0, 0))
        inner = fore.resize((size * 72 // 48, size * 72 // 48), Image.NEAREST)
        f.paste(inner, ((big - inner.width) // 2, (big - inner.height) // 2), inner)
        f.save(res / "ic_launcher_foreground.png")
        print(res.relative_to(ROOT))
    any26 = ROOT / "android/app/src/main/res/mipmap-anydpi-v26"
    any26.mkdir(exist_ok=True)
    (any26 / "ic_launcher.xml").write_text(ADAPTIVE)
    im.resize((512, 512), Image.NEAREST).save(ROOT / "android/icon_preview.png")


if __name__ == "__main__":
    main()
