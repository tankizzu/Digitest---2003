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

- **Grafica dei menu sempre in VRAM**: il foglio FILE_MENU_SPRITES (texture 0x140, 0) è già in memoria video sul
  titolo e in tutto il gioco; `FILE_CACHE.getEntry` carica il foglio se serve. Fotogrammi utili: 7 freccia dei
  fumetti, 0x16 pannellino 44×44, 0x30 / 0x37 ingranaggio ("Macchina", 12×12), 0x40 cornice gialla, 0x1A/0x41/0x42
  barre, 0x0F–0x14 e 0x26–0x38 icone degli elementi, 0x48–0x63 icone degli oggetti. `DW3_SHEET_DUMP=cartella`
  salva ogni fotogramma (titolo e menu) in PPM.
- **Stessa profondità di un layer**: ciò che si aggiunge dopo viene disegnato sotto (l'OT inserisce in testa). Per
  un'icona sopra un pannello: prima l'icona, poi il pannello, o profondità diverse (attenzione: sul titolo la
  profondità 1 sta dietro lo sfondo).
- `native_touch_safe_inset(lato)`: l'area sicura del telefono nei pixel del gioco, per i pulsanti disegnati dal
  gioco (le impostazioni del titolo).

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

- **Nome della memory card**: `saves/game<N>/name.txt` (games.c: `native_game_name`, `native_game_named`,
  `native_game_set_name`, `native_game_label` già codificato per le finestre). saves.c substate 8 = scrittura del
  nome (prima di `nativeStartCheck` quando si salva e il file non c'è); testi MEMCRD 0x31 (domanda) e 0x32 (OK).
- **Sì/No del gioco nella memory card**: windows[2] (0x16 Sì) e windows[3] (0x17 NO) con il cursore a (0xC2,
  0xBD + 14·scelta), riquadro piccolo sprite 29 a (188,185). Domande: substate 40 (sovrascrittura, testo 7), 101
  (errore: 0x19 formattazione = "Preparare la partita", 0x1A nuovo file), 33 (scelta del salvataggio, testo 6 / 0x10;
  il menu delle tre memory card è menu.c substate 6). Con NATIVE: `nativeYesNoShow` / `nativeYesNoTap` /
  `nativeYesNoInput` / `nativeYesNoDone`, in verticale due riquadri a NATIVE_YN_Y (0xF8) sotto la cornice.
- Il salvataggio del test di prova sta in `saves/game5` e `game9`: dopo una prova che salva, rimettere card.mcd e
  togliere name.txt (copia di riserva fuori dal repository).
- **Immagini dei salvataggi** (`platform/thumbs.c`): 160×120, il 320×240 centrale dell'area mostrata (g_disp) a metà,
  dopo `soft_read` (la VRAM software è aggiornata solo su richiesta). Presa da `native_autosave_point` ogni 240 vsync o
  cambiando stanza, e subito in `autosave()`. File grezzi 15 bit con il bit 15 acceso (un texel 0 sarebbe
  trasparente). Nella memory card: `native_thumb_show` le carica in VRAM a (512,768), (768,768), (512,888) — righe
  usate solo dalla mappa del campo — e `nativeDrawThumb` le disegna con uno SPRT in pagina texture a 15 bit
  (GetTPage(2, …)), sotto la cornice, nel riquadro sprite 29.
- **Lista degli autosalvataggi**: `nativeAutoListStart` riempie `STGMCARD_funcs.infoBuf` (MemCardFile) con i due
  autosalvataggi (`native_autosave_read_slot`, `nativeFillInfo` come STGMCARD_fillSave; zona e luogo da
  FIELDSTG_areaNames come un salvataggio dal menu) e salta allo stato 30 (la scelta). Sì → `nativeLoadAutosave`;
  nessun controllo della memory card in quella lista.
- I test di caricamento cambiano `saves/prefs.txt` (`game = N`): rimetterlo a 9 prima del confronto con i
  riferimenti in orizzontale (altrimenti l'elenco parte da un'altra riga).
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
- `DW3_PRESS` "save" (es. `1900:save`): il campo apre la schermata di salvataggio come la riga SALVA del menu
  (serve `build-linux/native.ini` con `save_anywhere = 1` solo per la riga del menu, non per questo).
- Percorso di prova in verticale (H 710): `300:start,500:start,700:t160y509` (CONTINUA), `1000:t190y662` (slot 5),
  `1100:t84y375` (Carica), `1200:t84y375` (Automatico), `1400:cross` → campo verso 1700.
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

- **Modalità del nome (STPLNMET) in verticale**: scene.c (patch nuova) crea i due layer sulla tela alta; il primo
  ha l'origine al centro (si centra da solo), il secondo (0x1001) riceve l'offset (H-240)/2. Lo sfondo (backdrop.c,
  foglio PLNMET_SCROLL: fotogrammi 0 e 1 che scorrono in diagonale, 540×532 e 580×477; 2 fermo) ha periodo verticale
  44 (fotogramma 0) e 38 (fotogramma 1): copie a ±264 e ±266. `native_sheet_dump` ora salva fogli fino a 640 pixel.
- **Strada di Asuka** (MODE_NEW_GAME 0x2D7, `--field 727`): mappa di 5×3 riquadri (640×384). In verticale la
  telecamera la centra (scroll y ≈ -223 con H 710): le bande nere sono la fine della mappa, non un 4:3.

## Verticale, fase 1: il campo a tutto schermo (progetto, 17/11/2026)

Cosa fa oggi il gioco (verificato nel codice):
- Schermo 320×240 (`SCREEN_WIDTH/HEIGHT`, engine/gfx.h); due buffer a (0,0) e (0,256) (display.c,
  `setDisplayMode`); ogni layer ha il suo DRAWENV per buffer (layer.c).
- La VRAM è 1024×512 (PlayStation). Nel port c'è il GPU software (gpu_draw.c) nella VRAM e la stessa cosa
  disegnata più fine da OpenGL in una copia della VRAM (gl/); lo schermo è l'area del DISPENV in quella copia.
- La mappa del campo è a pezzi da 128×128 (map_streamer.c): 30 "stream" tengono in RAM, compressi, i pezzi di un
  vicinato 6×5 attorno al giocatore (disposizione da FIELDSTG_slotLayouts secondo direzione e quadrante); 12
  "immagini" sono i pezzi decompressi nella VRAM (texture a 8 bit, CLUT dello stream); se ne disegnano 4×3
  attorno allo scroll. Ogni pezzo è fatto di sprite su 3 profondità (stream.c), per stare davanti/dietro agli
  attori. Un pezzo non ancora decompresso si copre di nero.
- La telecamera (camera.c) centra a (centro − 0xA0, centro − 0x8C) e tiene lo schermo dentro la mappa.

Progetto (tela verticale 320×640, larga quanto l'originale, più alta di 400 righe):
1. **VRAM del port più alta** (fatto: 1024×1024, come la GPU con 2 MB; E3/E4 a 10 bit, pagina con bit 11,
   `g_vram_dirty[16]`, shader con `& 1023`; il backend D3D12 non è aggiornato) — progetto iniziale: 1024×1280: righe ≥ 512 solo per il port. Pagina texture: bit 11 = y 512 (come
   la GPU con 2 MB). Toccare: platform.h (VRAM_H), gpu.c, gpu_draw.c (maschere `& 511`), gl/ (texture della copia),
   state.c (dimensione degli stati), post.c (lettura dell'area mostrata). Con render_scale alto la copia GL
   diventa grande: limitare la scala in verticale (texture ≤ 4096).
2. **Campo in verticale**: in modalità campo un solo buffer 320×640 a (0,384) (righe 384–1023; i vecchi
   buffer sotto la riga 384 restano liberi per le immagini di mappa in più) (display.c / layer.c via patch,
   solo nel campo); i vecchi buffer 320×480 a (0,0) restano liberi.
3. **Più immagini di mappa**: da 12 a 27 (le 15 in più nell'area dei vecchi buffer, x 0–319, y 0–479), e
   disegno di 4 colonne × fino a 6 righe; vicinato degli stream ridisposto 4 colonne × 7 righe (28 di 30) al
   posto delle tabelle 6×5.
4. **Telecamera**: centro con 640 di altezza; nelle mappe più basse di 640 righe (stanze piccole) ingrandire
   quanto basta (decisione già presa) invece del nero.
5. **Interfaccia del campo**: dialoghi, banner dell'area, fumetti e menu sono posizionati per 320×240 → vanno
   spostati (dialoghi in basso, ecc.).
6. **Android**: manifest in verticale; touch con le nuove dimensioni (s_disp).
Prima prova: il prologo (Online Center, Atrio principale), screenshot da mandare all'utente prima di andare
oltre.

Stato reale (17/11/2026, prototipo su Linux, `DW3_PORTRAIT=1`):
- Tela 320×512 a VRAM (0,512): le righe 496–511 sotto i vecchi buffer contengono le CLUT del gioco (x 0–831),
  quindi la tela non può partire prima; 640 righe non ci stanno in 1024.
- layer.c: un layer con `clip.y >= NATIVE_FIELD_Y` ha un solo buffer e il suo offset parte da NATIVE_FIELD_Y;
  `getViewRect` e `setClipPos` lavorano dall'alto della tela (prima gli oggetti animati sparivano: la vista
  risultava 512 righe più in basso).
- gpu.c: lo sfondo di un layer (TILE) è al massimo 511 righe: diviso in pezzi da 256.
- gl/post.c: con un'area mostrata più alta di 256 righe le proporzioni sono quelle dell'area (pixel quadrati).
- CLUT: il port legge la y delle CLUT su 10 bit; quelle delle immagini di mappa, in verticale, a x 384, y 896+slot
  (le righe dei vecchi buffer le usano i banner dell'area).
- Su Android il port sceglie render_scale fino a 4: con la VRAM 1024×1024 la copia GL è 4096² → da limitare (2
  o 3) prima di accendere il verticale sul telefono.

## Fascia alta del campo (fatta, 2.1)

- `FieldChildren.nativeBand` (field_mode.h, slot in più in fondo: runChildTasks lo fa girare col campo e muore con
  lui), creato in field.c accanto al banner. Il compito `nativeBandUpdate` ha 4 finestre di testo figlie (3 nomi,
  "MENU"). Si mostra se `openFieldMenu` ha visto il giocatore libero in questo fotogramma o nel precedente
  (`nativeBandAt`). Layer FIELD_LAYER_TEXT: barre (menu 0x41) a profondità 1, testi e HP a profondità 0.
- Nome: TEXT_DIGIMON_NAMES, stringa `partners[m].unlocked - 2` (id + 1). HP: `info.stats[STAT_HP / STAT_MAX_HP]`.
- MENU: `native_touch_hold(PAD_START, 6)`; in verticale il vecchio pulsante menu del touch non si mostra più.
- Prova: `--field 515 --progress 5` (Sala della Matrice dopo Genji, squadra di partenza).

## Piano: fascia alta del campo in verticale (da approvare nei dettagli, 17/11/2026)

Demo approvata: tre pannelli dei partner (nome, barra HP) a sinistra, MENU e OPZIONI a destra, in alto sul campo.
Proposta di realizzazione, tutta col materiale già in VRAM nel campo:
- Disegno: un compito del port nel campo (field.c, dove gira `openFieldMenu`), sul layer della mappa a profondità 0,
  solo in verticale e quando il giocatore ha il controllo (niente dialoghi, eventi, menu). Posizione: le prime
  ~100 righe della tela (y psx da -(H-240)/2), dentro l'area sicura (`native_touch_safe_inset`).
- Pannelli: le barre e i riquadri del foglio dei menu (FILE_MENU_SPRITES, 0x140,0: barre 0x1A / 0x41 / 0x42,
  pannellino 0x16) scalati; nome del partner in una finestra di testo del gioco (font del gioco); HP come barra
  piena/vuota (POLY_F4 verde come le barre del gioco, o le barre dei menu tagliate). Dati: `GAME.party[i]` →
  `GAME.partners[m].info.stats[STAT_HP / STAT_MAX_HP / STAT_LEVEL]`.
- MENU: tocco = Start (il menu del campo esiste solo da STORY_GENJI_BEATEN: prima il pulsante non si mostra).
  OPZIONI: le impostazioni del port (oggi la schermata della lingua; volume da aggiungere).
- Da decidere con l'utente: la fascia copre la mappa (sovrapposta) o sposta il centro della telecamera più in basso;
  se mostrarla sempre o solo da fermi; prima di Genji (niente menu) solo i partner.
Battaglia in verticale: lavoro più grande (layout della battaglia, telecamera 3D, comandi): serve un piano a parte
dopo il riscontro sulla 2.0.
