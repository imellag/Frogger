#pragma once
#include <stdbool.h>

// inizializza lo sfondo delle tane
void funzTane(WINDOW*finestraGioco,bool arrayTane[]);
// stampa le tane effettive, facendo differenza tra quelle aperte e quelle chiuse
void stampaTane(WINDOW*finestraGioco,bool arrayTane[]);
