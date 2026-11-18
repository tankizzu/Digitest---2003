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
- `platform/savetexts.c` riscrive anche il file STATUS (fid 178 EN, 180 IT, 108 stringhe): i suggerimenti coi tasti.
  `rebuild()` prende il numero di stringhe originale e nuovo.
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
- `DW3_PRESS="250:start,700:t150y176,..."`: tasti o tocchi (`tXyY`: x in pixel PSX; y sulla tela alta 0..H quando la schermata ha un'origine del tocco, come i menu in verticale) a un dato fotogramma; `f5`/`f9`
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
- Equipaggiamento di prova: `--set 805C=1` ... `--set 806F=1` (armi, WEAPON_IDS in engine/game/items.c; protezioni
  da 0xD7, accessori da 0x124). Percorso in verticale: `250:start,900:start,1100:t160y390,1400:t100y57,1700:t100y151`
  (Stato, Kotemon, Equipaggiamento), poi `t100y463` (Destra). Lo `unk70` di StatsScreen (che il gioco azzera soltanto)
  dice a Stato che l'aiuto dell'equipaggiamento copre il TP.
- `DW3_MONEY=5000`: col `--field`, il denaro di partenza (per provare i negozi).
- `DW3_ENEMY_HP=1`: a ogni turno del giocatore gli HP dei nemici scendono a quel valore (battaglia vinta al primo
  colpo: per arrivare al resoconto). `--field 5120` (il resoconto da solo) resta nero: senza una battaglia non ha
  niente da mostrare.
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

## Piano: strada di Asuka a tutto schermo (da approvare, 17/11/2026)

Richiesta dell'utente: "Strada di asuka se possibile a tutto schermo come tutte le altre" (anteprima C di
asuka_opzioni.png: ingrandita fino a riempire). La mappa (MODE_NEW_GAME 0x2D7) è 640×384: in verticale (H 710)
restano 326 righe nere. Riempire vuol dire ingrandire la **sola scena** di H / 384 ≈ 1,85 (si vedono 173 pixel di
mappa in larghezza invece di 320); testi, fascia alta, levetta e pulsanti restano come sono.

Cosa c'è già: il renderer (gl, `gpu_draw.c`) sposta per layer le coordinate x dei pacchetti (`gvtx`: x → a + (x −
a)·s + d, per le battaglie 16:9) sapendo il layer che disegna (`g_draw_layer`). Si estende alla y.

Ostacolo: sul layer della mappa (FIELD_LAYER_MAP 0x1002, OT di 16) ci sono sia la scena sia i menu: profondità 11,
7, 3 i pezzi della mappa (FIELDSTG_spriteDepths), attori a `actor->depth` (+1), menu e testi a 1–2, il cursore delle
scelte a 3, il menu del campo e le transizioni a 0.

Passi proposti:
1. **Quando**: in verticale, solo nelle mappe di una lista (per ora la strada di Asuka), fattore s = H / altezza
   della mappa. Le altre stanze basse restano col nero (decisione "dove la mappa finisce resta il nero").
