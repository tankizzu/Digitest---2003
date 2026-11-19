# Changelog

## Android (Digimon World 2003 per Android)

### 2.5 (in preparazione)

- **Menu col dito** (verticale): il menu del campo ha le voci grandi (righe di circa 29 pixel, il doppio di prima)
  e i partner in fila sopra, nel campo e nelle schermate dei menu; un tocco su una voce la apre. Oggetti: i tre
  partner grandi in alto (un tocco dà l'oggetto a quello), le categorie grandi (un tocco le apre), la lista degli
  oggetti in **una sola colonna di 16 righe grandi** (un tocco sceglie l'oggetto e mostra la sua descrizione, un
  secondo tocco lo usa; trascinare a destra o a sinistra cambia pagina, niente più L1 / R1). Verificato su Linux il
  menu, le categorie, la scelta del partner e l'uso di un oggetto; il tocco sulle righe della lista e il trascinamento
  solo sul telefono (su Linux non si simulano).
- **Ordina col dito**: i partner grandi, le frecce dello scambio accanto, il titolo sopra e l'aiuto sotto, tutto al
  centro dello schermo (richiesta dell'utente); si tocca il primo, poi quello con cui scambiarlo. Verificato su Linux.
- **Stato col dito**: si tocca il partner; poi il suo pannello in alto con EXP e TP, le scelte «Digievoluzioni» ed
  «Equipaggiamento» grandi (un tocco le apre), le statistiche e i pannelli delle digievoluzioni e dell'equipaggiamento
  più grandi. Verificato su Linux.
- **Equipaggiamento col dito** (da Stato): il titolo e le sei caselle grandi sotto le statistiche (un tocco apre la
  casella); poi l'oggetto della casella e la lista delle armi o protezioni grande (righe di 21 pixel): un tocco
  sceglie e mostra l'effetto sulle statistiche, un secondo tocco equipaggia, le frecce su e giù della lista cambiano
  pagina; l'aiuto in basso. Verificato su Linux (anche con 20 armi).
- **Digievoluzioni col dito** (da Stato): il titolo, le tre caselle (o la domanda su una casella) 1,8 volte, le
  statistiche grandi da un lato all'altro, l'aiuto in basso. Un tocco sul nome del partner o su una casella ne mostra
  le statistiche, un secondo tocco va avanti; le risposte alla domanda e le tecniche si toccano; nelle schermate che
  aspettano X basta un tocco. Verificato su Linux il percorso del partner (la sua tecnica); le caselle, la domanda e
  le tecniche di una digievoluzione **non verificati** (i salvataggi di prova non hanno digievoluzioni).
- **Tecniche col dito**: come Ordina, al centro dello schermo: il titolo, i partner grandi (un tocco apre le sue
  tecniche), sotto la lista delle tecniche 1,4 volte (un tocco sceglie, un secondo la usa; per una cura si tocca il
  partner da curare), l'aiuto; i messaggi si chiudono con un tocco. Verificato su Linux la scelta del partner e il
  messaggio «non ha imparato tecniche»; la lista **non verificata** (i partner di prova non hanno tecniche da campo).
- **Negozio, prima schermata**: COMPRA e VENDI il doppio più grandi, un tocco sceglie; tolta la scritta «Button: Back»
  (si torna indietro col pulsante dell'app). Vendere: i quattro tipi (Oggetti, Armi, Protezioni, Accessori) il doppio
  più grandi, un tocco sceglie; il messaggio «Non hai ...» grande sotto. Comprare: la lista degli oggetti del negozio in
  una colonna di 8 righe il doppio più grandi (un tocco sceglie e mostra i dettagli, un secondo compra), la pagina
  cambia toccando le frecce, senza le scritte L1 / R1. Poi la quantità 1,8 volte (toccando la metà alta delle frecce
  uno in più, la bassa uno in meno, il resto del riquadro va avanti) e il totale con Compra / No 1,5 volte; un tocco
  sul suggerimento dei dettagli gira la loro pagina; «Equipaggiare?» Sì / No e poi il partner si toccano (i dettagli
  restano in basso). Verificato su Linux un acquisto intero (3 spade, poi equipaggiate a Kotemon). Ancora
  originali: i dettagli (piccoli) e la scritta «Button: Effects». Vendere: la lista in una colonna di 14 righe 1,6 volte,
  poi quantità e totale grandi come per comprare; verificata su Linux la vendita di una spada.
- **Laboratorio, menu e scelta del partner**: le tre voci 1,75 volte e il titolo più grande, un tocco apre; tolta la
  scritta «Button: Back». Poi il partner (pannello in alto 1,5 volte) e le capsule della squadra 1,75 volte: un tocco
  mostra il partner, un secondo apre la schermata; il suggerimento delle voci si tocca. Nascosta «Button: Back to
  Menu». Lo scambio: titolo, capsule e partner da inserire 1,55 volte in alto, i due pannelli dei partner e la freccia
  1,3 volte sotto; toccando ai lati del partner da inserire si passa agli altri, toccandolo si scambia. Verificato su
  Linux l'aspetto (la squadra di prova non ha partner da scambiare); manca ancora lo sfondo scuro dietro le capsule.
  Digievolvi: partner, statistiche, caselle, titolo e le due scelte una sotto l'altra, più grandi; un tocco sceglie, i
  messaggi si chiudono con un tocco. Verificato su Linux. Tabella: le frecce in basso si toccano per cambiare pagina (tolte le scritte L1 / R1), un Digimon conosciuto si tocca
  per sceglierlo e di nuovo per vederne il nome (non verificato: i Digimon della squadra di prova sono sconosciuti),
  un tocco chiude il nome; la tabella resta della sua misura. La lista delle forme è ancora quella originale; «Button: Details» è ancora a schermo (si tocca).
- **Palestra**: il partner, le statistiche e i TP più grandi, le capsule della squadra 1,75 volte (un tocco mostra il
  partner, un secondo lo allena); poi la griglia degli allenamenti 1,7 volte (un tocco mostra l'allenamento, un
  secondo lo sceglie; la freccia della pagina si tocca) e la descrizione sotto. Nascoste le scritte «Button: Stop
  training» e L1 / R1. Poi la scelta dei TP 1,6 volte (un tocco sceglie, un secondo usa), la domanda e il messaggio
  si toccano. Verificato su Linux fino all'avvio dell'allenamento (con `DW3_TP`). L'allenamento stesso (i segni ○ ✕
  e le scritte restano al posto originale) e il risultato sono ancora quelli originali.
- Corretto: le icone dei tasti nei testi (es. il ● di «Button: Effects») restavano al loro posto quando il testo era
  spostato da una disposizione nuova; ora seguono il testo.
- **Salvataggi automatici affidabili** (non si riuscivano a caricare le partite: dopo «Carica» il gioco diceva che
  non c'era niente di salvato). Prima il salvataggio automatico scattava solo dopo due minuti *di fila* col
  personaggio libero, e ogni dialogo o scena azzerava il conto: nel prologo quasi mai. Ora il tempo conta comunque
  (scene comprese) e il salvataggio arriva al primo momento libero dopo due minuti di gioco; uscendo dall'app si salva
  l'ultimo momento in cui il personaggio era libero, anche nel mezzo di una scena. Verificato su Linux il salvataggio
  a tempo; quello all'uscita dall'app solo sul telefono.
- **Strada di Asuka a tutto schermo** (approvata): in verticale la scena del prologo si ingrandisce (≈ 1,85) fino a
  riempire lo schermo, i dialoghi seguono i personaggi; testi e pulsanti restano della loro grandezza. Verificato su
  Linux.

### 2.4

- Battaglia in verticale: il **lampo bianco** (entrata di un partner, alcuni colpi) copriva solo le prime 240 righe
  dello schermo; ora tutto. La **digievoluzione** (il vecchio Digimon che diventa reticolo e il nuovo che appare)
  scorre su tutta l'altezza, e i suoi effetti a tutto schermo stanno a metà schermo invece che in alto. (Lampo:
  verificato su Linux, entrata di Renamon. Digievoluzione: non provata, la squadra di prova non può digievolvere.)
- **Resoconto di fine battaglia a tutto schermo** (verticale): lo sfondo a griglia del gioco su tutta l'altezza, i
  pannelli dei partner e i messaggi al centro. Verificato su Linux (vittoria nel Settore Sud: EXP, livello,
  digievoluzione imparata, BIT).
- **Menu del campo a tutto schermo** (verticale): oggetti, ordina, mappa, tecniche, stato, salva sulla tela alta, lo
  sfondo del gioco su tutta l'altezza e il contenuto al centro. Verificato su Linux (tutte le schermate aperte, tocco su
  MAPPA).
- **Negozio degli oggetti a tutto schermo** (verticale): stesso schema. Verificato su Linux (Asuka Armory, compra).
  Restano le scritte coi tasti PlayStation ("Button: Back", "R1"), da rifare coi tocchi.
- **Laboratorio Digi-Lab a tutto schermo** (verticale): stesso schema. Verificato su Linux (menu, cambio Digimon).
- **Palestra (allenamento) a tutto schermo** (verticale): i tre layer sulla tela alta. Verificato su Linux (scelta del
  Digimon e dell'allenamento); la finestrella dell'allenamento non provata (la squadra di prova non ha TP).
- **Album e negozio delle carte a tutto schermo** (verticale): stesso schema. Verificato su Linux (aperti con
  `--field 4608` e `4864`). Anche l'editor del mazzo (`--field 1024`). La battaglia a carte non ancora.
- **Dissolvenze in verticale**: quella del titolo (dai bordi verso il centro) e quella comune a memory card,
  negozi, laboratorio e altri menu ora scuriscono tutto lo schermo, non solo le 240 righe centrali. Verificato su
  Linux (titolo → memory card); orizzontale identico.
- Prove: `DW3_ENEMY_HP=1` porta gli HP dei nemici a 1 a ogni turno (per arrivare al resoconto).
- Proposta spenta (da approvare): strada di Asuka del prologo a tutto schermo, solo la scena ingrandita
  (`DW3_ZOOM_MAP=1`).

### 2.3

- **Battaglia in verticale con la disposizione dell'utente** (dalle demo "Disposizione battaglia"): statistiche
  più in basso e più grandi, il volto del partner accanto; i messaggi sopra i comandi; Tecniche, Oggetti, DV e Tag
  più grandi in basso, ognuno dove l'utente l'ha messo (descrizione degli oggetti in alto, MP della tecnica sopra la
  lista). **Niente L1 / R1** nelle liste: un tocco sceglie una riga, **trascinare** a sinistra o in su va alla pagina
  dopo, a destra o in giù a quella prima (non provato: su Linux non si simula il trascinamento).
- Corretto: in verticale il **pallino bianco** della riga scelta (comandi e liste) non si vedeva mai (anche nella
  2.2): il gioco lo copia a vsync nel buffer già mostrato, e la tela alta si ridisegna tutta. Ora si disegna nel
  fotogramma, ingrandito come la riga (la stessa immagine del gioco con la sua animazione).
- Corretto: la cornice degli MP nelle Tecniche si portava dietro la linea che nel gioco la unisce al pannello del
  partner; ora è ritagliata al suo riquadro.

### 2.2

- **Battaglia in verticale** (prima versione da provare): la tela alta; la **scena 3D a tutta altezza** (il centro
  della ripresa a metà schermo: si vede molto più ambiente sopra e sotto); in alto le **statistiche affiancate**
  (nemico e partner, come nel gioco) e sotto il riquadro col volto del partner; in basso i **sei comandi come
  pulsanti grandi** su due colonne (Fight / Tech, DV / Tag, Item / Run: la riga del gioco ingrandita, le parole una
  volta e mezza più grandi), un tocco sceglie. Tecniche, oggetti e messaggi si aprono in basso con la loro grafica.
  Verificato su Linux: incontro nel Settore Sud, comandi, Tecniche, Oggetti; orizzontale invariato.
- Corretto: in verticale la battaglia scartava i pezzi 3D sopra il centro dello schermo (confronto col rettangolo
  del layer alla riga 512 della VRAM) e la barra del cursore finiva nella memoria del vecchio schermo.
- Prove: la "prova di battaglia" di debug del gioco (`--field 1536 --arg 1`) andava in crash: il gioco originale le
  dà 0 byte di spazio; ora ha il suo.

### 2.1

- **Fascia in alto nel campo (verticale)**: mentre il giocatore ha il controllo, in alto a sinistra i tre partner,
  ognuno nella barra dei menu del gioco con il nome (font del gioco) e la barra degli HP; in alto a destra **MENU**
  nella stessa barra (dalla vittoria su Genji, quando il gioco ha il menu del campo): un tocco apre il menu del
  gioco. Sparisce durante dialoghi, eventi e menu. Al posto del vecchio pulsante menu disegnato dal port.
  OPZIONI non c'è ancora: il gioco non ha una schermata opzioni nel campo e quella della lingua esce dalla partita.
  Verificato su Linux: pannelli (game5, prologo) e MENU → menu del campo (`--field 515 --progress 5`); orizzontale
  identico ai riferimenti.
- **Menu del campo senza tasti PlayStation**: i testi "Tasto △: chiudi lo stato", "Tasto △: torna al menu",
  "Tasto □: ordina oggetti" e simili (file STATUS, IT ed EN) diventano "Menu", "Oggetti", "Cambia formazione
  Digimon". Verificato su Linux (EN). Corretto anche: in francese, tedesco e spagnolo la domanda del nome della
  memory card e "OK" erano vuoti.

### 2.0

- **Indietro a tema col gioco**: nella schermata memory card (verticale) "CARICA" è tolto e nel riquadro di
  metallo appeso c'è la freccia arancione dei fumetti del gioco, girata a sinistra e lampeggiante: toccandola si
  torna indietro (anche un passo indietro nelle scelte). Altrove il pulsante Indietro è la stessa freccia (senza
  trattini) al posto del cerchio con "‹", appena il gioco l'ha letta. Verificato su Linux (aspetto).
- **Impostazioni con la grafica del gioco**: l'ingranaggio bianco disegnato dal port è sostituito dall'ingranaggio
  del gioco (l'icona "Macchina" del foglio dei menu, fotogramma 0x30) ingrandito, nel pannellino dei menu del gioco
  (0x16), in alto a destra dentro l'area sicura del telefono. Tre proposte (screenshot reali): A solo l'icona, **B
  icona nel pannello (scelta di partenza)**, C pannello sotto CONTINUA. Verificato su Linux: aspetto delle tre e
  tocco (apre la schermata della lingua); orizzontale identico ai riferimenti tranne il pannello nuovo sul titolo.
- **Nome della memory card al primo salvataggio**: la prima volta che si salva una partita, la schermata chiede "Dai
  un nome alla MEMORY CARD": si scrive con la tastiera del telefono e il nome appare col font del gioco nel riquadro
  di sinistra (toccandolo torna la tastiera), **OK** nel riquadro di destra (o Invio) conferma; vuoto resta
  "MEMORY CARD N". La freccia Indietro esce senza salvare. Il nome (lettere, cifre, spazi, max 16) sta in
  `saves/gameN/name.txt` e compare nell'elenco degli slot, sopra le scelte dello slot e nella riga in alto quando si
  salva. Verificato su Linux in verticale (scrittura simulata, OK, elenco); in orizzontale non provato.
- **Sì/No affiancati anche nelle schermate originali della memory card** (verticale): "Creare nuovo file?",
  "Preparare la partita?", "Sovrascrittura dati precedenti. Vuoi procedere?" e "Vuoi salvare qui?" / "Vuoi caricare
  questa partita?" hanno Sì e NO in due riquadri grandi del gioco sotto la cornice; un tocco risponde. Toccare una
  delle tre memory card in alto ora la sceglie soltanto (Sì salva/carica). Un tocco altrove non risponde più (prima
  confermava la scelta evidenziata). In orizzontale resta il riquadro piccolo del gioco, ma Sì e NO si toccano.
- **Niente tasti PlayStation nei testi**: tolte le righe "Tasto ✕: conferma / Tasto △: indietro" e il "Sì NO"
  scritto nel testo della formattazione (IT ed EN).
- Verificati su Linux in verticale: Creare nuovo file → Sì, Vuoi salvare qui → Sì, salvataggio riuscito, di nuovo
  Sì → Sovrascrittura → NO → ritorno. In orizzontale: nome e "Creare nuovo file?" (aspetto).
- **Liste con immagine e dati** (verticale): **Automatico** ora apre la schermata originale della scelta del
  salvataggio con i **due autosalvataggi** (il più recente per primo) e i dati del gioco (nome, zona e luogo,
  partner con livello, bit, tempo di gioco); Sì carica quello scelto. **Salvataggi** apre le tre memory card del
  gioco come prima. In tutte e due, sotto la cornice, l'**immagine del campo** com'era al salvataggio, nel riquadro
  del gioco. Le immagini (160×120) si prendono dal campo mentre si cammina (ogni ~4 secondi) e subito a ogni
  autosalvataggio, e si scrivono accanto ai salvataggi (`auto_a/b.thumb`, `save_0..2.thumb`). I salvataggi fatti
  prima di questa versione non hanno immagine. In orizzontale nessuna modifica.
- **Lingua a tutto schermo in verticale**: lo sfondo a riquadri del gioco su tutta l'altezza, pannelli e bandiere al
  centro, la dissolvenza in uscita su tutta la tela. Verificato su Linux (aspetto, tocco su ITALIANO, uscita).
- **Nome, kit e conferma a tutto schermo in verticale** (modalità del nome, STPLNMET): la tela alta, il contenuto al
  centro come nel gioco, e lo sfondo a esagoni del gioco ripetuto in altezza (le sue bande si ripetono ogni 44 / 38
  pixel: copie sopra e sotto, nessuna deformazione).
- Strada di Asuka (inizio del prologo): verificato che non è in 4:3; la sua mappa è alta solo 384 pixel (3 riquadri)
  e in verticale si vede tutta, col nero dove finisce. Tre possibilità da scegliere (nessuna modifica fatta).
- Salvando in verticale, "Nessun salvataggio in questo slot" e gli altri testi del gioco tornano dentro la cornice
  (prima finivano sotto, al posto della colonna degli slot).
- Test: `DW3_SHEET_DUMP=cartella` salva ogni fotogramma dei fogli di sprite del titolo e dei menu (per scegliere la
  grafica del gioco per i pulsanti); `DW3_SETTINGS_LOOK=1/2/3` mostra le tre proposte; `DW3_PRESS` "save" apre la schermata di salvataggio
  dal campo (anche nel prologo, senza menu).

### 1.9

- **Tutto schermo vero sul telefono**: barre di sistema nascoste (modalità immersiva di SDL; uno scorrimento dal
  bordo le mostra un momento), immagine anche sotto il notch. **Pulsanti nell'area sicura** del telefono (notch,
  angoli arrotondati, barra dei gesti): `SDL_GetWindowSafeArea`. Non verificato sul telefono.
- **Memory card in verticale riordinata**: **5 slot** (massimo) in una colonna, a destra del tubo, ognuno nel
  riquadro del gioco (sprite 29) con l'icona della memory card, nome e data; la finestra del gioco alla sua
  grandezza (niente linee stirate); "CARICA" nel riquadro di metallo appeso; scelte sempre a **due pulsanti
  affiancati**: Carica / Elimina, poi Automatico / Salvataggi (se c'è un autosalvataggio), Sì / No.
- **Slot pieni**: INIZIA chiede quale slot sostituire ("Scegli lo slot da sostituire", "Sostituire MEMORY CARD N?").
- Verificati su Linux: aspetto, Carica/Elimina, Elimina → Sì/No, INIZIA con 5 slot pieni → scelta e conferma.

### 1.8

- **Memory card in verticale con 10 slot**: due colonne da cinque, ogni slot con l'icona della memory card del
  gioco (sprite 0x23 del foglio GMCARD, a metà grandezza, lampeggia sullo slot scelto), nome, data e ora o
  "vuota"; cornice del gioco 1,9 volte più alta; "CARICAMENTO" tolto, "CARICA" nel riquadro di metallo sopra.
  Toccando uno slot: Automatico (se c'è) / Salvataggi / **Elimina** (con Sì/No, "No" già scelto).
  Verificato su Linux: aspetto, scelta dello slot, Elimina → conferma.
- **Massimo 10 memory card**: una partita nuova prende il primo slot libero; con tutti e 10 occupati riusa quello
  creato da più tempo.

### 1.7

- **Titolo in verticale**: INIZIA/CONTINUA e "Premi START" 1,6 volte più grandi e più in basso, copyright in fondo
  con un margine, **pulsante impostazioni** (ingranaggio) in alto a destra che apre la schermata della lingua del
  gioco anche con una lingua salvata. Verificato su Linux: aspetto e tocco su CONTINUA; il tocco
  sull'ingranaggio non è simulabile su Linux.
- **Memory card a tutto schermo**: sfondo a motivo e tubo su tutta l'altezza, finestra al centro, tocchi e lista
  riallineati. Verificato su Linux (aspetto).
- Scena comune dei menu (`shared/update_scene.inc.c`) verticale per chi definisce SCENE_UPRIGHT.

### 1.6

- **Filmati di nuovo in 4:3** (nella 1.3–1.5 erano stirati in verticale: la regola "pixel quadrati" valeva per
  ogni area alta; ora solo per la tela del campo).
- **Titolo a tutto schermo in verticale**: sfondo a circuiti ingrandito a tutta altezza (lati tagliati), logo e
  scritte al centro; tocchi riallineati (`native_touch_origin`). Verificato su Linux.
- GPU del port: sulle tele verticali (sotto la riga 512) i poligoni possono essere alti fino a 1023.

### 1.5

- **Icona adattiva piena** (sfondo viola-blu e uovo in primo piano): riempie la forma del launcher, niente più
  contorno bianco.
- **Personaggio sempre al centro** in verticale: la telecamera non si ferma più ai bordi della mappa, oltre si
  vede il nero. Verificato su Linux.

### 1.4

- **Freccia dei dialoghi automatici**: la velocità si legge nei tre trattini della freccia stessa (x1 nessuno, x2
  due, x3 tre), al posto delle frecce piccole. Verificato su Linux a x2 e x3.
- **Icona**: disegno originale in pixel art (uovo digitale su fondo viola-blu con cifre binarie, nessun
  personaggio o titolo).

### 1.3

- **Campo a tutto schermo in verticale**: la tela prende la forma del telefono (320 × fino a 720; su un 9:20,
  320×711). VRAM del port 1024×2048; su Android render_scale 2 (risoluzione del 3D più bassa, come concordato).
  Verificato su Linux con DW3_PORTRAIT_H=710.
- **Schermata del cambio di ambiente** (banner dell'area) su tutta la tela verticale. Verificato su Linux.

### 1.2

- **"Vuoi scaricare il Partner Digimon ora?" si risponde col tocco**: era una copia del menu sì/no dentro la
  stanza (WSTAG210, evento 9), senza tocco. Ora usa i due riquadri grandi, come le altre copie in
  WSTAG220/270/280 (funzioni comuni in field_map.h). Verificato su Linux (DW3_TEST_EVENT=9): "Sì, grazie!" porta
  al Servizio domatori, anche in verticale.
- Lista dei consigli del Servizio domatori toccabile.
- **Anteprima del verticale sul telefono**: l'app parte in verticale e il campo usa la tela 320×512; menu,
  battaglie e filmati restano nel riquadro 4:3. Non verificato sul telefono.

### 1.1

- **Elenco MEMORY CARD**: scorreva in un senso solo (all'inizio e alla fine dell'elenco il trascinamento veniva
  azzerato a ogni fotogramma). Ora si muove in tutti e due i sensi. Non verificato col dito.
- **Dialoghi automatici con due fumetti aperti**: si fermavano (il conteggio dell'attesa ripartiva a ogni
  fotogramma alternandosi tra le due finestre). Ora ogni finestra ha il suo conteggio.
- **Domande sì/no della storia** in due riquadri grandi affiancati, nel pannello del gioco più alto. Verificato su
  Linux (DW3_TEST_CHOICE): aspetto e risposta col tocco.
- **Icona dell'app** originale al posto di quella di SDL.
- VRAM del port 1024×1024 (base del verticale); su Android render_scale massimo 3.
- Prototipo del campo in verticale (spento: DW3_PORTRAIT=1 su Linux).

### 1.0

- **Elenco MEMORY CARD più grande**: le partite stanno nel riquadro grande dello schermo, due alla volta, ogni
  riga alta il doppio, con giorno e ora di creazione accanto ("MEMORY CARD 8   17.11.2026  07:46"). Trascinando
  si scorre, un tocco sceglie, **tenendo premuto su una riga si elimina** (domanda Sì/No, "No" già scelto).
  "Automatico / Salvataggi" nello stesso formato. Verificati su Linux: aspetto, scelta, caricamento.
  Eliminazione e trascinamento non verificati (servono dita vere).
- **Sì / No della registrazione in due riquadri affiancati**, con la cornice della barra di registrazione del
  gioco; un tocco risponde. Verificato su Linux.
- **Fumetto dei personaggi con gli scarabocchi** delle scene (riga 1 delle animazioni dei fumetti). Verificato
  su Linux.
- **Dialoghi x3 più veloci**: scrittura 8 volte più veloce, 0,2 s di attesa per pagina.
- **Velocità dei dialoghi automatici**: al posto di "x1/x2/x3" una, due o tre frecce piccole (lo sprite del
  gioco) accanto alla freccia grande.
- Storia di GitHub riscritta senza `build-linux-gles/`.
- Dialoghi automatici in sequenza: su Linux (x2, nessun tocco) il prologo va avanti da solo fino alla scelta del
  kit dall'impiegato; il blocco segnalato non si riproduce, da capire dove succede.

### 0.9

- **Fumetto vicino ai personaggi corretto**: nella 0.8 veniva creato ma non compariva mai (un task del gioco
  gira solo se è figlio di un altro). Ora è figlio del giocatore. Verificato su Linux: compare vicino alla
  ragazza della Sala della Matrice e il tocco fa partire il dialogo.
- Tutti i personaggi sono considerati, non solo una parte.
- PROJECT_STATE.md.

### 0.8

- **Crash alla scelta del kit risolto** (choice.c): toccando le linguette B/C il nuovo kit leggeva un
  fotogramma d'animazione inesistente. Verificato su Linux.
- **Pulsante OK per confermare il kit** sotto le linguette A/B/C (sprite OK del gioco); toccare i Digimon non
  conferma più. Verificato su Linux.
- **Indietro in basso a sinistra**, più grande (touch.c). Non verificato sul telefono.
- **"MEMORY CARD N"** al posto di "Partita N"; "Scegli lo slot MEMORY CARD" (savetexts.c). Verificato su Linux.
- **Porte automatiche** con la levetta (trigger.c). Verificato su Linux (Tempio della giungla → Giungla
  eterea).
- **Corsa al 58%** con la levetta, camminata con la spinta leggera (actor_control.c).
- **Fumetto vicino ai personaggi**, tocco per parlare girandosi verso di loro (actor_control.c). Non
  compariva: corretto nella 0.9.
- Test: `DW3_STICK` simula la levetta su Linux.
- Documenti di progetto: CLAUDE.md, PROJECT_STATE.md, DECISIONS.md, TODO.md, TECHNICAL_NOTES.md.
- `build-linux-gles/` tolto dal repository e ignorato.

### 0.7

- Freccia arancione vera dei fumetti per i dialoghi automatici, con x1/x2/x3; velocità di scrittura diverse.
- Slot della memory card toccabili.
- Salvataggio automatico come vero salvataggio di gioco, su due posizioni alternate; nessun tasto PlayStation.

## Upstream (dw3-native-port)

## Unreleased

- **Japanese** on the language screen, as a sixth row (NIHONGO, with Japan's flag). The European disc has every text in Japanese too, and the game the option for it, which its screen never shows; the rows close up to make room, and the label is made of the other labels' letters. Saves stay the European ones and the buttons are the same as in the other languages (✕ confirms). The port's own texts (DIGILOG, the starter packs, the Quit menu) have a Japanese version in kana, like the game's.

## v0.21.0 (beta)

- **Skip scenes** (Options, Input, and the launcher; `skip_scenes`, off by default; #15). Square held about two thirds of a second in a cutscene that takes the controls runs the rest of its script at once: no dialogues or waits, walks and the camera at their ends, and the scene's poses, commands, story flags and map changes as it has them. One hold goes on through the scenes, map changes and movies that follow, until the player has the controls back, the name screen opens or a battle starts. While it runs the screen stays black and silent and the game runs at x8; the picture fades back in once, where the player takes over. A cue at the bottom left (the skip button's icon in a ring that fills while it is held, and the word in the game's language) shows whenever a skip is possible, in a panel and font of the field menu's. A field's area banner ("Main Lobby") waits for the skip to end and shows as the picture comes back. The welcome text that opens the name entry screen ("Welcome to Digimon Online...") is skipped the same way. Suggested by Hucaru.
- **Fast forward's corner** in a panel and font of the field menu's, with two arrows before the speed, instead of the debug font on black.
- **Name entry's arrow keys** two pixels wider, with OK four pixels to the left to make room, so the arrows sit in their boxes.
- **Disc reads without a thread**: when the reader thread can't start, every read stays synchronous instead of hanging the next one.
- **Direct3D 12 start-up** releases what it made when a step fails, and the objects it no longer needs once it is up.
- **Launcher's DLL rows** (FSR 3.1, DLSS, DirectStorage) count a file as installed only while its SHA-256 matches; a wrong one is offered for download again.

## v0.20.0 (beta)

- **Sharpen, Brightness, Gamma, Saturation** (Options, Graphics; `sharpen` 0 to 10, `brightness`, `gamma`, `saturation` -10 to 10; all 0 by default, the game's own picture). A post-process pass over the game's picture on its way to the window, not over the movies or the options menu. The sharpening is contrast-adaptive, after AMD's FidelityFX CAS.
- **Scanlines** (Options, Graphics; `scanlines` 0 to 10, off by default). Dark lines between the game's rows, in the same pass.
- **Integer scaling** (Options, Display; `integer_scale`, off by default). The picture is a whole multiple of the game's 240 rows, with bars around it, so that every pixel of the game's is the same size on screen.
- **SMAA anti-aliasing** (Options, Graphics; `smaa`, off by default). The picture's jagged edges are smoothed and the rest is left sharp. Jimenez, Echevarria, Masia, Navarro and Gutierrez's SMAA 1x, as they publish it.
- **FSR 1 upscaling** (Options, Graphics; `fsr`, off by default). With the Resolution below the window's size, the frames with 3D in them (the battles) are enlarged to the window by the upscaling pass of AMD's FidelityFX Super Resolution 1.0 instead of plain smoothing; Sharpen does the rest. For slow graphics cards: at a Resolution that matches the window it has nothing to do.
- **Preset** (Options, Graphics, first row). Original, Enhanced or Remaster set the tab's options at once; it reads Custom when they are at none of the three.
- **Reads from the disc on their own thread.** The game asks for a file and goes on drawing while a thread reads it, as the console's drive did, instead of waiting for the disk. On a hard disk, the first battle no longer stutters when its files load.
- **No console window.** The game is built as a window's program: started from the launcher or Explorer, no black console window opens with it.
- **log.txt** (launcher, Tools): the game started from the launcher now writes what it reports there. The launcher pointed to it after a failed start, but nothing wrote the file.

**x64 build** (new: `dw2003-native-x64.zip`, for 64-bit Windows; the 32-bit `dw2003-native.zip` stays, OpenGL only). What only it has:

- **FSR 3.1** (Options, Graphics, Upscaling: Off, FSR 1 or FSR 3.1; `fsr = 2`; the Direct3D 12 renderer only). AMD's temporal upscaler over the frames with 3D in them, fed with the game's own depth, motion and jitter. It is loaded at run time from AMD's two FidelityFX DLLs, which the player puts next to the exe; without them it stays off.
- **FSR 3.1 files** (launcher, Settings, Graphics): downloads AMD's two DLLs from the FidelityFX SDK's tag v2.4.0 next to the game, each checked by its SHA-256. The port itself carries none of AMD's binaries.
- **DLSS** (Options, Graphics, Upscaling: Off, FSR 1, FSR 3.1 or DLSS; `fsr = 3`; the Direct3D 12 renderer, an NVIDIA RTX card). NVIDIA's temporal upscaler through Streamline, over the frames with 3D in them, fed with the same depth, motion and jitter as FSR 3.1. The mode is a row of its own (Upscaler mode, `upscale_mode`: DLAA, Quality, Balanced or Performance, Quality by default) and, for DLSS and FSR 3.1, it sets the render size instead of Resolution: 100, 67, 58 or 50 % of the picture's height, rounded to whole 240 rows of the game's (at 1440p: 6x, 4x, 3x, 3x). DLSS gets the picture in the game's 4:3 with square pixels, stretched over the window when shown, and is told the mode nearest to the real render ratio, since it refuses sizes outside a mode's range. It is loaded at run time from NVIDIA's four Streamline files next to the exe; without them, or on another card, it stays off. Needs a restart to take effect.
- **DLSS files** (launcher, Tools): downloads NVIDIA's Streamline SDK release v2.14.16 from GitHub and keeps the four DLLs next to the game, each checked by its SHA-256. The port itself carries none of NVIDIA's binaries.
- **Renderer** (x64 build; Options, Display, and the launcher; `renderer`: Auto, Direct3D 12 or OpenGL, Auto by default). The 64-bit exe carries both renderers and picks one at start: Auto takes Direct3D 12 where the graphics card has it, else OpenGL, and OpenGL takes over on its own if Direct3D 12 fails to start. Direct3D 12's DLLs are loaded at run time, so the exe starts on a system without them. Needs a restart. The 32-bit build stays OpenGL only, and the launcher shows neither this row nor Direct3D 12's extras (FSR 3.1, DLSS, DirectStorage) next to it.
- **DirectStorage reads** (Windows, x64 build). With Microsoft's `dstorage.dll` and `dstoragecore.dll` next to the exe, the reader thread asks DirectStorage for the files instead of reading them itself; without them, or if it fails, the thread's plain reads stay. Measured on a hard disk, a SATA SSD and an NVMe, the game's small reads take the same either way today; it is there for the larger files mods will bring. The port carries none of Microsoft's binaries, and DirectStorage's usage reports to Microsoft are switched off.
- **DirectStorage files** (launcher, Tools): downloads Microsoft's two DLLs from the DirectStorage 1.3.0 NuGet package next to the game, each checked by its SHA-256.
- **The boot image while Direct3D 12 starts** (x64). DLSS takes a few seconds to start (NVIDIA's Streamline); meanwhile the window shows the game's own boot image, with a line saying what is starting, instead of nothing. Upscaler mode is listed only while FSR 3.1 or DLSS is picked.

## v0.19.7 (beta)

- The Digilog no longer gives away the Qing Long Chief while the Digi-Egg of Sincerity is still being looked for: the step is titled after the egg, and once the swamp's wall is down it only says to go through it.
- The Digilog's titles that are wider than their box (the list's rows, the entry's title) are cut at the box's edge and scroll left, then start over (the chosen row's, and the entry's).

## v0.19.6 (beta)

- **Tech visual upgrade** (Options, Mods; on by default; `technique_visuals`). The players' magic techniques that use the scripts 9 and 13 take their projectile, launch sprite and sound, impact and stage from `technique_visuals.ini`, next to the exe, instead of the generic light ball. Off plays the game's own, and it switches in game for the next technique. Also in the launcher's Mods tab. The scripts are prepared at start from your own disc into `data/generated/`; your `data/mods` files win over them. Thanks to Zaw Htet Naing (#21).

## v0.19.5 (beta)

- **Crest fix** (Mods, off by default; `fix_equip_stacking`). The game switches off Search Crest and Counter Crest when the partner wears a weapon that poisons, paralyses, confuses, knocks out, drains or hits critically. With the option they work beside such a weapon, except Search beside a critical weapon, which sets the same bonus. Triple Crest is unchanged: a triple hit skips the weapon's effect.
- The options menu (Esc / Select) scrolls a tab with more rows than fit, with a white thumb like the game's own scroll bars.
- The options menu looks like the game's own menus: its navy panel and frame, the chosen row's bar and the game's font, also from the language selector, where the game has not loaded it yet.

## v0.19.4 (beta)

- The Digilog's DDNA quests send you to the DRI agent first (Johann, Jimmy, Kazuki, Michael, Ken, Yuji, Yang, Bob) with the party level total the agent asks for, and to the Digimon once the agent has made the request. They show up when the agent's field has been visited.
- **Form name in battle** (Graphics, off by default; `battle_form_name`). The battle HUD names the partner's current form, which follows a digivolution, instead of its base form. It changes when the digivolution swaps the model, not when the form is chosen. A name wider than the name box is shrunk to fit.
- The item shop's confirmation shows "Comprar" and "Vender" in Spanish; the disc has "Buy" and "Sell" in English there.
- The Digilog follows Sepikmon's mask after it is found: who knows of it in Asuka City, who holds it in the Asuka Sewers, then back to Sepikmon for the Smelly Herb.

## v0.19.3 (beta)

- The Digilog follows more of the story's steps within a step: the Seiryu Leader (Tom's word, the Protocol Ruins, MasterTyranno), the Blue Card (the Guilmons, the fake, the Trickster), Nami's word on the Symbol of Sincerity, the Agumon Suits (Master John, the couple, the man in the sewers, Kail), the search for Teddy in the Admin Center and the Staff Pass (Keith and Nick, then the hideout). The TNT Ball's step points at Tyranno Valley.
- The Digilog shows the Crony ID's quest as soon as it can be done and only while it can (one step of the story), so that it isn't missed; before, it showed once it was too late.

## v0.19.2 (beta)

- The Digilog follows the steps within a quest: its description, its place's picture and the map's objective show what it asks for now, by the game's own flags. For now the Tree Boots (Hide's card, Debbie, Veemon's hide and seek), Sepikmon's mask and the bomb for Bios Swamp's wall. The Tree Boots show up once Plug Cape has been visited.

## v0.19.1 (beta)

- Fast travel takes you there at once: Yes on the map screen fades to the area's field, instead of waiting for the menu to be closed.
- Fast travel only works from where you could walk out to another area at that moment: not from a room only an event lets you leave, nor from a place the story has shut. There, X on the map sounds the cancel.
- The Digilog's keys at the foot no longer run into each other when the map's key shows: the tabs' key reads LIST.

## v0.19.0 (beta)

- The Digilog's pictures of the fields are JPEG: about 8 MB in `data/postcards` instead of 29. The game makes them again by itself the next time it starts and removes the PNGs.

## v0.18.11 (beta)

- The Digilog's pictures of the fields make themselves: the first time the game starts it draws each field (its map and its objects) from your own game files into `data/postcards`, apart from the game, in a few seconds; the journal shows each one as soon as it is there. Nothing to run by hand, and none ship. Delete the folder to have them made again.

## v0.18.10 (beta)

- The Digilog has a layout of its own, in the menus' colours: a title and the tab at the top, the list on the left with its count and page, the chosen entry on the right (its field's picture with the place's name on it, the title and its kind's badge, how it stands, and a side quest's reward), its description below and the keys at the foot.
- The Digilog's pictures of the fields come from your own disc: `python tools/postcards.py <dw3fe.exe>` draws each field with the field editor into `build/data/postcards` (a build from source; they are the game's art, so the release has none). Without them the frame shows a cross.
- The Digilog marks a side quest that needs an item first: while you lack it, its status shows NEEDS and the item's name. Tree Boots (Veemon) asks for the Folder Bag.
- The D-pad's arrows in the game's own texts follow the icons chosen, as the face buttons do: the keys bound, or the pad's D-pad.
- The shops' lists name the key or button that turns the page (Q / E, LB / RB, L / R...) instead of always L1 and R1.
- Cancel on B shows On when either the swapped keys or the swapped gamepad buttons are in place, so a custom binding on the other half no longer leaves it showing Off.

## v0.18.9 (beta)

- The launcher moves with the arrows where it makes sense. On the home screen, Left and Right walk the bar (Play, Settings, Saves, Tools, language) and Up and Down move between it and the buttons under it, where Left and Right swap Play and Quit. In Settings, Up from the first row reaches the tabs, and Left and Right switch them.
- **Menu cursor wraps** (Options, Input; off by default). Up from the first row goes to the last, and down from the last to the first, in the field menu, the item lists and shops, and the techniques list (who uses it, which, and whom it heals). The other menus keep stopping at their ends. Also in the launcher's Input tab. Thanks to Hucaru (#14).
- **Cancel on B**: a row in Controls (options menu, Input tab; and the launcher's Input tab) that swaps Triangle and Circle, keys and gamepad buttons, so the game's cancel is on B. It is a remap: the icons in the game's texts follow it, and pressing it again undoes it. The row, before the list of buttons, shows On or Off. Thanks to Hucaru (#14).
- The launcher's settings list scrolls with the wheel by itself (three rows a notch) and the selection goes to the row under the pointer, instead of the selection jumping row by row and fighting the pointer; its scroll bar is wider, leaves the values clear and can be dragged with the mouse, its thumb lighting under the pointer and when held.
- **Menus in fast forward** (Options, Input; off by default). With fast forward on, a menu's cursor repeats at its normal pace when a direction is held, instead of at the game's speed. Also in the launcher's Input tab. Thanks to Hucaru (#19).
- **Skip PRESS START** (Options, Input; off by default). The title screen shows New game and Continue as soon as its intro ends, without waiting for Start. Also in the launcher's Input tab. Thanks to Hucaru (#17).
- **Heal on level up** (Options, Mods; off by default). A partner that levels up in a battle gets its HP and MP back in full; its Digimon's levels do not heal. Also in the launcher's Mods tab. Thanks to Hucaru (#18).
- **Inactive window** (Options, Display; off by default). While the game's window is not in front, nothing happens as before, the sound is muted, or the game is paused until the window is back. Also in the launcher's Display tab. Thanks to Hucaru (#16).

## v0.18.8 (beta)

- **Script mods** (`data/mods/scripts/<fighter>/<script>.json`): a fighter's battle script (its attack, hit, KO and evade scripts) as a list of commands that replaces the game's, e.g. `{"schema_version": 1, "commands": [{"op": "sound", "sound": "punch"}, {"op": "end"}]}`. A sound command can play a sound of the library, one of the game's (1 to 89), the hit's sound, or the game's own on a result 3 hit. A file that is wrong is rejected with the reason and the game's script runs; `--check-mods` lists them. Options > Mods > Custom scripts switches them off in game (also `native.ini` and the launcher's Mods tab). The model viewer (v0.18.5) edits the scripts in its Fighter tab and saves these files.

## v0.18.7 (beta)

- **A sound library** (`data/mods/sounds/sounds.json`): WAVs of that folder, each with a name and a volume, and a `replace` table that says which of the game's battle sounds (1 to 89) each one replaces, e.g. `{"sounds": {"punch": {"file": "punch.wav", "volume": 1}}, "replace": {"57": "punch"}}`. A replaced sound plays the WAV wherever the game plays it (hits, steps, falls, effects' casts), to its end. Options > Mods > Custom sounds switches it off in game, and `--render-sounds` never replaces, so the game's own stay available. WAVs load when they first play, or a frame at a time at start-up for the ones that replace a sound. The model viewer (v0.18.4) has a Sounds tab to import, name and assign them.

## v0.18.6 (beta)

- **An effect's sound replaces the game's hit sound** (v0.18.5 mixed it with the game's). With Custom sounds on and a sound in the effect's folder, only that sound plays; with it off, or no sound in the folder, the game's own plays. A hit that is a result 3 always keeps the game's. WAVs load the first time they are needed, not at start-up, so nothing stalls a hit.
- `dw2003.exe --render-sounds DIR [list]` plays the game's battle sounds with its own sound code and writes each as `DIR/<index>.wav`, with no window or audio device. The launcher does it for all of them into `data/sounds` after reading the disc, and once at start-up on an install that doesn't have them (or whose `data/sounds/version.txt` is older), in the background. A render that fails is not retried at every start. The model viewer (v0.18.3) uses them for its Original sound buttons.

## v0.18.5 (beta)

- **Custom effects and Custom sounds** (Options, Mods; on by default). Custom effects plays the battle effects' own images (`data/mods/effects/`) instead of the game's; off plays the game's own. An `effect.json` can also give the folder a sound (`{"sound": {"file": "hit.wav", "volume": 1}}`: a WAV of up to 10 s, any rate or channels, volume 0 to 4) that plays when the effect starts, mixed with the game's own; a folder with only a sound needs no PNGs. Both switch in game and show at the next effect. They are also in `native.ini` and in the launcher's Mods tab.
- The launcher's Tools tab shows an **Update** button, with the new version, to the right of Open on the model viewer's and the field editor's rows when a newer release of that tool is out, instead of one row for both. Left and right move between the two buttons; the mouse picks by where it clicks.

## v0.18.4 (beta)

- **Save anywhere** (Options, Mods; off by default). The field menu gets a SAVE row that opens the game's memory card screen from wherever you stand; loading that save puts you back at the same spot. The slot shows the area and the place's name. Cancelling returns to the field. A save made this way can show a wrong place name in the original game or an emulator. Thanks to Hucaru (#2).
- **Quit** in the field menu asks "Do you wish to quit the game?" with TITLE MENU, DESKTOP and CANCEL (shown in the game's five languages), and the options menu (Display tab) has a Quit game row. Thanks to Hucaru (#1).
- The launcher's Mods tab has the Full EXP Share and Save anywhere rows.
- Fixed: the launcher's "Your team" showed the slot with the most play time instead of the last one saved.

## v0.18.3 (beta)

- **Full EXP Share** (Options, Mods; off by default). Every party partner still standing at the end of a battle gets the full EXP, even if it did not fight, and its selected or used digivolutions the full Digimon EXP; knocked-out partners get neither. Thanks to Zaw Htet Naing (#13, #12).
- For mod authors: `dw2003.exe --check-mods` lists the Digimon mods in `data/mods/digimon` (loaded, disabled, rejected or skipped, with the reason and any warning) and exits with the number rejected. It prints to standard output, so redirect it to a file (`dw2003.exe --check-mods > mods.txt`).
- If the game cannot allocate memory at start-up it now says so in a message box and exits, instead of crashing.
- For builders: `build.py` stops when `dw3_decomp` is not at the commit the port was made against, or has uncommitted changes (`--allow-decomp-mismatch` overrides it); it also fails on a new unimplemented PsyQ function (`tools/stubs.baseline`), records what built it in `build/report.md`, and the repository's tests run on GitHub Actions (`tools/run_tests.py`).

## v0.18.2 (beta)

- **A Digimon mod's `like` is checked.** It must be a partner of the game's fighters' file (259, ExVeemon's, by default); a mod that points at one that isn't there is rejected and the log says so, where before it loaded and its record was silently left out.
- The effects' and the stage props' JSON files (`effect.json`, `stages.json`, `props.json`) are read by the same strict reader as the manifests: a name twice, a malformed number, text after the value or a file over 4 MB is now reported in the log, with its line, instead of being let by.
- Fixed: looking up a fighter in the fighters' file could read outside it when the file's list started at a word near 4 GB.

## v0.18.1 (beta)

- Fixed: the launcher's updates (the port's and the tools') never unpacked: the path of Windows' tar had a tab in it, so tar never started. Versions before this one still can't update from the launcher; unpack this release by hand once and they can.

## v0.18.0 (beta)

- **Digimon mods are checked.** Each `digimon.json` is read by a stricter JSON reader (`platform/json.c`: limits on size and depth, every
  allocation checked, real escapes and UTF-8, JSON's number grammar, a name twice in an object refused, `true`/`false`, nothing after the value) and validated: a wrong type, a number out of its
  field's range, fewer than six techniques, a model that isn't there, isn't a model file or has fewer entries than `subs` asks for, `pair.scripts` past its scripts, a name or help that isn't plain ASCII, a missing English help, or a `fid` that is a file of the
  game's disc rejects the mod, and the log says why. Folders are read in the order of their names, and mods that clash on id, fusion pair or
  effect id are all left out. `schema_version`, `author` and `version` are accepted; `enabled` takes `true`/`false`.
- Fixed: a manifest with fewer than six techniques read outside the techniques table; a `template` out of range and a Digimon's HP/MP above 255 or at 0
  are now refused or held in the byte they live in.
- Fixed: the stage mods' files were read without checking the file's size or a failed read.
- New tests: `tools/test_json.c`, `tools/test_mods.c`; `test_effect_stages` now runs on the real JSON reader.

## v0.17.8 (beta)

- **The launcher updates the port.** When a newer version is out, the "New version" chip downloads it, installs it over the old files and
  restarts the launcher; settings, saves and data stay. It asks to close the game first and never stops it.

## v0.17.7 (beta)

- **Digimon mods are folders now.** `data/mods/digimon/<name>/digimon.json` adds a Digimon partners can learn (stats, techniques with
  their texts, what it is learnt with, an optional fusion), with no change to the port; the README lists the fields. Flamedramon is the first:
  `dw2-extractor/flame_dcb.py` writes its manifest beside its model. BladeGarurumon stays built in, made from the game's own files.
- Changed: to turn a Digimon mod off, set `"enabled": 0` in its manifest (or move its folder); `flamedramon = 0` in `native.ini` no longer
  does anything. Flamedramon has no switch in the options menu or the launcher.
- Fixed: the DNA fusion into Flamedramon left the end of its attack's sound looping until the next map or fight; it now stops with the attack.

## v0.17.6 (beta)

- Changed: the Mods tab no longer shows the Flamedramon row. Its model can't be shipped, so players found a switch they couldn't turn on.
  If you make `data/mods/7120.bin` yourself the game still offers it; `flamedramon = 0` in `native.ini` turns it off.

## v0.17.5 (beta)

- Fixed: Check for updates could say it couldn't reach GitHub when GitHub's API was refusing the request. The launcher now reads
  the public releases feed instead, so it no longer uses the API at all.

## v0.17.4 (beta)

- Fixed: downloading or updating the model viewer or the field editor could fail at once at 0%, depending on the connection.
  The launcher no longer needs GitHub's API for them, which limits how often an address can ask.
- If a request still fails, `launcher.log` now says why (its HTTP status or WinHTTP error).

## v0.17.3 (beta)

- **Launcher: tool updates.** It now also checks for new versions of the model viewer and the field editor, at start and with
  Check for updates. If one is out, Tools shows an **Update tool** row; your settings and the tool's data stay. Close the tool
  first. Tools the launcher downloaded before this version are offered the latest release once, as it can't tell their version.

## v0.17.2 (beta)

- Fixed: 3D lines, like the wireframes of a digivolution, were wide and blocky at higher render scales. They are now thin and
  follow the model smoothly; 2D lines in menus stay as they were.

## v0.17.1 (beta)

- Fixed three crashes, reported by players and fixed by j0an2:
  - walking to an exit before the partners are called (a balloon drawn without its actor);
  - opening the Weapons list of the item menu (the category of no item);
  - calling Digmon or Submarimon (the field's saved position read from the player's missing actor).

## v0.17.0 (beta)

- **Extra Digimon belong to the save they were made with.** If a save slot is written by the original game, an emulator or a
  card from elsewhere after the port's, the port no longer attaches the old forms to the new save: it leaves them out and keeps
  a copy of the card as `saves/card1.pre-stale.mcd` before its next save. A save that has extra Digimon still loads in the
  original game, but without them.
- **Launcher** (`dw3launcher.exe`): the disc's first reading moves here from the game, with Play, Settings (every option
  with its description, the controls' keys and gamepad buttons, the mods with their credits), a notice of new versions
  and Tools.
- **Saves page** in the launcher: what each slot of the memory card holds and whether its extra Digimon still belong to the
  save, **Export card** (a backup, or to play on another computer), **Import card** (a PlayStation or emulator `.mcd` works)
  and **Restore last copy**. Every import or restore keeps the card you had in `saves/backups` first.
- The launcher has a language picker with flags at the end of its top bar, and a DW3 icon (also the icon of `dw3launcher.exe`
  and `dw2003.exe`).
- **Tools**: the launcher downloads the model viewer and the field editor from GitHub, sets them up to read the port's
  data and opens them; one already beside the port is used as it is.
- Fixed: the game couldn't start from a folder with accents or other non-English letters in its path (Windows 10 1903
  or later).
- Reading the disc also writes `data/names.txt`, the disc's file names, which the model viewer reads.
- **Effects by Digimon level**: a mod can have images for Rookies, Champions, Ultimates and Megas
  (`data/mods/effects/<id>/champion/` and so on), each folder with its own timing, and `stages.json` says which partner is
  what. A level with no images plays the nearest lower level's, then the base mod, then the original. The model viewer's
  Effects tab makes them: pick the level and drop the PNGs on the window.
- `"active": 0` in an `effect.json` leaves that folder out.
- Fixed: critical hit images could show on an item's or a missed attack's effect, after an earlier critical hit, and a
  double-magic technique only counted its last hit's critical.

## v0.16.0 (beta)

- **Flamedramon as a DNA fusion in the Tag menu**, like Paildramon: a partner with ExVeemon tags a partner with Greymon
  (either way round) and the pair fuses into Flamedramon, using the game's own DNA animation (dark stage, entrance, the
  form's attack, the white flash back). The attack is one of its three signatures, chosen at random, played inside that
  same stage. The fusion costs 100 MP each and is fire. It needs the same `data/mods/7120.bin` as Flamedramon itself.
- **More than one pair per Digimon**: the game's data holds a single Tag partner per form; the port now keeps a table of
  extra pairs, so a form can fuse with several.

## v0.15.1 (beta)

- Fixed: Flamedramon's eyes were half shut in its normal pose. They stay half shut only in the weak pose (a quarter of its HP or less).

## v0.15.0 (beta)

- **Flamedramon's signatures** are Digital Card Battle's three fire attacks, with the animation, camera and effects of that
  game: Fire Rocket (60 MP, power 300), Knuckle Fire (84 MP, 480) and Express Claw (four hits of 150, 110 MP), learnt at
  levels 50, 70 and 85 and shown as signature techniques with their own descriptions. It now needs ExVeemon and Greymon at
  level 40, and its model is `data/mods/7120.bin` (made by dw2-extractor).
- **Textured fire that moves**: Digital Card Battle's effect models scroll their texture as the original did, in their own
  VRAM page. Only those effects (452-458) do; every effect of the game is as it was.
- **Flamedramon** (options menu, on while its model is in `data/mods/7120.bin`): a 46th Digimon of its own, from Digimon
  World 2. A quick fire attacker (strength and speed high, fire-immune, weak to water) with six techniques of its own
  (Ember Jab to Express Claw); learnt with ExVeemon and Greymon. The port
  now holds a list of extra forms rather than one. `--give 472` gives it to every partner on a load (a development aid:
  it goes into the card on the next save, so use a copy).
- **BladeGarurumon's progress is kept in the save file's own spare space.** The memory card file has 2 KB at its end that
  the game never reads or writes; each save slot now has an area there for the forms the port adds (BladeGarurumon, and
  the ones to come), with a checksum and a version. The game's own bytes are unchanged, so a card is still a standard one.
- **Saves made before this are carried over**: loading one reads BladeGarurumon from where it was kept before, and the
  next save writes it into its area. It is also still written where it was, so such a save still loads in the previous
  version with its progress. The first time a save without an area is read or saved, the card is copied once to
  `saves/card1.pre-dw3x.mcd`.
- The memory card is written to a temporary file and renamed over `card1.mcd`, so a crash while saving can't leave a
  half-written card.

## v0.14.0 (beta)

- **Faster drawing**: the game's own 1x picture (the PlayStation's VRAM, which the game reads back for some textures)
  is no longer drawn in software for every primitive. Primitives are queued and drawn only when something reads or
  writes that part of the VRAM, and only those the read depends on; a frame the next clear covers is dropped. In battle
  the CPU's drawing went from 7 ms a frame to 1.6 ms, and the late frames of a fight from about 12 to 2-7.
- What still has to be drawn in software is 2.7 times faster, pixel for pixel the same.
- Screens with no full clear (the title, loading screens, the step from the EXP screen back to the field) draw what is
  queued three frames later, a little each frame, instead of all at once when the queue fills.
- The first fight's props load faster: a stage's OBJ models are read in 2 ms instead of 26, so the fight no longer
  starts with a 78 ms frame.
- The performance meter's CSV notes why the queued VRAM was drawn on each frame it was.

## v0.13.0 (beta)

- **Props in the fight stages** (`data/mods/fightstages/<id>/props.json`, written by the model viewer's Stages tab):
  billboards (grass, flowers, rocks), falling-particle emitters (petals, snow, embers), the stage's own falling leaves
  and small 3D models (an OBJ with a PNG), placed anywhere in the stage. Up to 100000 billboards, 64 emitters of 5000
  particles, 5000 leaves and 64 models. They are hidden by the stage and by the fighters where those are nearer, bend in
  the stage's wind, and take its fog (or a fog of their own).
- **The grass is trodden down under a fighter**: a blade near a fighter's surface is flattened, so the feet no longer
  clip through it.
- All of it is drawn on the card from tables uploaded once: billboards, particles, leaves and 3D models (a mesh kept
  there, one instanced draw per mesh), so the props cost next to nothing: twelve trees of 6000 triangles take 0.07 ms
  of the CPU, where they took 4 ms.
- A stage's props list is baked into a cache (`data/cache/props_<id>.bin`) the first time it is read, and is read again
  only when the JSON changes. A fight keeps its props through a signature move's own stage.
- **Effects' own pictures no longer hitch** the first frames of a signature move: each effect folder of `mods/effects`
  is read when the game starts and its pictures are put on the card one a frame.
- **Performance meter** (F10, or `perf = 1` in `native.ini`): a corner graph while a fight runs, frames stacked by
  game, drawing and swap against the vsync budget, with late frames marked and the card's times for the scene and the
  props; every frame goes to `perf/battle_<time>.csv`.

## v0.12.0 (beta)

- Fight stages edited in the model viewer (v0.5.0 on: its Stages tab, parts moved, removed or taken from other stages,
  OBJs from Blender) play in battle: saved in the game's own format to `data/mods/`, read instead of the disc's.
- Widescreen battles: the whole interface keeps its shape. The HP panels each sit against their side, with a gap
  between them; the partner's picture, its frame and the gauge against the right edge; the messages in the middle.
- **Battle sky and fog** (options menu, Graphics tab, off by default): the battle's flat background becomes a sky,
  darker toward the top and the stage's own colour at the horizon, and the stage's far side fades into it with
  distance (the stages' painted horizon kept clear), so the arena's edge no longer ends against a flat colour. Each
  stage takes its kind's look from `stages.ini` by the exe: open air, night, rooms (fog toward black), water;
  digital, space and the techniques' backdrops are left as they are. Leaves fall through the forests and parks, carried
  by a gusting wind, among the fighters; the forests' trees sway in it and the clouds of East Wire Forest and Central
  Park drift. The game's own picture (1x) stays as it was.

## v0.11.0 (beta)

- Fast text waits for X to be held a third of a second: a tap shows the page whole, as in the original, and the next
  tap goes on, instead of the tap also skipping the page it finished.
- **Controls** (options menu, Input tab): each PlayStation button's two keys and gamepad button, set by pressing them,
  saved in `native.ini` (`keys_cross = Z, Space`, `pad_cross = a`).
- **Button icons** (options menu, Input tab): the menu's help line, tab ends and Controls page show the icons chosen
  there: PS1 (the game's own in its texts), PS3, PS4, PS5 (the default), Xbox 360, Xbox One, Xbox Series, Switch,
  Keyboard, or Auto by the device used last; from Xelu's free prompts pack (CC0), in `prompts/` by the exe.
- The options menu is drawn four times finer, its text and icons sharp at any window size.
- The game's own texts show the face buttons (✕ ○ □ △, as in "△ Button: Back") with the same icons: the key or the
  pad's button each is bound to.

## v0.10.0 (beta)

- **Effects' own images**: a battle's 2D effect drawn from PNGs in `data/mods/effects/<id>/` (the model viewer's
  export format, at any size, with alpha), and a critical hit's from `critical/` there, which the original doesn't
  tell apart. Drawn by the graphics card in the effect's place. The folder's `effect.json`, which the model viewer
  writes, gives each frame's ticks (1/60 s), an offset, a scale and the blend (alpha or added).
- **Digilog**, the journal (options menu, off by default): a last row of the field menu, its seven-row panel made of
  the game's own tiles. It lists the story's steps so far in the item list's panels and pages, the current one first:
  each with its place (the field banner's own name), whether it is done, and what to do, in the five languages; X
  shows its area on the map and comes back. Left / right turn two tabs: Active (the story's step, highlighted, and the
  side quests found, from places visited: DDNA bosses, old weapons, dungeons, Black Kings, errands) and Done.
- **Objective on map** (options menu, off by default): the map screen blinks the area where the story's next step
  happens, read from the stages' own story events.
- **Fast travel** (options menu, off by default): on the map screen, X on a visited area asks whether to travel there;
  on Yes the home mark moves to it and the menu, as it closes, takes you there. Once you have been to Amaterasu,
  Square switches between the two servers' maps, to travel from one to the other. You arrive through one of the game's own doors into the area that is open at that
  point of the story (or at its start, for the two areas no door leads into). After Flawe's Fast Travel hack; no part
  of it is used.
- The options menu in tabs, Display, Graphics, Input and Mods, turned with L1 / R1 (Q / E or Page Up / Down on the
  keyboard) beside the battle menu's own page arrows, read from your disc.
- **F12** saves the game's picture, as drawn, to `screenshots/` by the exe.
- **VSync** (options menu, on by default): frames wait for the screen's refresh, which ends the tearing in
  fullscreen.
- Battle shadows: the partner's shadow while its commands are chosen (its picture beside the menus turned it off).
- The window, resized from the menu or by Widescreen battles, fits the screen it is on and is centred there.
- **Widescreen battles** (options menu, off by default): battles shown 16:9 with the 3D a third wider, the way
  emulators' widescreen hacks do it; the fields stay 4:3. The window turns 16:9 with it. The menus keep their shape,
  against the left or the right edge; the HP panels, the partner's picture and the messages stretch with the picture.
- **Battle shadows** (options menu, on by default): the Digimon in battle cast their own shape on the ground, along a
  light from above, with soft edges (blurred as they darken the ground once), hidden behind them. The original has none.
- **Perspective 3D** (options menu, on by default): the 3D's textures in perspective, by the depth the GTE gave each
  point, without the PlayStation's warping as the camera moves.
- **Smooth 3D textures** (options menu, on by default): the 3D models' textures filtered between their texels when
  enlarged, their see-through texels left out; sprites, text and the backgrounds stay sharp.
- **True colour** (options menu, on by default): the graphics card keeps the colours at 8 bits a channel, without the
  PlayStation's 5-bit banding in shading and blending.
- **Soft shadows** (options menu, off by default): the shadows under the characters on the field darken the ground
  they fall on, with smooth edges at any resolution, instead of the original's grey disc.
- **Fast text** (options menu, on by default): holding X advances the texts, outside battles and card games. After
  markisha64's Dmw2003FastText.
- **Fast forward:** Tab or R3 cycles x2, x3, x5 and off, with ">> xN" in a corner. The music and the movies keep
  their speed.
- **Extra starter packs** (options menu, on by default): Dragon (Agumon, Guilmon, Veemon), Tactical (Kotemon,
  Monmon, Patamon) and Mystic (Kumamon, Renamon, Veemon), on a second page of the choice, with their own texts in the
  five languages. The lobby shows the pack's partners, each with a greeting, and Veemon celebrates with the others.
- **BladeGarurumon** (options menu, on by default): a 45th Digimon every partner can learn, with Growlmon and Grizzmon
  at level 40. Its DNA model gets MetalGarurumon's animations, retargeted at start from your own disc, and a signature
  of its own, Solar Laser: a technique of its own (444), named in every language. It is kept in save bytes the game doesn't use: cards stay standard, and a PlayStation
  simply doesn't see it.

## v0.9.0 (first public release, beta)

Digimon World 2003 (Europe, SLES-03936) as a native Windows program, built from the
[dw3_decomp](https://github.com/ReGame-Labs/dw3_decomp) C code with the console's hardware rewritten for the PC. A beta
until a full playthrough has been checked: see **Not checked yet** below.

**Setup**

- No Python, no tools: the first run opens a window that asks for your disc image (drop the `.cue`/`.bin` on it, or
  browse), checks it is the European game and reads it into `data/` in a few seconds. The window speaks English,
  Spanish, French, German and Italian.
- Nothing of the game's content in the download: the exe reads the game's own tables from your `data/` at startup
  (`dw2003.fill` says where).
- A formatted memory card on the first run, `saves/card1.mcd`; cards from emulators work.

**Graphics**

- OpenGL 3.3, the 3D drawn 1 to 8 times finer than the PlayStation's 320x240 with its exact points (no wobble), the
  battle models through a depth buffer (no flicker at high resolutions).
- The window sized to your screen (1280x960 on a 1080p one) and the 3D as fine, up to 4x, until you choose in the
  options menu.
- Fullscreen (F11 or Alt+Enter), the PlayStation's dithering on or off, the movies smoothed or sharp.

**Sound and movies**

- The sound chip rewritten: music, effects and reverb from your disc's own presets; the movies with their XA audio.

**Controls**

- Keyboard and gamepad (Xbox pads and any SDL knows): analog sticks with a round deadzone, vibration, plugged in or
  swapped while playing.

**Options**

- Esc (gamepad: Back) opens the options menu, saved to `native.ini`: 50 or 60 Hz (the game's own 60 Hz mode),
  resolution, sub-pixel 3D, depth buffer, dithering, smooth movies, fullscreen, window size, vibration, stick
  deadzone.

**For modders and testers**

- Mods as plain files: `data/mods/NNNN.bin` replaces a disc file, of any size; the
  [field editor](https://github.com/Feerdus95/dw3-field-editor) writes its changes there and in `data/mods/stages/`.
- `--field`, `--at`, `--party`, `--event`, `--progress`, `--set` start straight in a field for tests; `debug = 1` in
  `native.ini` turns on the developers' stage select (F1), the sound and vibration tests (F3, F4) and savestates
  (F5 / F9).
- `--extract IMAGE` reads a disc without the window.

**Not checked yet**

- A full story playthrough, battles in depth (DV, signature, DNA), the card game, and sound and music everywhere.
  Reports are welcome: see the issue templates.
- An `.iso` with 2048-byte sectors plays the movies without sound; a `.cue`/`.bin` is best.
