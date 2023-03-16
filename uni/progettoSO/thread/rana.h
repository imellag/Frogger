#include "lib.h"

// creo il processo della rana
void funzRana(int gameDifficulty);

// funziona nella quale viene preso l'input dell'utente (movimento, proiettile, uscita)
void movimentoRana(int gameDifficulty);

// funzione che gestisce il movimento del proiettile
void funzProiettile(Oggetto rana, int numeroProiettile);

// funzione che stampa la sprite della rana e il suo sfondo in base al terreno sopra il quale si trova
void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty);