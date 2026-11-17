# Decisioni approvate

Ogni voce: la decisione, il motivo, le alternative scartate. Le decisioni si cambiano solo con l'utente.

## Generali

- **Port nativo, non emulatore.** Si parte da dw3-native-port (Feerdus95, MIT) sul codice C di dw3_decomp.
  Motivo: tutto schermo, comandi touch e interfaccia rifatta richiedono il codice. Scartato: emulatore PS1 con
  overlay (non permette di cambiare il gioco).
- **Repository solo codice.** Mai dati del gioco, binari, salvataggi. Il disco dell'utente viene letto sul
  telefono.
- **Prima in verticale, l'orizzontale è un di più** (ribadito il 17/11/2026).
- **Coerenza grafica con il gioco** prima di tutto: pulsanti e finestre nuove usano sprite, finestre e font del
  gioco. Scartata (17/11/2026) la demo dei menu con pulsanti in stile "app" (blu sfumato, font Chakra Petch,
  sfondi finti): grafica incoerente, pulsanti sovrapposti al titolo.
- **Accompagnare il giocatore**: dove una scelta conta, serve un pulsante di conferma esplicito, non un tocco
  qualsiasi che conferma per sbaglio.

## Comandi

- Solo touch. Nessun tasto PlayStation a schermo, né nei menu né nei filmati. (Le opzioni DW3_TOUCH_KEYS
  restano solo per i test.)
- **Levetta analogica mobile**: compare dove si appoggia il dito e solo dove il giocatore cammina; sparisce
  quando si alza il dito. Spinta leggera = cammina, spinta piena = corre.
- **Corsa al 58% di quella del gioco** (richiesta: "la metà e poco più"). Solo per il giocatore guidato dalla
  levetta; i filmati restano alla velocità originale.
- **Porte automatiche**: spingendo la levetta dentro un'uscita si entra/esce senza toccare nulla. Scale,
  salti e minigiochi restano col fumetto da toccare (da rivedere con l'utente).
- **Personaggi**: avvicinandosi compare il fumetto sopra la testa del giocatore (come per le porte); un tocco
  parla al personaggio più vicino, girandosi verso di lui.
- Un tocco breve dove si cammina = l'azione (X).
- Nessuna rotazione della telecamera (il gioco non ce l'ha).
- Filmati: tenere premuto 2 secondi per saltare.
- Gesto Indietro di Android = annulla (Triangolo).
- **Pulsante Indietro in basso a sinistra** (17/11/2026), per chi gioca con una mano; prima era in alto a
  sinistra e non si notava. Se c'è anche il pulsante dei dialoghi automatici, Indietro sta sopra.
- Menu del campo: pulsante in alto a destra.

## Dialoghi

- Un tocco va avanti, tenere premuto fa scorrere il testo.
- **Dialoghi automatici**: la freccia arancione dei fumetti del gioco (lo sprite vero), ruotata a destra, in
  basso a sinistra sopra la banda nera, senza sfondo; accanto "x1/x2/x3"; animata quando attiva; ogni tocco
  cambia velocità (spento → x1 → x2 → x3 → spento). La velocità cambia sia l'attesa sia la scrittura; x3 è più
  veloce che tenere premuto.

## Menu iniziali

- Lingua ricordata; si sceglie toccando la riga.
- Titolo: tocco ovunque per "Premi START"; opzioni toccabili.
- Nome: tastiera del telefono, mai sopra l'immagine; pulsante OK (il tasto "fine" del gioco) sotto il nome;
  Invio conferma; toccare il nome riapre la tastiera; niente levetta.
- **Scelta del kit**: le linguette A/B/C scelgono il kit, il pulsante **OK sotto le linguette** (lo sprite OK
  della schermata del nome, stesso pacchetto grafico) conferma. Toccare i Digimon non conferma più.
- Da discutere con l'utente (in sospeso): titolo a tutto schermo, pulsante Impostazioni per la lingua,
  pulsanti più grandi — sempre con la grafica del gioco.

## Salvataggi

- **Più partite**, ognuna con i suoi salvataggi. Nell'elenco si chiamano **"MEMORY CARD 1, 2, 3…"** e la
  domanda è "Scegli lo slot MEMORY CARD" (17/11/2026: l'utente preferisce la dicitura vintage a "Partita N").
- **Salvataggio automatico** = vero salvataggio di gioco (non uno stato della memoria), su due posizioni
  alternate (auto_a / auto_b), ogni 2 minuti mentre si cammina e quando l'app va in secondo piano. Un piccolo
  segno in alto al centro quando salva. Scartato: lo stato della memoria (dipende dalla build, si perde a ogni
  aggiornamento).
- Continua → elenco delle partite (grafica del gioco) → "Automatico" o "Salvataggi". Nessuna finestra Android
  "vuoi riprendere".

## Cronologia git

- In sospeso (chiesto all'utente il 17/11/2026): riscrivere la storia per togliere `build-linux-gles/`
  (eseguibile compilato) finito nel commit 05c507c. È già tolto dalla versione attuale e ignorato.
