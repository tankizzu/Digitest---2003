# Stato del progetto

Ultimo aggiornamento: 18/11/2026 — versione **2.4** (versionCode 27) mandata il 18/11; in preparazione la 2.5; l'ultima
provata dall'utente sul telefono è precedente alla 1.9.

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
| Battaglia in verticale: scena alta, statistiche in alto, comandi 2×3 grandi (2.2) | sì | no |
| Battaglia in verticale con la disposizione dell'utente (HUD, volto, messaggi, Tecniche, Oggetti, DV, Tag) e pallino del cursore (2.3) | sì | no |
| Liste della battaglia: trascinare per cambiare pagina (2.3) | no (non si simula) | no |
| Battaglia verticale: lampo bianco a tutto schermo, menu di entrata del partner (2.4) | sì | no |
| Menu del campo (oggetti, mappa, tecniche, stato...) a tutto schermo (2.4) | sì | no |
| Negozio, laboratorio, palestra, album e negozio delle carte a tutto schermo (2.4) | sì (aperti, menu) | no |
| Menu del campo col dito: menu, oggetti (lista in una colonna), Ordina, Stato (2.5) | sì (lista: trascinamento no) | no |
| Salvataggio automatico ogni 2 minuti di gioco e all'uscita dall'app (2.5) | a tempo sì | no |
| Resoconto di fine battaglia a tutto schermo; dissolvenze del titolo e dei menu su tutta la tela (2.4) | sì | no |
| Digievoluzione in battaglia su tutta l'altezza (2.4) | no (la squadra di prova non digievolve) | no |
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
- Rinominare una memory card dopo il primo salvataggio (oggi non si può).
- Fascia alta: scelte di partenza (sopra la mappa, solo col giocatore in controllo, prima di Genji solo i
  partner); OPZIONI quando ci sarà la schermata impostazioni.

## Problemi aperti

- **Senza tocchi** (sul telefono si usa solo Indietro): le schermate interne del menu del campo (oggetti, ordina,
  tecniche, stato; il menu principale invece sì), negozio, laboratorio, palestra, album/negozio/editor delle carte.
  Deciso (18/11): righe grandi disposte dall'utente con demo, schermata per schermata.

- Tutto ciò che è dalla 1.3 in poi non è stato provato sul telefono (immersivo, area sicura, verticale).
- Costo sul telefono della cattura delle immagini (ogni ~4 s il GPU software disegna un fotogramma).
- Dialoghi automatici che si fermano con più dialoghi di fila: non riprodotto su Linux.
- Il repository su GitHub è `tankizzu/Digitest---2003` (il vecchio indirizzo reindirizza).

## Da dove ripartire

1. Riscontro dell'utente sulla 2.4 (mandata il 18/11): battaglia in verticale, trascinamento nelle liste, schermate
   a tutto schermo.
2. Menu col dito (l'utente: "pulsanti grandi e liste da scorrere", libertà di migliorare la sua disposizione):
   fatti menu, oggetti, Ordina (ora al centro dello schermo, come chiesto), Stato, equipaggiamento; mancano digievoluzioni, Tecniche, poi negozio, laboratorio,
   palestra (TECHNICAL_NOTES "Menu col dito").
3. Menu di entrata del partner dopo Tag (switch_in_menu.c) e risultati della battaglia in verticale.
4. Le scelte qui sopra.
5. In coda: volume nelle impostazioni (serve una schermata impostazioni: da progettare con l'utente); OPZIONI
   nella fascia alta del campo; orologio facoltativo.
6. Test: percorsi di prova in TECHNICAL_NOTES ("Test su Linux"); salvataggi di prova in `saves/game5` e `game9`
   (copia di riserva prima di una prova che salva).
