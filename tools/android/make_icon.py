"""The app's launcher icon (an original drawing: a digital egg on a navy tile, circuit lines and pixel sparks; no
character or logo of the game), at Android's five densities into android/app/src/main/res/mipmap-*/ic_launcher.png

    python3 tools/android/make_icon.py
"""
import math
from pathlib import Path
from PIL import Image, ImageDraw, ImageFilter

ROOT = Path(__file__).resolve().parents[2]
SIZES = {"mdpi": 48, "hdpi": 72, "xhdpi": 96, "xxhdpi": 144, "xxxhdpi": 192}
S = 768  # drawn big, then scaled down


def egg(d, box, fill):
    """An egg's outline in the box: round below, narrower above"""
    x0, y0, x1, y1 = box
    cx, w, h = (x0 + x1) / 2, (x1 - x0) / 2, (y1 - y0)
    pts = []
    for i in range(181):
        t = math.pi * 2 * i / 180
        y = math.cos(t)  # 1 bottom, -1 top
        k = 1.0 - 0.18 * (1 - y) / 2  # narrower toward the top
        pts.append((cx + w * k * math.sin(t), y0 + h * (0.58 + 0.42 * y) if y >= 0 else y0 + h * 0.58 * (1 + y)))
    d.polygon(pts, fill=fill)


def draw():
    im = Image.new("RGBA", (S, S), (0, 0, 0, 0))
    d = ImageDraw.Draw(im)
    # the tile: navy, a lighter rim
    d.rounded_rectangle((16, 16, S - 16, S - 16), radius=150, fill=(10, 22, 70, 255), outline=(70, 140, 255, 255), width=18)
    # circuit lines behind the egg
    line = (40, 90, 200, 255)
    for y, x0, x1, dot in ((210, 70, 250, 250), (560, 520, 700, 520), (470, 70, 220, 220), (300, 560, 700, 560)):
        d.line((x0, y, x1, y), fill=line, width=14)
        d.ellipse((dot - 22, y - 22, dot + 22, y + 22), fill=line)
    # the glow, then the egg: orange shading to yellow
    glow = Image.new("RGBA", (S, S), (0, 0, 0, 0))
    egg(ImageDraw.Draw(glow), (210, 150, 560, 640), (255, 150, 40, 170))
    im.alpha_composite(glow.filter(ImageFilter.GaussianBlur(40)))
    egg(d, (230, 170, 540, 620), (240, 120, 20, 255))
    egg(d, (270, 200, 500, 560), (255, 170, 50, 255))
    egg(d, (310, 230, 450, 450), (255, 215, 110, 255))
    # the egg's digital band: a zigzag of squares across it
    for i, x in enumerate(range(250, 525, 34)):
        y = 420 + (16 if i % 2 else -16)
        d.rectangle((x, y - 16, x + 30, y + 16), fill=(20, 40, 120, 255))
    # pixel sparks
    for x, y, s in ((150, 150, 26), (620, 180, 34), (600, 640, 22), (130, 600, 30)):
        d.rectangle((x - s, y - s // 3, x + s, y + s // 3), fill=(150, 220, 255, 255))
        d.rectangle((x - s // 3, y - s, x + s // 3, y + s), fill=(150, 220, 255, 255))
    return im


def main():
    im = draw()
    for name, size in SIZES.items():
        out = ROOT / f"android/app/src/main/res/mipmap-{name}/ic_launcher.png"
        im.resize((size, size), Image.LANCZOS).save(out)
        print(out.relative_to(ROOT))
    im.resize((512, 512), Image.LANCZOS).save(ROOT / "android/icon_preview.png")


if __name__ == "__main__":
    main()
