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
- [x] 1. VRAM del port 1024×1024 (come la GPU con 2 MB; righe ≥ 512 solo per il port). Verificato: menu e
      caricamento identici pixel per pixel, campo regolare. Il backend Direct3D 12 (solo Windows) non è aggiornato.
- [x] 2. Campo con un buffer 320×512 a (0,512) (le righe 496–511 hanno le CLUT del gioco: niente 640). Prova su
      Linux con DW3_PORTRAIT=1: stanza della Sala della Matrice intera, oggetti animati, dialogo, camminata.
- [x] 3. Mappa: 27 immagini (CLUT in alto, y 896+), fino a 5 righe disegnate, stream sulle righe in vista.
- [x] 4. Telecamera centrata su 512 righe; le stanze più basse restano al centro col nero (deciso).
- [ ] 5. Fascia in alto: Impostazioni, Menu, stato dei partner (grafica del gioco). Fascia in basso: dialoghi
      e comandi. Banner, transizioni tra stanze e inizio battaglia con le misure nuove.
- [x] 6. Android in verticale (1.2 anteprima, 1.3 a tutto schermo).
- [x] 7. Freccia dei dialoghi: i trattini indicano la velocità (A).
- [ ] 8. Demo da discutere: battaglia verticale (comandi grandi in basso), fascia alta (partner, MENU, OPZIONI),
      schermata Opzioni.

## (vecchio) verticale

- [ ] Piano per il **verticale come modalità principale**: campo a tutto schermo con più mappa sopra e sotto,
      menu e battaglia ridisposti con la grafica del gioco, orizzontale come opzione. Il manifest oggi è
      sensorLandscape.

## P1 — menu iniziali a tutto schermo (fase 2 del verticale, in corso)

- [x] Titolo (sfondo ingrandito, contenuto al centro, tocchi con l'origine).
- [x] Titolo: pulsanti grandi, copyright in fondo, ingranaggio impostazioni (lingua).
- [x] Memory card a tutto schermo: 10 slot in griglia con icona, Elimina, CARICA nel riquadro di metallo.
- [ ] Strada di Asuka all'inizio del prologo: ancora 4:3 (imposta lo schermo per conto suo? da trovare).
- [ ] Volume nelle impostazioni (schermata impostazioni con la grafica del gioco).
- [ ] Lingua, nome, kit, conferma a tutto schermo.

- [ ] Elenco MEMORY CARD a tutto schermo (l'utente: scorrere è ancora complesso; a tutto schermo diventa più
      semplice).

- [ ] Titolo a tutto schermo, pulsante Impostazioni (lingua), pulsanti più grandi: solo con la grafica del
      gioco. Rifare le proposte partendo da schermate reali e sprite del gioco, senza sfondi finti né pulsanti
      sovrapposti al titolo.

## P2 — altro

- [ ] Verificare sul telefono il salvataggio automatico quando l'app va in secondo piano.
- [ ] Schermate del menu del campo (squadra, oggetti, tecniche, mappa), negozi, laboratorio.

## P3

- [ ] Battaglia: ridisposizione per il verticale.
- [ ] Mappa 2D estesa a tutto schermo (task 9).
