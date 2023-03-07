#include "lib.h"

// controlla all'inzio le dimensioni della finestra e
// se è troppo piccola chiede di ingrandire lo schermo
void dimensioneFinestra(int maxx, int maxy);

// stampa le vite rimaste
void stampaVite(WINDOW* finestraGioco, int vite);

// stampa la scritta finale quando il giocatore esaurisce le vite
void gameOver(WINDOW* finestraGioco);

// stampa la scritta finale quando il giocatore chiude tutte le tane
void vittoria(WINDOW* finestraGioco, int punteggio);

// stampa la scritta HAI VINTO! dopo aver chiuso le tane
void stampaScrittaFinale(WINDOW* finestraGioco ,int iniziox, int  inizioy);
