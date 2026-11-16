#!/bin/bash
# shoot.sh OUTDIR SECONDS DUMP_SPEC "PRESS_SPEC" : runs the Linux build headless, saving window_*.bmp frames
out=$1; secs=$2; dump=$3; press=$4
mkdir -p "$out" && cd "$out" && rm -f *.bmp *.png
DW3_DATA=${DW3_DATA:?set DW3_DATA to the extracted game} SDL_AUDIODRIVER=dummy DW3_DUMP="$dump" DW3_PRESS="$press" \
  timeout "$secs" xvfb-run -a -s "-screen 0 1280x960x24" $(dirname "$0")/../../build-linux/dw2003 > out.txt 2>&1
grep -v ALSA out.txt | grep -i "crash\|error" | head -5
rm -f vram_*.bmp
python3 - <<'PY'
from PIL import Image
import glob
fs=sorted(glob.glob('window_*.bmp'))
for f in fs: Image.open(f).save(f[:-4]+'.png')
import os
for f in fs: os.remove(f)
fs=sorted(glob.glob('window_*.png'))
if fs:
    cols=6; tw=240; ims=[Image.open(f) for f in fs]; w,h=ims[0].size; th=int(h*tw/w)
    rows=(len(ims)+cols-1)//cols
    sheet=Image.new('RGB',(cols*tw,rows*(th+14)),'black')
    from PIL import ImageDraw
    d=ImageDraw.Draw(sheet)
    for k,(f,im) in enumerate(zip(fs,ims)):
        x=(k%cols)*tw; y=(k//cols)*(th+14)
        sheet.paste(im.resize((tw,th)),(x,y+14)); d.text((x+2,y+1),f[7:12],fill='white')
    sheet.save('sheet.png'); print(len(fs),'frames')
PY
