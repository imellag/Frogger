#include "lib.h"

/* funzione generale utilizzata per diversi oggetti nel gioco, riconosciuti tramite la variabile tipo, 
e in base al movimento di questo controlla che si comporti correttamente una volta arrivato al bordo dello schermo */
int controlloLimiti(Coordinate entita, int tipo);

// controlla se la rana è sul tronco
int controlloRanaTronco(Coordinate rana, Oggetto tronco[]);

//inzializza i colori
void colori();

// fine partita
void gameOver();

// in base alla posizione della rana stampa i colori sottostanti
int controlloPosizione(Coordinate rana,_Bool coloreRanaTronco);

void funzTempo(int pOrologio[]);

void orologio(int pOrologio[]);