#include "lib.h"
#include "marciapiede.h"

void funzMarciapiede() {
    int i,j;
    
    attron( COLOR_PAIR(DUE));

    for(i = ZERO; i < ALTEZZA_MARCIAPIEDE; i++){
        for(j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        mvprintw(INIZIO_MARCIAPIEDE + i, ZERO + j, " ");
    }
    attroff( COLOR_PAIR(DUE));   
}