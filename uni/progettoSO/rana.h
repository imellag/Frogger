#include "lib.h"

// funziona nella quale viene preso l'input dell'utente (movimento, proiettile, uscita)
void funzRana(int p[], int pRana[]);
// funzione che gestisce il movimento del proiettile
int funzProiettile(Oggetto rana,int p[2]);
// funzione che stampa la sprite della rana e il suo sfondo in base al terreno sopra il quale si trova 
void stampaRana(Coordinate rana,_Bool coloreRanaTronco);
