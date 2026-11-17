# Stato del progetto

Ultimo aggiornamento: 17/11/2026 — versione **2.1** (versionCode 24) pronta (2.0 già mandata); l'ultima provata
dall'utente sul telefono è precedente alla 1.9.

## Obiettivo attuale

Prologo perfetto fino alla prima battaglia "tutorial", tutto col touch e con la grafica del gioco, **in verticale a
tutto schermo** (modalità principale); poi la battaglia.

## Funziona

| Cosa | Linux | Telefono |
|---|---|---|
| Compilazione Linux e APK arm64, lettura del disco dal telefono | sì | sì (dalla 0.3) |
| Filmati in 4:3, saltati tenendo premuto; nessun tasto PlayStation a schermo | sì | da riverificare |
| Campo in verticale a tutto schermo (tela 320 × forma del telefono), personaggio al centro | sì | no (dalla 1.3) |
| Titolo in verticale: pulsanti grandi, copyright in fondo | sì | no |
| Impostazioni col pulsante del gioco (ingranaggio "Macchina" nel pannellino, 2.0): apre la lingua | sì | no |
| Memory card in verticale: 5 slot, Carica/Elimina, Automatico/Salvataggi, slot pieni → sostituzione | sì | no |
| Indietro: freccia del gioco (nel riquadro di metallo della memory card e nell'angolo) | sì | no |
| Nome della memory card al primo salvataggio (2.0) | sì (verticale) | no |
| Sì/No affiancati: storia, registrazione, memory card originale (nuovo file, sovrascrittura, salvare qui) | sì (verticale) | no |
| Automatico: lista dei due autosalvataggi coi dati del gioco; immagine del campo nelle liste (2.0) | sì (verticale) | no |
| Nome con la tastiera del telefono, OK sotto il nome; kit con linguette + OK | sì | da riverificare |
| Lingua, nome, kit e conferma a tutto schermo in verticale (2.0) | sì | no |
| Fascia alta nel campo: partner (nome, HP) e MENU (2.1) | sì | no |
| Menu del campo senza tasti PlayStation nei testi (2.1) | sì (EN) | no |
| Salvataggio automatico ogni 2 minuti mentre si cammina, posizioni alternate | sì | no |
| Salvataggio quando l'app va in secondo piano | no (solo su telefono) | no |
| Dialoghi: tocco, tenere premuto, automatici x1/x2/x3 (trattini della freccia) | sì | parziale |
| Levetta mobile, corsa al 58%, porte automatiche, fumetto vicino ai personaggi | sì | no |
| Battaglia: comandi, tecniche, oggetti, messaggi, risultati toccabili (in orizzontale) | sì (vecchie versioni) | parziale |

Orizzontale: identico pixel per pixel ai riferimenti dopo ogni modifica (tranne il pannello impostazioni nuovo sul
titolo).

## Da decidere con l'utente

- **Impostazioni**: tre proposte (A icona sola, B icona nel pannellino — quella attiva —, C pannello sotto
  CONTINUA). `DW3_SETTINGS_LOOK=1/2/3` per vederle.
- **Strada di Asuka**: non è in 4:3, la mappa è alta solo 384 pixel; lasciare il nero (decisione attuale),
  ingrandire x1,4, o ingrandire fino a riempire.
- Rinominare una memory card dopo il primo salvataggio (oggi non si può).
- Fascia alta: scelte di partenza (sopra la mappa, solo col giocatore in controllo, prima di Genji solo i
  partner); OPZIONI quando ci sarà la schermata impostazioni.

## Problemi aperti

- Tutto ciò che è dalla 1.3 in poi non è stato provato sul telefono (immersivo, area sicura, verticale).
- Costo sul telefono della cattura delle immagini (ogni ~4 s il GPU software disegna un fotogramma).
- Dialoghi automatici che si fermano con più dialoghi di fila: non riprodotto su Linux.
- Il repository su GitHub è `tankizzu/Digitest---2003` (il vecchio indirizzo reindirizza).

## Da dove ripartire

1. Riscontro dell'utente sulla 2.1 (APK in `build-android/dw2003.apk`, da mandare solo a lui).
2. Le tre scelte qui sopra.
3. In coda: volume nelle impostazioni (serve una schermata impostazioni: da progettare con l'utente); battaglia in
   verticale (statistiche affiancate in alto, comandi grandi sopra la scena); fascia alta del campo (partner, MENU,
   OPZIONI); orologio facoltativo.
4. Test: percorsi di prova in TECHNICAL_NOTES ("Test su Linux"); salvataggi di prova in `saves/game5` e `game9`
   (copia di riserva prima di una prova che salva).
