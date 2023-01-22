#include "lib.h"

#define TEMPO_TRONCO_MIN 20000
#define TEMPO_TRONCO_MAX 60000

void funzFiume();

int funzTronchi(int p[DUE],int pRana[]);
void funzTronco(int p[DUE],int numeroTronco,int velocita,int pRana[]);
void stampaTronco(Coordinate tronco);
void stampaNemico(Coordinate nemico);

void funzProiettileNemico(Coordinate tronco,int p[],int i);
void movimentoProiettileNemico(Coordinate tronco, int p[],int i);
