# Digimon World 2003 per Android

Port nativo per Android di *Digimon World 2003* (PlayStation, versione europea SLES-03936),
con comandi solo touch e gioco in verticale a tutto schermo.

Parte da [dw3-native-port](https://github.com/Feerdus95/dw3-native-port) di Feerdus95 (licenza MIT),
che a sua volta usa il codice C della decompilazione [dw3_decomp](https://github.com/ReGame-Labs/dw3_decomp).
Il README originale del port è in [README.md](README.md).

**Il gioco non è incluso.** Serve l'immagine del proprio disco: *Digimon World 2003 (Europe)
(En,Fr,De,Es,It)*, `.bin` da 692.146.560 byte. In questo repository va solo il codice:
mai il `.bin`, i dati estratti o il binario compilato, che contiene parti del gioco.

## Documentazione del progetto

- [PROJECT_STATE.md](PROJECT_STATE.md): stato attuale, cosa è verificato, da dove ripartire.
- [DECISIONS.md](DECISIONS.md): decisioni approvate (comandi, interfaccia, salvataggi).
- [TODO.md](TODO.md): attività aperte e priorità.
- [TECHNICAL_NOTES.md](TECHNICAL_NOTES.md): architettura, formati, test su Linux, problemi risolti.
- [CHANGELOG.md](CHANGELOG.md): modifiche (sezione Android in cima).
- [CLAUDE.md](CLAUDE.md): regole di lavoro.

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
