#include "lib.h"

// valore restituito dalla funzione di controllo delle coordinate se la rana esce dallo schermo
#define LIMITE_RANA 6

// valore restituito dalla funzione di controllo delle coordinate se la rana esce dallo schermo
#define LIMITE_RANA 6

// creo il processo della rana
void funzRana(int p[], int pRana[], int gameDifficulty);

// funziona nella quale viene preso l'input dell'utente (movimento, proiettile, uscita)
void movimentoRana(int p[], int pRana[], int gameDifficulty);

// funzione che gestisce il movimento del proiettile
void funzProiettile(Oggetto rana, int p[2], int numeroProiettile);

// funzione che stampa la sprite della rana e il suo sfondo in base al terreno sopra il quale si trova
void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty);