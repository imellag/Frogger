#include "lib.h"
#define LIMITE_RANA 6

// input da tastiera
#define A 65
#define a 97
#define D 68
#define d 100
#define P 80
#define p 112
#define Q 81
#define q 113
#define S 83
#define s 115
#define W 87
#define w 119
#define SPACEBAR ' '


// funziona nella quale viene preso l'input dell'utente (movimento, proiettile, uscita)
void* movimentoRana(void* _rana);

// funzione che gestisce il movimento del proiettile
void* funzProiettile(void* _proiettile);

// funzione che stampa la sprite della rana e il suo sfondo in base al terreno sopra il quale si trova
void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty);