# Da fare

Priorità: P1 subito, P2 dopo, P3 più avanti. Obiettivo attuale: **prologo perfetto fino alla prima battaglia
"tutorial"**, poi la battaglia.

## P1 — prologo

- [ ] Verificare sul telefono la 1.0 (anche elenco MEMORY CARD: trascinare, tenere premuto per eliminare; Sì/No)
      e la 0.9: crash del kit risolto, pulsante OK del kit, Indietro in basso a sinistra,
      "MEMORY CARD N", porte automatiche, corsa più lenta, fumetto vicino ai personaggi.
- [x] Fumetto dei personaggi: verificato su Linux (0.9).
- [ ] Percorrere il prologo dal nuovo gioco fino alla battaglia tutorial e annotare ogni punto in cui serve un
      tasto o ci si blocca.
- [ ] Rivedere con l'utente: scale, salti e minigiochi automatici come le porte?
- [ ] Dialoghi automatici che si fermano con più dialoghi di fila: farsi dire il punto (su Linux non succede).

## P1 — verticale, fase 1 (approvata il 17/11/2026): il campo a tutto schermo

Progetto in TECHNICAL_NOTES ("Verticale, fase 1"). Passi:
- [ ] 1. VRAM del port 1024×1280 (righe alte solo per il port), test che tutto il resto resti uguale.
- [ ] 2. Campo con un buffer 320×640 a (0,512).
- [ ] 3. Mappa: 27 immagini, 4×6 pezzi disegnati, stream 4×7.
- [ ] 4. Telecamera centrata su 640 righe; stanze piccole ingrandite.
- [ ] 5. Dialoghi/banner/fumetti riposizionati.
- [ ] 6. Android in verticale; screenshot del prologo all'utente.

## (vecchio) verticale

- [ ] Piano per il **verticale come modalità principale**: campo a tutto schermo con più mappa sopra e sotto,
      menu e battaglia ridisposti con la grafica del gioco, orizzontale come opzione. Il manifest oggi è
      sensorLandscape.

## P2 — menu iniziali (da discutere con l'utente)

- [ ] Titolo a tutto schermo, pulsante Impostazioni (lingua), pulsanti più grandi: solo con la grafica del
      gioco. Rifare le proposte partendo da schermate reali e sprite del gioco, senza sfondi finti né pulsanti
      sovrapposti al titolo.

## P2 — altro

- [ ] Verificare sul telefono il salvataggio automatico quando l'app va in secondo piano.
- [ ] Schermate del menu del campo (squadra, oggetti, tecniche, mappa), negozi, laboratorio.

## P3

- [ ] Battaglia: ridisposizione per il verticale.
- [ ] Mappa 2D estesa a tutto schermo (task 9).
