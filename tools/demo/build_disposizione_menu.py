"""Builds the field menu layout demo ("Disposizione menu"): pieces cropped from real landscape captures at 3x (each game
pixel is 3 screen pixels) over the menu's own background repeated over the upright screen, from the battle demo's
template (disposizione_template.html). The page it writes holds game graphics: never commit it.

    python3 tools/demo/build_disposizione_menu.py OUT.html

The captures (see TECHNICAL_NOTES, "Demo da disporre"): F + mpA, mpB, mpE (landscape, --field 515 --progress 5, items
given with --set 802B=1 ...) and fm2 (upright, for the background)."""
import base64
import io
import json
import os
import sys

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
F = "/home/user/feerdus95/"
H = 710
TOP = (H - 240) // 2  # where the game's 240 lines sit upright today (centred)


HELP_BATTLE = ("Le linee arancioni tratteggiate segnano dove inizia la parte bassa usata oggi dai menu (le ultime 240 "
               "righe). Statistiche e volto del partner si spostano nella scheda Comandi; nelle altre schede compaiono "
               "più chiari, come riferimento.")
HELP_MENU = ("Le linee arancioni tratteggiate segnano dove sta oggi il menu (i suoi 320×240 al centro dello schermo). I tre "
             "pannelli dei partner si spostano nella scheda «Oggetti: categorie»; in «A chi» e «Ordina» compaiono più "
             "chiari, come riferimento. Le righe delle liste diventeranno toccabili: falle grandi quanto ti servono.")


def uri(im):
    buf = io.BytesIO()
    im.save(buf, "PNG", optimize=True)
    return "data:image/png;base64," + base64.b64encode(buf.getvalue()).decode()


def crop3(path, x, y, w, h):
    return Image.open(path).convert("RGBA").crop((x * 3, y * 3, (x + w) * 3, (y + h) * 3))


def piece(pid, label, path, rect, shared=None):
    x, y, w, h = rect
    return {"id": pid, "label": label, "src": uri(crop3(path, x, y, w, h)), "ow": w, "oh": h,
            "x": x, "y": TOP + y, "w": w, "h": h, "shared": shared}


def background():
    """The menu's background from an upright capture: the rows above the content are the pattern alone (period 96),
    two periods repeated down the whole height"""
    win = Image.open(F + "fm2/window_01300.png").convert("RGB").crop((318, 0, 642, 720)).resize((320, H), Image.LANCZOS)
    tile = win.crop((0, 0, 320, 192))
    bg = Image.new("RGB", (320, H))
    for y in range(0, H, 192):
        bg.paste(tile, (0, y))
    return bg


def main():
    menu = F + "mpA/window_01500.png"
    cat = F + "mpA/window_01300.png"
    lst = F + "mpE/window_01500.png"
    who = F + "mpE/window_01700.png"
    sort = F + "mpB/window_02400.png"
    stat = F + "mpA/window_03300.png"

    panels = [piece("p1", "Partner 1 (pannello)", cat, (0, 17, 146, 44)),
              piece("p2", "Partner 2 (pannello)", cat, (0, 63, 146, 44)),
              piece("p3", "Partner 3 (pannello)", cat, (0, 109, 146, 44))]
    ref = [dict(p, shared="categorie") for p in panels]
    screens = [
        {"id": "menu", "name": "Menu", "pieces": [
            piece("m1", "Partner 1 (piccolo)", menu, (0, 17, 116, 44)),
            piece("m2", "Partner 2 (piccolo)", menu, (0, 63, 116, 44)),
            piece("m3", "Partner 3 (piccolo)", menu, (0, 109, 116, 44)),
            piece("bit", "BIT", menu, (0, 157, 108, 20)),
            piece("titolo", "Titolo \"Menu\"", menu, (115, 15, 205, 22)),
            piece("voci", "Voci del menu", menu, (168, 40, 152, 110)),
        ]},
        {"id": "categorie", "name": "Oggetti: categorie", "pieces": panels + [
            piece("titolo", "Titolo \"Quale?\"", cat, (145, 15, 175, 22)),
            piece("categorie", "Categorie", cat, (168, 40, 152, 82)),
            piece("bit", "BIT", cat, (0, 157, 108, 20)),
            piece("aiuto", "Riquadro di aiuto", cat, (0, 194, 320, 36)),
        ]},
        {"id": "lista", "name": "Oggetti: lista", "pieces": [
            piece("titolo", "Titolo della categoria", lst, (10, 17, 135, 18)),
            piece("col1", "Colonna sinistra", lst, (28, 36, 132, 114)),
            piece("col2", "Colonna destra", lst, (168, 36, 124, 114)),
            piece("pagina", "Pagina (1/2, R1)", lst, (145, 152, 145, 19)),
            piece("riga", "Oggetto scelto (Equip, quanti)", lst, (0, 172, 300, 20)),
            piece("descr", "Descrizione", lst, (14, 193, 292, 37)),
        ]},
        {"id": "achi", "name": "Oggetti: a chi", "pieces": ref + [
            piece("titolo", "Titolo \"Scegli Digimon\"", who, (145, 15, 175, 22)),
            piece("riga", "Oggetto scelto", who, (14, 165, 300, 27)),
            piece("descr", "Descrizione", who, (14, 193, 292, 37)),
        ]},
        {"id": "ordina", "name": "Ordina", "pieces": ref + [
            piece("frecce", "Frecce dello scambio", sort, (146, 35, 22, 58)),
            piece("titolo", "Titolo \"Scambia con?\"", sort, (145, 15, 175, 22)),
            piece("aiuto", "Riquadro di aiuto", sort, (0, 194, 320, 36)),
        ]},
        {"id": "stato", "name": "Stato", "pieces": [
            piece("pannello", "Partner (pannello)", stat, (0, 17, 146, 44)),
            piece("exp", "EXP", stat, (0, 60, 107, 20)),
            piece("stat", "Statistiche", stat, (0, 84, 108, 108)),
            piece("tp", "TP", stat, (0, 195, 75, 25)),
            piece("scelte", "Scelte (Digievoluzioni, Equipaggiamento)", stat, (148, 18, 110, 30)),
            piece("dv", "Digievoluzioni", stat, (160, 52, 160, 66)),
            piece("equip", "Equipaggiamento", stat, (160, 118, 160, 104)),
        ]},
    ]
    for sc in screens:
        sc["guides"] = [{"y": TOP, "label": "dove sta oggi (320×240 al centro)"}, {"y": TOP + 240, "label": ""}]
    data = {"height": H, "bg": uri(background()), "screens": screens}
    tpl = open(os.path.join(HERE, "disposizione_template.html")).read()
    tpl = tpl.replace("Disposizione battaglia", "Disposizione menu").replace(HELP_BATTLE, HELP_MENU)
    out = tpl.replace("__DATA__", json.dumps(data))
    open(sys.argv[1] if len(sys.argv) > 1 else "disposizione_menu.html", "w").write(out)
    print("wrote", len(out) // 1024, "KB")


if __name__ == "__main__":
    main()
