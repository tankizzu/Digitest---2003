# Digimon World 2003 per Android

Port nativo per Android di *Digimon World 2003* (PlayStation, versione europea SLES-03936),
con comandi solo touch e gioco in verticale a tutto schermo.

Parte da [dw3-native-port](https://github.com/Feerdus95/dw3-native-port) di Feerdus95 (licenza MIT),
che a sua volta usa il codice C della decompilazione [dw3_decomp](https://github.com/ReGame-Labs/dw3_decomp).
Il README originale del port è in [README.md](README.md).

**Il gioco non è incluso.** Serve l'immagine del proprio disco: *Digimon World 2003 (Europe)
(En,Fr,De,Es,It)*, `.bin` da 692.146.560 byte. In questo repository va solo il codice:
mai il `.bin`, i dati estratti o il binario compilato, che contiene parti del gioco.

## Stato

| Passo | Stato |
|---|---|
| Compilazione per Linux a 64 bit e OpenGL ES (`build_unix.py`, `--gles`) | Fatto |
| APK Android arm64 (`tools/android/build_apk.sh`), lettura del disco dal telefono | Fatto (0.3) |
| Comandi touch: levetta mobile, filmati saltati tenendo premuto | Fatto |
| Tocco nei menu: lingua (ricordata), titolo, dialoghi, Sì/No, nome, kit, menu del campo | Fatto |
| Battaglia: comandi, tecniche, oggetti, cambio, messaggi, risultati | Fatto |
| Pulsanti ☰ (menu) e ‹ (indietro), gesto Indietro di Android | Fatto |
| Salvataggio automatico (uscita dall'app, ogni 3 minuti) con ripresa al titolo | Fatto |
| Schermate del menu (squadra, oggetti, tecniche, mappa), negozi, memory card, laboratorio | Da fare |
| Verticale a tutto schermo con la mappa 2D estesa | Da fare |
| Togliere i tasti provvisori ✕ ○ △ □ | Quando tutte le schermate saranno toccabili |

## Scelte di interfaccia decise

- Esplorazione a tutto schermo anche in verticale: dove la mappa lo permette si mostra più mappa
  sopra e sotto, nelle stanze piccole si ingrandisce quanto basta a riempire lo schermo.
- Levetta analogica che compare dove si tocca e sparisce quando si alza il dito; la corsa
  dipende da quanto si spinge. Nessuna rotazione della telecamera (il gioco non ce l'ha).
- Nessun tasto a schermo: vicino a un personaggio compare un fumetto da toccare per parlare.
- Dialoghi: un tocco va avanti, tenere premuto fa scorrere il testo veloce.
- In alto a sinistra la squadra con nome, livello, HP ed esperienza; in alto a destra il menu.
- Menu e battaglia mantengono la grafica originale, ridisposta per il verticale.

## Compilare su Linux

Cartelle affiancate:

    qualche-cartella/
      dw3_decomp/        git checkout 791932b757b358bb8228f851ce96f4d22ae112dd
      native/            questo repository
      sdl3-linux/        SDL 3.4.16 installato (cmake -DSDL_X11_XTEST=OFF)

Poi:

    python3 native/build_unix.py
    DW3_DATA=gamedata native/build-linux/dw2003 --extract "Digimon World 2003 (Europe).cue"
    DW3_DATA=gamedata native/build-linux/dw2003

`tools/unix/shoot.sh` avvia il gioco senza schermo (Xvfb), con i tasti scritti in `DW3_PRESS`, e
salva le immagini dei fotogrammi: serve a provare il gioco su un server.

## Note tecniche sui 64 bit

- Il codice del gioco è compilato con `-Dlong=int`: su PlayStation e su Windows il `long` è di 32
  bit, su Linux e Android di 64. `patches/include/common.h.patch` tiene `s64`/`u64` a 64 bit.
- Le funzioni PsyQ della piattaforma che scambiano `long` con il gioco usano `psx_long`.
- Il gioco salva indirizzi in variabili a 32 bit: tutta la sua memoria deve stare sotto i 4 GB.
  Su Linux basta un eseguibile non PIE. Su Android la libreria del gioco andrà caricata a un
  indirizzo basso con `android_dlopen_ext` e `ANDROID_DLEXT_RESERVED_ADDRESS`.
