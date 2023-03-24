// dimensioni dei rettangoli utilizzati per l'input del mouse
#define LARGHEZZA_RETTANGOLO 36
#define ALTEZZA_RETTANGOLO 5
// coordinate della x di inizio e fine dei rettangoli stampati
#define INIZIO_RETTANGOLO_AVVIO 50
#define FINE_RETTANGOLO_AVVIO 90

// menu iniziale di gioco che restituisce dati come la difficoltà e le impostazioni
Avvio menuIniziale();

// stampa la sprite della scritta frogger
void stampaFrogger(int iniziox, int inizioy, WINDOW *finestraIniziale);

// stampa i rettangoli contenenti gli input possibili
void stampaRettangolo(WINDOW *finestra, int iniziox, int inizioy);

// stampa le 3 scritte iniziali
void stampaIniziale(WINDOW *finestraIniziale);

// gestisce il menù delle impostazioni
Avvio impostazioni(WINDOW *finestraIniziale, Avvio info);

// stampa i giusti colori in base alle scelte dell'utente
void stampaColoriGiusti(WINDOW *finestraIniziale, Avvio info);

// stampa le scritte ogni iterazione del while
void stampaSprite(WINDOW *finestraIniziale, Avvio info);

// stampa i comandi prima dell'inizio della partita
void stampaComandi(WINDOW *finestraIniziale);
