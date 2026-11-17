# Stato del progetto

Ultimo aggiornamento: 17/11/2026 — versione **1.6** (versionCode 19) consegnata all'utente.

## Obiettivo attuale

Prologo perfetto fino alla prima battaglia "tutorial", tutto col touch e con la grafica del gioco; poi la
battaglia. In parallelo: verticale a tutto schermo come modalità principale (direzione approvata, piano in
TODO P1).

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
| Elenco MEMORY CARD grande con data/ora, trascinamento, tenere premuto per eliminare (1.0) | aspetto e scelta sì; trascinamento ed eliminazione no | no |
| Sì/No della registrazione in due riquadri (1.0) | sì | no |
| Salvataggio automatico ogni 2 minuti mentre si cammina, posizioni alternate | sì | no |
| Salvataggio quando l'app va in secondo piano | no (solo su telefono) | no |
| Dialoghi: tocco, tenere premuto, automatici x1/x2/x3 con la freccia del gioco | sì | parziale |
| Levetta mobile solo dove si cammina; spinta leggera cammina | sì | no |
| Corsa al 58% | compilato | no |
| Porte automatiche con la levetta | sì | no |
| Fumetto vicino ai personaggi (scarabocchi, 1.0), tocco per parlare | sì | no |
| Indietro in basso a sinistra | compilato | no |
| Battaglia: comandi, tecniche, oggetti, messaggi, risultati toccabili | sì (vecchie versioni) | parziale |

## Problemi aperti

- Il gioco è ancora solo in orizzontale (manifest sensorLandscape); il verticale richiede di far disegnare al
  campo un'area più alta di 320×240.
- I menu iniziali vanno rivisti con l'utente, solo con la grafica del gioco (la demo del 17/11 è stata scartata).
- Il repository su GitHub è stato rinominato `tankizzu/Digitest---2003` (il vecchio indirizzo reindirizza).
- Dialoghi automatici che si fermano con più dialoghi di fila: non riprodotto su Linux (il prologo va avanti da
  solo con x2); chiedere all'utente il punto esatto.

## Da dove ripartire

1. Raccogliere il riscontro dell'utente sulla 1.0.
2. **Verticale a tutto schermo** (approvato come direzione il 17/11/2026): prima fase descritta in TODO P1.
3. Percorrere il prologo fino alla battaglia tutorial e segnare ogni punto senza touch.
4. Test: la partita "MEMORY CARD 9" dell'ambiente di prova ha un salvataggio automatico nella stanza della
   Sala della Matrice, con un personaggio vicino (vedi TECHNICAL_NOTES, "Test su Linux").
