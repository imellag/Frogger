#include "lib.h"
#include "prato.h"

void funzPrato() {
   
    // alto 3
    int i,j;
    attron( COLOR_PAIR(QUATTRO));
    for(i = ZERO; i < ALTEZZA_PRATO; i++){
        for(j = ZERO; j < LARGHEZZA_SCHERMO; j++){
            mvprintw(INIZIO_PRATO + i, j, " ");
        }
    }
    attroff(COLOR_PAIR(QUATTRO)); 
}
