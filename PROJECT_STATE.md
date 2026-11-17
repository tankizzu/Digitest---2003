# Stato del progetto

Ultimo aggiornamento: 17/11/2026 — versione **0.9** (versionCode 12) consegnata all'utente.

## Obiettivo attuale

Prologo perfetto fino alla prima battaglia "tutorial", tutto col touch e con la grafica del gioco; poi la
battaglia. In parallelo: piano per il verticale come modalità principale (da approvare).

## Funziona

| Cosa | Linux | Telefono |
|---|---|---|
| Compilazione Linux e APK arm64, lettura del disco dal telefono | sì | sì (dalla 0.3) |
| Filmati saltati tenendo premuto; nessun tasto PlayStation a schermo | sì | da riverificare |
| Lingua ricordata, titolo e opzioni toccabili | sì | sì |
| Nome con la tastiera del telefono, OK sotto il nome | sì | da riverificare |
| Scelta del kit: linguette A/B/C + OK; crash risolto (0.8) | sì | no |
| Conferma "Registri l'account?" con Sì/No toccabili | sì | — |
| Più partite, "MEMORY CARD N", Continua → Automatico/Salvataggi | sì | no |
| Salvataggio automatico ogni 2 minuti mentre si cammina, posizioni alternate | sì | no |
| Salvataggio quando l'app va in secondo piano | no (solo su telefono) | no |
| Dialoghi: tocco, tenere premuto, automatici x1/x2/x3 con la freccia del gioco | sì | parziale |
| Levetta mobile solo dove si cammina; spinta leggera cammina | sì | no |
| Corsa al 58% | compilato | no |
| Porte automatiche con la levetta | sì | no |
| Fumetto vicino ai personaggi, tocco per parlare (corretto nella 0.9) | sì | no |
| Indietro in basso a sinistra | compilato | no |
| Battaglia: comandi, tecniche, oggetti, messaggi, risultati toccabili | sì (vecchie versioni) | parziale |

## Problemi aperti

- Il gioco è ancora solo in orizzontale (manifest sensorLandscape); il verticale richiede di far disegnare al
  campo un'area più alta di 320×240 (piano da preparare e approvare).
- I menu iniziali vanno rivisti con l'utente, solo con la grafica del gioco (la demo del 17/11 è stata scartata).
- `build-linux-gles/` è nella storia git (commit 05c507c): tolto dalla versione attuale, la riscrittura della
  storia aspetta il sì dell'utente.
- Il repository su GitHub è stato rinominato `tankizzu/Digitest---2003` (il vecchio indirizzo reindirizza).

## Da dove ripartire

1. Raccogliere il riscontro dell'utente sulla 0.9 (corsa, porte, fumetto, kit, Indietro).
2. Preparare il piano del verticale (TODO P1) e presentarlo per l'approvazione.
3. Percorrere il prologo fino alla battaglia tutorial e segnare ogni punto senza touch.
4. Test: la partita "MEMORY CARD 9" dell'ambiente di prova ha un salvataggio automatico nella stanza della
   Sala della Matrice, con un personaggio vicino (vedi TECHNICAL_NOTES, "Test su Linux").
