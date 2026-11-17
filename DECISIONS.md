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
- Il fumetto dei personaggi ha l'animazione degli scarabocchi delle scene (richiesta del 17/11/2026).
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
  veloce che tenere premuto (1.0: scrittura x8, 0,2 s per pagina). La velocità si legge nei trattini della freccia
  stessa: x1 nessuno, x2 due, x3 tre (proposta A, approvata il 17/11/2026).

## Menu iniziali

- Lingua ricordata; si sceglie toccando la riga.
- Titolo: tocco ovunque per "Premi START"; opzioni toccabili.
- Nome: tastiera del telefono, mai sopra l'immagine; pulsante OK (il tasto "fine" del gioco) sotto il nome;
  Invio conferma; toccare il nome riapre la tastiera; niente levetta.
- **Scelta del kit**: le linguette A/B/C scelgono il kit, il pulsante **OK sotto le linguette** (lo sprite OK
  della schermata del nome, stesso pacchetto grafico) conferma. Toccare i Digimon non conferma più.
- **Domande sì/no della storia** (field/choice.c, es. "Vuoi creare il partner Digimon ora?"): pannello del
  gioco più alto di un terzo, le due risposte in due riquadri affiancati (lo stesso pannello in piccolo); un tocco
  risponde. In verticale in fondo alla tela.
- **Registrazione dell'account**: Sì e No in due riquadri affiancati, con la cornice della barra di
  registrazione del gioco; un tocco risponde.
- Da discutere con l'utente (in sospeso): titolo a tutto schermo, pulsante Impostazioni per la lingua,
  pulsanti più grandi — sempre con la grafica del gioco.

## Salvataggi

- **Più partite**, ognuna con i suoi salvataggi. Nell'elenco si chiamano **"MEMORY CARD 1, 2, 3…"** e la
  domanda è "Scegli lo slot MEMORY CARD" (17/11/2026: l'utente preferisce la dicitura vintage a "Partita N").
- **Salvataggio automatico** = vero salvataggio di gioco (non uno stato della memoria), su due posizioni
  alternate (auto_a / auto_b), ogni 2 minuti mentre si cammina e quando l'app va in secondo piano. Un piccolo
  segno in alto al centro quando salva. Scartato: lo stato della memoria (dipende dalla build, si perde a ogni
  aggiornamento).
- Elenco grande: due partite alla volta nel riquadro principale, righe alte il doppio, giorno e ora di
  creazione accanto (created.txt), trascinare per scorrere, tocco per scegliere, **tenere premuto per
  eliminare** (con Sì/No).
- Continua → elenco delle partite (grafica del gioco) → "Automatico" o "Salvataggi". Nessuna finestra Android
  "vuoi riprendere".

## App

- **Battaglia in verticale**: a tutto schermo, i pulsanti dei comandi grandi in basso **sopra** la scena 3D
  (richiesta del 17/11/2026). Demo e interfacce sempre con la grafica del gioco (l'utente ha scartato le demo
  disegnate a mano).

- L'immagine di copertina proposta dall'utente (17/11/2026) non si usa come icona: contiene personaggi e logo
  protetti; si resta su un'icona originale, eventualmente ridisegnata nei colori della copertina.
- Icona originale (uovo digitale su fondo blu notte, circuiti; nessun personaggio o logo del gioco),
  `tools/android/make_icon.py`. Sostituisce l'icona di SDL.
- Android: render_scale massimo 3 (la VRAM 1024×1024 a 4× pesa il doppio).

## Cronologia git

- 17/11/2026, approvato dall'utente: storia riscritta (filter-branch) senza `build-linux-gles/`; push forzato.

## Verticale

- **Dove la mappa finisce resta il nero** (17/11/2026): niente ingrandimento delle stanze piccole, dà l'idea del
  gioco d'epoca. (Sostituisce la vecchia idea "nelle stanze piccole si ingrandisce".)
- **Fascia in alto** (idea approvata dall'utente, 17/11/2026): pulsante Impostazioni, pulsante Menu e stato dei
  partner (nome, livello, HP, esperienza), con la grafica del gioco; la fascia in basso per dialoghi e comandi.
- Tela del campo 320 × (forma del telefono, max 720), da VRAM (0,512) in una VRAM del port 1024×2048; su
  Android render_scale 2 (l'utente accetta meno risoluzione pur di avere tutto schermo, 17/11/2026).
- Filmati sempre in 4:3. Banner del cambio di ambiente a tutto schermo.

- **Il verticale è la modalità principale** (ribadito il 17/11/2026, "fin dall'inizio"). Tutto schermo vero:
  campo, menu e battaglie riempiono il telefono. L'orizzontale è un di più.
