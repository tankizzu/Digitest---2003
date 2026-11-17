"""The page of the battle's layout demos (artifact "Disposizione battaglia"): python3 tools/demo/build_disposizione.py OUT.html.
The output holds pieces of the game's graphics: never in the repository.

Builds the layout editor page: crops the game's pieces from real screenshots (landscape at 3x, so each game
pixel is 3 screen pixels) and the clean upright background, and writes disposizione.html from template.html."""
import base64
import io
import json
import os
import sys

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
PIECES = "/home/user/feerdus95/pieces"
PIECES2 = "/home/user/feerdus95/pieces2"
BG = "/home/user/feerdus95/bgclean/window_01500.png"
H = 710
LOW = H - 240  # where the menus' 240 lines start upright


def uri(im):
    buf = io.BytesIO()
    im.save(buf, "PNG", optimize=True)
    return "data:image/png;base64," + base64.b64encode(buf.getvalue()).decode()


def crop3(path, x, y, w, h):
    im = Image.open(path).convert("RGBA")
    return im.crop((x * 3, y * 3, (x + w) * 3, (y + h) * 3))


def piece(pid, label, path, rect, at=None, size=None, shared=None):
    x, y, w, h = rect
    ax, ay = at if at else (x, y)
    sw, sh = size if size else (w, h)
    return {"id": pid, "label": label, "src": uri(crop3(path, x, y, w, h)), "ow": w, "oh": h,
            "x": ax, "y": ay, "w": sw, "h": sh, "shared": shared}


def main():
    bg = Image.open(BG).convert("RGB").crop((318, 0, 642, 720)).resize((320, H), Image.NEAREST)
    bg_uri = uri(bg)
    f_cmd = os.path.join(PIECES, "window_01300.png")
    f_tech = os.path.join(PIECES, "window_01500.png")
    f_dv = os.path.join(PIECES, "window_01850.png")
    f_msg = os.path.join(PIECES, "window_02400.png")

    hud = [
        piece("nemico", "Nemico (nome, HP, squadra)", f_cmd, (8, 15, 150, 44)),
        piece("partner", "Partner (nome, HP, squadra)", f_cmd, (161, 15, 151, 44)),
        piece("indicatore", "Barra bianca (indicatore)", f_cmd, (260, 61, 50, 10)),
        piece("volto", "Volto del partner", f_cmd, (206, 73, 105, 65)),
    ]
    names = ["Fight", "Tech", "DV", "Tag", "Item", "Run"]
    cmds = []
    for i, n in enumerate(names):
        cmds.append(piece("cmd_" + n.lower(), "Comando " + n, f_cmd, (16, 107 + 19 * i, 98, 17),
                          at=(6 + i % 2 * 158, LOW + 240 - 6 - (3 - i // 2) * 42), size=(150, 36)))
    shared = [dict(p, shared="comandi") for p in hud]

    screens = [
        {"id": "comandi", "name": "Comandi", "pieces": hud + cmds},
        {"id": "tecniche", "name": "Tecniche", "pieces": shared + [
            piece("lista", "Lista delle tecniche", f_tech, (8, 62, 151, 111), at=(8, LOW + 62)),
            piece("mp", "MP del partner", f_tech, (163, 50, 96, 22), at=(163, LOW + 50)),
            piece("descrizione", "Descrizione e costo", f_tech, (11, 188, 297, 39), at=(11, LOW + 188)),
        ]},
        {"id": "dv", "name": "DV", "pieces": shared + [
            piece("statistiche", "Statistiche del partner", f_dv, (17, 74, 142, 111), at=(17, LOW + 74)),
            piece("forme", "Forma da scegliere", f_dv, (167, 143, 141, 18), at=(167, LOW + 143)),
        ]},
        {"id": "messaggi", "name": "Messaggi", "pieces": shared + [
            piece("messaggio", "Messaggio di battaglia", f_msg, (11, 188, 296, 38), at=(11, LOW + 188)),
        ]},
    ]
    extra = json.load(open(os.path.join(HERE, "disposizione_extra.json"))) if os.path.exists(os.path.join(HERE, "disposizione_extra.json")) else []
    for sc in extra:  # Tag and Items, once captured
        sc["pieces"] = shared + [piece(p["id"], p["label"], os.path.join(PIECES2, p["file"]), tuple(p["rect"]),
                                       at=(p["rect"][0], LOW + p["rect"][1])) for p in sc["pieces"]]
        screens.append(sc)
    for sc in screens:
        sc["guides"] = [{"y": LOW, "label": "menu (ultime 240 righe)"}]
    data = {"height": H, "bg": bg_uri, "screens": screens}
    tpl = open(os.path.join(HERE, "disposizione_template.html")).read()
    out = tpl.replace("__DATA__", json.dumps(data))
    open(sys.argv[1] if len(sys.argv) > 1 else "/tmp/disposizione.html", "w").write(out)
    print("wrote", len(out) // 1024, "KB")


if __name__ == "__main__":
    main()
