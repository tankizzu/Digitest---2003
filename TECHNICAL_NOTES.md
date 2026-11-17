# Note tecniche

## Architettura

- Codice del gioco: dw3_decomp (commit 791932b) + patch in `patches/` (applicate da `build.py`/`build_unix.py`,
  che scrivono `build-*/patched/`). `build_unix.py` riusa le riscritture IR di `build.py`.
- Piattaforma: `platform/` (SDL3). Rendering GL, GLES 3.0 su Android (`DW3_GLES`).
- 64 bit: il gioco è compilato con `-Dlong=int`; `psx_long` per le funzioni PsyQ; tutta la memoria del gioco
  sotto i 4 GB, heap sotto i 16 MB.
- Android: `libmain.so` (loader) riserva memoria bassa e carica `libdw3.so` con `android_dlopen_ext` +
  `ANDROID_DLEXT_RESERVED_ADDRESS`; la sezione heap `.bss.dw3heap` sta prima di `.text`. GameActivity,
  manifest sensorLandscape (da rivedere per il verticale), adjustResize, TRAP_BACK_BUTTON.

## Strato touch (`platform/touch.c`, `platform/gl/touch.c`)

- I bersagli sono in coordinate dello schermo PSX: `native_touch_target(id, x, y, w, h)`,
  `native_touch_tap()` / `native_touch_tap_id(id)`. Gli id ≥ 0x7A00 sono del port.
- `native_touch_hold(bit, vsync)` inietta un tasto logico (letto in engine/pad/port.c).
- `native_touch_field_free()`: il campo lascia il controllo al giocatore in questo fotogramma → la levetta
  compare; tocco breve = X.
- `native_touch_stick_level()`: 0 levetta ferma, 1 spinta leggera (≤ 0,65: cammina), 2 piena (corre).
- Pulsanti d'angolo: menu in alto a destra; Indietro in basso a sinistra (sopra il pulsante dei dialoghi
  automatici quando c'è un dialogo). Pulsante dialoghi automatici: `auto_button()`, freccia dallo sprite del
  gioco (`native_touch_auto_sprite`, FILE_MENU_SPRITES fotogramma 7, texture 0x140,0) disegnata in
  `draw_arrow()`.

## Campo

- `FIELDSTG_controlPlayer` (actor_control.c): con NATIVE cerca il personaggio parlabile più vicino entro 30
  unità di mappa (tutti gli attori: `find(FIELD_TASK_ACTOR, -1, -1)`; escludendo i partner, che hanno `trail`, e
  i key1 0x180–0x182), mostra il fumetto (task id 0x7A, riga 3 di FIELDSTG_triggerAnims) se non c'è già quello
  di un'uscita (id 6); il tocco (X) gira il giocatore verso di lui (direzione da FIELDSTG_dirSteps) e chiama
  `FIELDSTG_talkToActorAt`.
