#include "lib.h"

#define TEMPO_TRONCO_MIN 40000
#define TEMPO_TRONCO_MAX 60000

/* controlla la posizione dei tronchi, la aggiorna e eventualmente cambia la loro direzione
 se sono arrivati al bordo, e infine invia la posizione aggiornata al main */
void* movimentoTronco(void* _tronco);
// stampa la sprite del tronco
void stampaTronco(WINDOW *finestraGioco, Coordinate tronco);
// stampa la sprite del nemico
void stampaNemico(WINDOW *finestraGioco, Coordinate nemico);
// crea il processo del proiettile nemico
void funzProiettileNemico(Coordinate tronco, int i, int gameDifficulty);
/* controlla la posizione attuale del proiettile nemico, la aggiorna e eventualmente
 invia al main un segnale speciale se il proiettile esce fuori dallo schermo */
void* movimentoProiettileNemico(void* _proiettileNemico);

