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

void stampaIniziale(WINDOW *finestraIniziale);

Avvio impostazioni(WINDOW *finestraIniziale, Avvio info);

void stampaColoriGiusti(WINDOW *finestraIniziale, Avvio info);