- **Un task nuovo gira solo se sta in uno spazio "figlio"** di un task che gira (`runChildTasks` scorre
  l'array `children` del genitore): creato e basta resta in TASK_INIT per sempre. Il fumetto dei personaggi sta
  in `ActorChildren.balloon` del giocatore (lo stesso spazio del minigioco, che si riconosce dall'id).
- Corsa: in `FIELDSTG_runActorAction`, ACTOR_RUN del giocatore (`control == FIELDSTG_controlPlayer`) con la
  levetta usa `speed * NATIVE_RUN_PERCENT / 100` (58).
- Porte: in `FIELDSTG_updateTriggers` substate 1, uno SLOT_EXIT parte anche quando la levetta è spinta (il
  trigger d'uscita conta solo se il giocatore è rivolto verso l'uscita: `FIELDSTG_nearDirs`).

## Salvataggi

- Elenco delle partite (saves.c, NATIVE): testo costruito da `native_games_list_text` (games.c) nel
  `windows[0]` della schermata, 2 righe visibili a passo 28 px da y 0x81; `native_touch_list` /
  `native_touch_list_event` (touch.c) danno tocco, pressione lunga (600 ms) e trascinamento. Substate 6 =
  conferma dell'eliminazione (`native_game_delete`). Data di creazione: `saves/game<N>/created.txt` (secondi),
  altrimenti l'ultima scrittura di card.mcd.

- Partite: `saves/game<N>/card.mcd` + `auto_a.sav` / `auto_b.sav`; `prefs.txt` (language, auto_text, game).
- GAME_SAVE = primi 0x26C4 byte di GAME; checksum al byte 0 = computeChecksum(buf+4, size-4); versione byte 2 =
  4; fieldMode a 0x34, posizione 0x38/0x3C, direzione 0x40.
- Testi della memory card riscritti in `platform/savetexts.c` (IT ed EN; le altre lingue ricevono solo i testi
  aggiunti in inglese).

## Testi del gioco

- **Punteggiatura a 2 byte**: `01 05` è il punto, `01 07` i due punti, `01 6F` la Ì; `E6` da solo è il "?".
  Un testo costruito a mano si passa con `setString(window, testo, -1)` (1 byte); `setText` lo tratta come
  Shift-JIS e lo mostra illeggibile.
- Codifica europea a 1 byte: maiuscole −0x33, minuscole −0x39, cifre −0x2C, `01 01` spazio, `02 01` a capo,
  `02 05 01` il numero della finestra, E6 punto. `tools/unix/gametext.py dump FILE`.
- File di testo: u32 conteggio, u32 offset, stringhe.
- LANGUAGE: 2 inglese, 4 italiano (file ESxxx / ISxxx).

## Test su Linux (senza telefono)

- `tools/shoot.sh OUTDIR SECONDI OGNI:DA:A "PRESS"` (copia in /home/user/feerdus95/tools): avvia con Xvfb e
  salva i fotogrammi; `sheet.png` li riassume.
- `DW3_PRESS="250:start,700:t150y176,..."`: tasti o tocchi (`tXyY` in pixel PSX) a un dato fotogramma; `f5`/`f9`
  stato rapido (solo stessa build).
- `DW3_TOUCH=1` attiva il touch (indispensabile per tocchi e levetta). `DW3_STICK="da:a:x:y,..."` simula la
  levetta. `DW3_TYPE="<<<<<<Marco"` scrive col "telefono" (`<` cancella, `|` Invio).
- I tocchi `tXyY` di DW3_PRESS passano solo dai bersagli (`native_touch_target`), non dall'elenco scorrevole:
  per l'elenco usare `down`/`up`/`cross`.
- `DW3_SHOT_TOUCH=1` mette i pulsanti del touch negli screenshot.
- `DW3_AUTOSAVE_VSYNCS=600` salvataggio automatico più frequente. `DW3_LOG=1` stampa i log. `--field 1280`
  parte dalla schermata del nome.

## Problemi risolti (da non ripetere)

- `long` a 64 bit su Linux → `-Dlong=int`.
- Puntatori troncati in EventChildren → s64.
- Crash alla scelta del kit (17/11/2026): il tocco su una linguetta cambiava `task->choice` prima che
  `runChoice` salvasse il valore vecchio, quindi i contatori d'animazione non venivano azzerati e il nuovo
  Digimon (con meno fotogrammi) leggeva il fotogramma -1 → `spriteDrawerDraw` fuori dalla tabella. Ora `old` è
  letto prima del tocco.
- Un pannello nuovo si azzera al primo fotogramma: va avviato un fotogramma dopo.
- `native_touch_tap` non deve consumare gli id ≥ 0x7A00.
- Fumetto dei personaggi invisibile (0.8): creato senza genitore, non girava mai. Ora è figlio del giocatore.
- Righe della memory card troppo lunghe ("Slot MEMORY CARD N" tagliato): accorciate in "MEMORY CARD N".
