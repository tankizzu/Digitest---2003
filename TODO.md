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
- [x] 5. Fascia in alto: partner (nome, HP) e MENU con la grafica del gioco (2.1). Manca OPZIONI (serve la
      schermata impostazioni con volume, da progettare con l'utente).
- [x] Testi del menu del campo (file STATUS) senza tasti PlayStation (2.1).
- [ ] Altri testi con "Tasto": CARD_ALBUM (40), CARD_SHOP (54), DIGI_LAB (61), DIGIMON_NAMING (68), ITEM_SHOP (117),
      TRAINING (271) (fid italiani). Alcuni indicano funzioni del tasto □ (dettagli, effetti, ordina): riscriverli
      quando quelle schermate avranno i loro tocchi.
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
- [x] Memory card a tutto schermo: 5 slot, Carica/Elimina, Automatico/Salvataggi, Sì/No affiancati, slot pieni →
      scelta di quale sostituire.
- [x] Impostazioni con l'ingranaggio del gioco (3 proposte, B di partenza): **far scegliere all'utente** e togliere
      le altre (`DW3_SETTINGS_LOOK`).
- [x] Nome della memory card al primo salvataggio (tastiera del telefono, font del gioco). Da provare in orizzontale
      e sul telefono; manca un modo per rinominare (da chiedere all'utente se serve).
- [x] Salvataggi e Autosalvataggio: liste con immagine (screenshot al salvataggio) e dati. Da provare sul telefono
      (costo della cattura ogni ~4 s: un fotogramma disegnato anche dal GPU software).
- [x] Sì/No del gioco nelle schermate memory card originali (sovrascrittura, formattazione, nuovo file, "salvare
      qui") affiancati. Da provare sul telefono.
- [ ] Eventuale orologio in sovraimpressione (l'utente: "nel caso").
- [x] Strada di Asuka all'inizio del prologo: **non è 4:3**, è la mappa che finisce (MODE_NEW_GAME 0x2D7, mappa 5×3
      riquadri = 640×384 px; in verticale se ne vede tutta l'altezza, sopra e sotto nero come deciso per le stanze
      piccole). **Chiedere all'utente**: lasciare il nero (A), ingrandire x1,4 (B) o fino a riempire (C) solo nelle
      mappe basse (anteprima da screenshot reale; contraddice la decisione "dove la mappa finisce resta il nero").
- [ ] Strada di Asuka a tutto schermo (richiesta dell'utente): prototipo spento (`DW3_ZOOM_MAP=1`), verificato su
      Linux. Da approvare; poi accenderlo, provare scelte Sì/No e fumetto nella strada, la miniatura del salvataggio
      (GPU software senza ingrandimento).
- [ ] Volume nelle impostazioni (schermata impostazioni con la grafica del gioco).
- [x] Lingua a tutto schermo (sfondo su tutta l'altezza; righe alte 31 px: ingrandirle se l'utente le trova piccole).
- [x] Nome, kit, conferma a tutto schermo (sfondo a esagoni ripetuto in altezza). Verificato su Linux.

- [ ] Elenco MEMORY CARD a tutto schermo (l'utente: scorrere è ancora complesso; a tutto schermo diventa più
      semplice).

- [ ] Titolo a tutto schermo, pulsante Impostazioni (lingua), pulsanti più grandi: solo con la grafica del
      gioco. Rifare le proposte partendo da schermate reali e sprite del gioco, senza sfondi finti né pulsanti
      sovrapposti al titolo.

## P2 — altro

- [ ] Verificare sul telefono il salvataggio automatico quando l'app va in secondo piano.
- [x] Menu del campo a tutto schermo in verticale (2.4): contenuto al centro, sfondo ripetuto.
- [x] Negozio, laboratorio, palestra, album e negozio delle carte a tutto schermo in verticale (2.4).
- [x] Editor del mazzo a tutto schermo (2.4).
- [ ] **Tocchi** nelle schermate interne del menu del campo, negozio, laboratorio, palestra, carte (oggi solo
      Indietro): decidere con l'utente fra tocco generico sul cursore e bersagli per schermata (PROJECT_STATE).
- [ ] Battaglia a carte (CARDGAME) in verticale: un solo layer (0x100, fondo grigio). Aperta da sola (`--field 1792`)
      va in crash (manca il mazzo/avversario): per provarla serve una partita a carte vera da un salvataggio.
- [ ] Testi coi tasti PlayStation ancora a schermo in negozio ("Button: Back", "Button: Effects", "R1"), album,
      negozio delle carte, laboratorio, palestra: rifarli quando quelle schermate avranno i loro tocchi.

## P1 — battaglia in verticale (2.2 prima versione, 2.3 disposizione dell'utente)

- [x] Tela alta, scena 3D a tutta altezza, statistiche in alto, volto del partner sotto, comandi 2×3 grandi in basso.
- [ ] Provare sul telefono (l'utente): 2.3, soprattutto il trascinamento per cambiare pagina nelle liste.
- [x] Disposizione dell'utente dalle demo (2.3): HUD, volto, messaggi, Tecniche, Oggetti, DV, Tag; niente L1 / R1
      nelle liste, tocco per scegliere, trascinamento per la pagina. Verificato su Linux (aspetto e tocchi).
- [x] Menu di entrata del partner (quando uno va KO): statistiche dove l'utente ha messo quelle del DV, "Nome /
      Switch" in basso a destra. Verificato su Linux; con la tecnica di coppia (cornice MP) non provato.
- [x] Lampo bianco a tutto schermo; digievoluzione su tutta l'altezza (2.4; la digievoluzione non provata).
- [x] Resoconto di fine battaglia (STFGTREP) a tutto schermo: sfondo su tutta l'altezza, contenuto al centro (2.4).
      Se l'utente lo vuole più grande o ridisposto: una demo da disporre come per la battaglia.
- [ ] Inquadrature ravvicinate: nero oltre il set 3D (vedi PROJECT_STATE, da decidere).
- [ ] Inizio e fine battaglia (transizione, risultati, resoconto STFGTREP) a tutto schermo.

## P3
- [ ] Mappa 2D estesa a tutto schermo (task 9).
