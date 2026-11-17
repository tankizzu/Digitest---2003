"""Reads and writes the European version's 1-byte text files (a u32 count, u32 offsets, then the strings).

    python3 tools/unix/gametext.py dump FILE      every string, decoded (control codes as <xx>)

The letters: uppercase ASCII - 0x33, lowercase - 0x39, digits - 0x2C; 01 is half a space (a space is 01 01), 00 ends
a string; the others below as far as they are known, anything else stays a <xx> code.
"""
import struct
import sys

PUNCT = {0xE6: "."}


def decode(b):
    out = []
    for c in b:
        if 0x0E <= c <= 0x0E + 25:
            out.append(chr(c + 0x33))
        elif 0x28 <= c <= 0x28 + 25:
            out.append(chr(c + 0x39))
        elif 0x04 <= c <= 0x0D:
            out.append(chr(c + 0x2C))
        elif c == 0x01:
            out.append("_")
        elif c in PUNCT:
            out.append(PUNCT[c])
        else:
            out.append(f"<{c:02x}>")
    return "".join(out).replace("__", " ")


def encode(s):
    """Plain text to the game's bytes, ending 00; <xx> passes a code through"""
    out, i = bytearray(), 0
    while i < len(s):
        ch = s[i]
        if ch == "<":
            out.append(int(s[i + 1:i + 3], 16))
            i += 4
            continue
        if "A" <= ch <= "Z":
            out.append(ord(ch) - 0x33)
        elif "a" <= ch <= "z":
            out.append(ord(ch) - 0x39)
        elif "0" <= ch <= "9":
            out.append(ord(ch) - 0x2C)
        elif ch == " ":
            out += b"\x01\x01"
        else:
            rev = {v: k for k, v in PUNCT.items()}
            if ch not in rev:
                raise ValueError(f"no glyph known for {ch!r}")
            out.append(rev[ch])
        i += 1
    return bytes(out + b"\x00")


def strings(data):
    count = struct.unpack_from("<I", data, 0)[0]
    offs = struct.unpack_from(f"<{count}I", data, 4)
    res = []
    for o in offs:
        e = data.index(b"\x00", o)
        res.append(data[o:e])
    return res


if __name__ == "__main__":
    if sys.argv[1] == "dump":
        for i, s in enumerate(strings(open(sys.argv[2], "rb").read())):
            print(f"{i:#04x} {decode(s)}")