2. **Telecamera** (camera.c): in quelle mappe la y al centro della mappa (la mappa riempie l'altezza), la x sul
   giocatore come ora.
3. **Renderer**: layer.c dice al GPU dove sta l'OT del layer che disegna; `DrawOTag`, passando dalle voci dell'OT,
   sa la profondità e ingrandisce attorno al centro della tela i pacchetti a profondità ≥ 3 (x e y in `gvtx`, righe
   comprese). Il cursore delle scelte passa a profondità 2 nelle mappe ingrandite.
4. **Cose della scena disegnate altrove** (fumetto sopra i personaggi, riquadro del nome vicino a chi parla,
   bersagli del tocco sugli attori): la stessa trasformazione sulle loro coordinate.
5. **GPU software** (miniature dei salvataggi): resta senza ingrandimento (la miniatura mostra la scena col nero).
   Direct3D 12 non aggiornato (come il resto del verticale).
6. Prova su Linux (`--field 727`), confronto con l'anteprima C, poi APK.
Rischi: pezzi della mappa e attori che non combaciano dopo l'ingrandimento (righe di un pixel tra i pezzi da 128),
qualità più bassa (la scena è 1,85 volte più grossa).

**Acceso** dal 18/11/2026 (approvato; `DW3_ZOOM_MAP=0` lo spegne nelle prove, `native_field_zoom_on` in portrait.c):
- camera.c: nella strada (`GAME.funcs.getMode() == MODE_NEW_GAME`, mappa più bassa della tela) la y della telecamera
  mette il centro della mappa a metà tela e chiama `native_field_zoom(H / altezza, 160, H / 2, NATIVE_FIELD_Y)`;
  altrimenti `native_field_zoom(0, ...)`. `resetGraphics` (display.c) lo spegne a ogni cambio di schermata.
- display.c passa al GPU l'OT del layer che disegna (`native_draw_ot`); gpu_draw.c `DrawOTag`, a ogni voce dell'OT
  del layer della mappa, accende o spegne la trasformazione (`s_map_a/s` per la x, nuovi `s_map_b/t` per la y in
  `gvtx`) secondo la profondità (≥ 3). Solo la scheda grafica: la VRAM del GPU software resta senza ingrandimento.
- Gli attori sono a profondità 3–4 (provato con soglie 5/8/12): ingranditi con la mappa.
- speech.c (riquadro di chi parla) e balloon.c (fumetto, profondità 1) spostano il loro punto con
  `native_field_zoom_point`, restando della loro grandezza.
- Verificato su Linux (`--field 727`, `DW3_ZOOM_MAP=1`): la strada riempie la tela, Junior e i suoi dialoghi
  combaciano, le auto passano. Non provati: scelte Sì/No nella strada, il fumetto, la miniatura del salvataggio.

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

## Battaglia in verticale (2.2)

- `FIGHTSTG_CREATE_LAYERS` (include/battle/battle_mode/types.h): con `native_portrait()` area mostrata
  (0, 512, 320, H), layer sulla tela alta; i layer 3D (SCREEN, CAMERA, STAGE, WIREFRAME, MODELS) con l'offset
  (160, H/2); BATTLE_LAYER_MENUS con offset (0, H-240) (le sue 240 righe in fondo) e `native_touch_origin(H-240)`;
  un layer in più, NATIVE_BATTLE_LAYER_HUD (0x1007), con offset 0: hud.c ridefinisce BATTLE_LAYER_MENUS su di lui.
- model_mesh.c `FIGHTSTG_isMeshOnScreen`: il clip del layer va contato dall'alto della tela (meno 512).
- cursor.c: la barra del cursore è una copia VRAM→VRAM a vsync nelle coordinate dello schermo: in verticale una sola,
  a 512 + H - 240 + y. Il cursore dei comandi (6 righe, x 17: `NATIVE_CMD_GRID`) disegna la riga del gioco
  (sprite 34) scalata in una griglia 2×3 (NATIVE_CMD_X/Y), con i tocchi; command_menu.c mette le parole lì, ×1,5.
- partner_view.c: il layer del volto (0x1009) a y 512 + 0x4C, la cornice sul layer dell'HUD.
- Prova: `--field 578 --party 0` e camminare a destra e sinistra (incontro verso il fotogramma 1250); griglia a
  y 470 + 108/150/192 (+18 il centro) con H 710.

## Battaglia in verticale: la disposizione dell'utente (2.3)

- types.h: tabella `NATIVE_XF[NXF_COUNT]`, un pezzo per riga `{ox, oy, ow, oh, x, y, w, h}`: il rettangolo del pezzo
  nel gioco (coordinate 320×240) e dove l'utente l'ha messo nella demo. Pezzi dell'HUD in coordinate della tela (dal
  layer 0x1007, offset 0), pezzi dei menu in coordinate del layer MENUS (y della demo − (H − 240), cioè −470 con
  H 710). Si riportano a mano dal db della demo.
- Macro: `NXF_X/NXF_Y` (un punto del gioco → dove va), `NXF_DRAW` (sprite scalato con perno assoluto nel punto del
  pezzo), `NXF_PLACE`/`NXF_TEXT` (finestra di testo: posizione, scala, perno), `NXF_DRAW_CUT` (come NXF_DRAW ma
  ritagliato al rettangolo del pezzo, sopra esatto e `m` pixel di margine negli altri lati).
- Ritaglio degli sprite (`g_sprite_clip_on`, `g_sprite_clip[4]` in sprite_drawer.c, native.h): solo per le parti
  disegnate senza scala né rotazione (SPRT), si accorciano x/y/u/v/w/h. Serve alla cornice degli MP (0x29): nel gioco
  ha anche la linea verticale che la unisce al pannello del partner, che in verticale sta altrove.
