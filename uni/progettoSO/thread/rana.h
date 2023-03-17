#include "lib.h"
#define LIMITE_RANA 6

// funziona nella quale viene preso l'input dell'utente (movimento, proiettile, uscita)
void* movimentoRana(void* _rana);

// funzione che gestisce il movimento del proiettile
void* funzProiettile(void* _proiettile);

// funzione che stampa la sprite della rana e il suo sfondo in base al terreno sopra il quale si trova
void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty);