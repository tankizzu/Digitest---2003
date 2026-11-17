# Istruzioni per Claude — Digimon World 2003 per Android

Port nativo Android di *Digimon World 2003* (PS1, EU SLES-03936, italiano e inglese), solo touch.
Si parla in **italiano** con l'utente.

## A inizio sessione (sempre)

1. Leggi questo file, poi `PROJECT_STATE.md`, `TODO.md`, `DECISIONS.md`.
2. Per i dettagli tecnici: `TECHNICAL_NOTES.md`. Per la cronologia delle modifiche: `CHANGELOG.md` (sezione Android in cima).
3. Controlla `git status` / `git log -5` e che il codice corrisponda allo stato scritto.
4. Non contraddire una decisione di `DECISIONS.md` senza spiegarne il motivo e chiedere conferma.

## Regole non negoziabili

- Nel repository **solo codice**: mai il `.bin`/`.cue`, i dati estratti, gli eseguibili o le librerie compilate,
  i salvataggi, le cartelle `build-*` (contengono parti del gioco). Controlla `git status` prima di ogni commit.
- L'APK non va pubblicato né condiviso: si manda solo all'utente.
- Il gioco è pensato **prima in verticale**; l'orizzontale è un di più.
- Coerenza grafica: ogni elemento nuovo usa la grafica del gioco (sprite, finestre, font del gioco).
  Niente pulsanti "web" o stili inventati. Niente tasti PlayStation a schermo.
- Non dichiarare verificato ciò che non è stato provato. Distinguere sempre: verificato su Linux,
  verificato sul telefono (solo l'utente può), non verificato.
- Modifiche importanti, architetturali o distruttive (es. riscrivere la storia git, cambiare il layout
  generale): prima un piano, poi l'approvazione dell'utente.
- Fedeltà al gioco originale: non sostituire meccaniche senza autorizzazione.

## A fine sessione / prima di una compattazione

Aggiorna `PROJECT_STATE.md` (stato, cosa è verificato, da dove ripartire), `TODO.md`, `DECISIONS.md` se sono
state prese decisioni, `CHANGELOG.md`, e `TECHNICAL_NOTES.md` per le scoperte tecniche. Poi commit e push.

## Come si lavora

- Le modifiche al codice del gioco sono **patch** su dw3_decomp, in `patches/src/...`. Si modificano con
  `python3 tools/unix/patchedit.py apply src/<percorso>.c FILE_EDITS.py` (lista `EDITS` di coppie vecchio/nuovo,
  sul file già patchato). Tutto il codice nuovo dentro `#ifdef NATIVE`.
- Il codice della piattaforma (touch, salvataggi, rendering, preferenze) è in `platform/`; le dichiarazioni in
  `include/native.h`.
- Compilare per Linux: `python3 build_unix.py` → `build-linux/dw2003`.
- APK: `ANDROID_HOME=/home/user/android-sdk ANDROID_NDK=/home/user/android-sdk/ndk/27.2.12479018 tools/android/build_apk.sh`
  → `build-android/dw2003.apk`. Alzare `versionCode`/`versionName` in `android/app/build.gradle` a ogni consegna.
- Provare senza telefono: vedi "Test su Linux" in `TECHNICAL_NOTES.md` (shoot.sh, DW3_PRESS, DW3_TOUCH,
  DW3_STICK, DW3_TYPE). I salvataggi rapidi F5/F9 valgono **solo per la build che li ha creati**: per i test
  lunghi usare il salvataggio automatico (Continua → slot → Automatico).