- Barra del cursore (il pallino bianco nel quadratino della riga scelta): il gioco la copia VRAM→VRAM nella
  callback della vsync, cioè nel buffer già mostrato. In verticale la tela è una e si ridisegna tutta a ogni
  fotogramma, quindi la copia veniva coperta prima di essere vista (anche nella 2.2). Ora in verticale la callback
  fa solo avanzare l'animazione, e il task del cursore (`nativeCursorDot`, finché `GFX.vsyncFunc/vsyncArg` sono i
  suoi) disegna nel fotogramma un POLY_FT4 a 15 bit della stessa immagine (VRAM `frame * 12`, 0xF4, 12×12) sul layer
  MENUS a profondità 0, **scalato come la riga** (griglia dei comandi o pezzo della disposizione). Provato prima un
  gancio prima di `present_frame` con la copia del gioco: funziona ma il pallino resta 12×12 in un quadrato
  ingrandito.
- Liste (tecniche, oggetti, Tag, forme DV): `native_battle_list(cursor, f)` registra l'area della lista
  (`native_touch_list`); tocco su una riga → scelta e X; trascinamento di 36 pixel in su o a sinistra → R1 (pagina
  dopo), in giù o a destra → L1 (pagina prima). L1/R1 a schermo sono nascosti. Il trascinamento non si può simulare
  su Linux (DW3_PRESS ha solo tocchi).

## Altre schermate sulla tela alta (2.4)

- Resoconto (STFGTREP, modalità 0x1400, overlay a sé dopo la battaglia): scene.c crea SCREEN_LAYER sulla tela alta
  con offset (0, (H − 240) / 2) e l'origine del tocco; report.c ripete lo sfondo (fotogramma 0x1E, periodo 96) ogni
  192 righe su tutta l'altezza. `--field 5120` da solo resta nero: serve una battaglia vinta (`DW3_ENEMY_HP=1`).
- Dissolvenza comune (src/shared/draw_fader.inc.c, usata da memory card, negozi, laboratorio, album...): se il layer
  è sulla tela alta (clip.y ≥ NATIVE_FIELD_Y) copre `-offset` + tutto il clip invece di 320×256.
- Titolo (edge_fade.c): i quattro triangoli dal centro ai bordi, scalati sul clip del layer.
- Schermate con `shared/update_scene.inc.c`: basta `#define SCENE_UPRIGHT` (dentro `#ifdef NATIVE`) prima
  dell'include (tela alta, offset (0, (H − 240) / 2), origine del tocco), più lo sfondo che scorre (periodo 96)
  ridisegnato ogni 192 righe su tutta l'altezza. Fatto: memory card, menu del campo (card_screen.c + menu.c), negozio
  (scene.c + shop.c), laboratorio (scene.c + lab.c), album (album.c), negozio delle carte (scene.c + shop.c), editor
  del mazzo (scene.c + screen.c). La palestra (gym/scene.c) crea i suoi tre layer da sé: patch a mano.
- Per provarle: `--field` col numero della modalità (`--party 0`): 0x400 1024 editor del mazzo, 0xA00 2560
  palestra, 0xD00 3328 laboratorio, 0xF00 3840 negozio (`--arg` il negozio), 0x1200 4608 album, 0x1300 4864 negozio
  delle carte; il menu del campo dalla fascia (`--field 515 --progress 5`, Start).
- Battaglia: white_flash.c (lampo bianco) alto quanto la tela; digimon_change.c (digievoluzione) con CHANGE_H /
  CHANGE_STEP al posto di 240 e della velocità; sprite_effect.c sposta di (H − 240) / 2 gli effetti del layer FRONT.

## Menu col dito: la disposizione sulla scheda grafica (2.5)

- `native_layout_piece(layer, depth, sx, sy, sw, sh, dx, dy, dw, dh)` (gpu_draw.c, native.h): in questo fotogramma
  i pacchetti del layer, a profondità dell'OT sotto `depth`, la cui scatola sta (±2) nel rettangolo sorgente sono
  disegnati spostati e scalati nel rettangolo d'arrivo (dw/dh 0: nascosti). Vince il pezzo **più piccolo** che li
  contiene (il testo di una riga va con la riga, non con la cornice dietro). Coordinate del layer (quelle dei draw
  del gioco). `native_layout_end_frame` (display.c dopo i layer) svuota la tabella: ogni task rimette i suoi pezzi
  a ogni fotogramma, solo per gli elementi visibili. Solo la scheda grafica; il GPU software resta com'era.
- `NATIVE_MENU_PIECE` / `NATIVE_MENU_HIDE` (native.h): per le schermate con lo schermo 320×240 al centro della tela
  (SCENE_UPRIGHT), d'arrivo in righe della tela; profondità 7 = lo sfondo del menu resta fermo.
