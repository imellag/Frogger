#include "lib.h"

int controlloLimiti(Coordinate entita, int tipo);

// controlla se la rana è sul tronco
int controlloRanaTronco(Coordinate rana, Oggetto tronco[]);

//inzializza i colori
void colori();

// fine partita
void gameOver();

// in base alla posizione della rana stampa i colori sottostanti
int controlloPosizione(Coordinate rana,_Bool coloreRanaTronco);