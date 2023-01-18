#include "lib.h"

#define TEMPO_TRONCO_MIN 25000
#define TEMPO_TRONCO_MAX 40000

void funzFiume();

int funzTronchi(int p[2],int pRana[]);
void funzTronco(int p[2],int numeroTronco,int velocita,int pRana[]);
void stampaTronco(Coordinate tronco);
void stampaNemico(Coordinate nemico);

void funzProiettileNemico(Coordinate tronco,int p[],int i);
void movimentoProiettileNemico(Coordinate tronco, int p[],int i);