- `native_layout_unmap` riporta un punto toccato alle coordinate del gioco (per i tocchi dove serve).
- Fatti: menu del campo (engine/menu/field_menu.c: nel campo layer della mappa, profondità < 3, in alto; nelle
  schermate STSTATUS al centro), schermata oggetti (item_screen.c: partner, titolo, categorie, BIT, oggetto scelto,
  aiuto; tocchi su categorie e partner), lista oggetti (item_list.c: 16 righe in una colonna, cornice allungata, la
  sua linea interna nascosta, L1/R1 nascosti; tocchi con `native_touch_list`).
- Fatti anche: Ordina (sort_screen.c: partner, frecce, titolo, aiuto; tocchi sui partner), Stato (status_screen.c:
  partner, EXP, TP, scelte, statistiche, digievoluzioni, equipaggiamento; tocchi su partner e scelte).
- Fatti anche: equipaggiamento (equip_panel.c: titolo, caselle, oggetto della casella, lista, aiuto; lo `unk70` di
  StatsScreen dice a Stato di non spostare il TP mentre l'aiuto lo copre), digievoluzioni (digivolve_panel.c: titolo,
  caselle o domanda, statistiche che scorrono, aiuto), Tecniche (tech_screen.c: come Ordina più la lista).
- Negozio: fatta la prima schermata (shop.c: COMPRA/VENDI ×2, aiuto col tasto nascosto) e i tipi da vendere
  (trade.c: panels[0] ×2, il messaggio panels[4] ×1,5).
  Lista per comprare (item_list.c, `selling == 0`, 8 per pagina): un pezzo per oggetto (la sua riga nella sua
  colonna, sorgente alta 16) alla riga i ×2, nell'ordine naturale; cornice allungata, pagina sotto, L1/R1 nascosti,
  dettagli ×1 sotto; tocchi in `nativeShopListTouch` (righe, frecce 20/21 → L1/R1, suggerimento 9 → cerchio).
  Acquisto (trade.c `nativeBuyLayout`): quantità panels[0] ×1,8, totale e messaggi panels[1-3] ×1,5, dettagli come
  con la lista. Le icone dei tasti nei testi (gpu_draw.c `rect`, controls_glyph) ora seguono anche la y della
  disposizione (prima solo la x: il ● restava al posto originale). Resta un pezzetto di cornice a destra (circa x 290-320,
  y 130-150 del gioco) accanto al messaggio: da capire di quale sprite è. Da fare: lista (item_list.c, due colonne
  con L1/R1 a schermo), quantità e conferma (trade.c), informazioni (info.c), equip.c; percorso di prova
  `--field 3840 --party 0`, `250:start,700:t160y358` (VENDI), poi `t160y374` (Protezioni). Poi laboratorio, palestra.

## Demo da disporre: menu del campo (artefatto "Disposizione menu", 18/11/2026)

- https://claude.ai/artifact/BPppfTLQDz7YC1RCv7FwYG — sei schede: Menu, Oggetti: categorie, Oggetti: lista, Oggetti: a
  chi, Ordina, Stato. Stesso modello della battaglia (db `layouts`, un documento per scheda); i pannelli dei partner
  sono della scheda «Oggetti: categorie» e in «A chi» / «Ordina» sono riferimenti.
- Si ricostruisce con `tools/demo/build_disposizione_menu.py OUT.html` dalle catture in orizzontale (3×) mpA, mpB, mpE
  (`--field 515 --progress 5`; oggetti di prova con `--set 802B=1` ... `--set 803C=1`: gli usabili partono da 0x2B,
  ITEM_LISTS in engine/game/items.c) e dallo sfondo verticale di fm2. Mancano le liste delle tecniche (la squadra di
  prova non ne ha) e la scheda "salva".
- Poi: applicare le posizioni come per la battaglia (trasformazione per pezzo) e dare i tocchi alle righe.

## Demo da disporre (artefatto "Disposizione battaglia")

- https://claude.ai/artifact/3DVyDQZggmhr1isbDtUXnL — l'utente trascina e ridimensiona i pezzi veri della grafica
  della battaglia su uno schermo 320×710 (schede Comandi, Tecniche, DV, Messaggi, Tag, Oggetti) e scrive note.
- Le posizioni sono nel db dell'artefatto: `ArtifactData list` della collezione `layouts`, un documento per scheda
  (`pieces.<id> = {x, y, w, h, hidden, z}` in pixel del gioco, `note`).
- Si ricostruisce con `tools/demo/build_disposizione.py OUT.html` (pezzi ritagliati da schermate a 3× in
  orizzontale, sfondo pulito in verticale con `DW3_HIDE_LAYERS=1005,1007,1009`); l'HTML prodotto contiene grafica
  del gioco: mai nel repository.
