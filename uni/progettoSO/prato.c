#include "lib.h"
#include "prato.h"

void funzPrato() {
   
    // alto 3
    int i,j;
    attron( COLOR_PAIR(4));
    for(i=0;i<ALTEZZA_PRATO;i++){
        for(j=0;j<LARGHEZZA_SCHERMO;j++){
            mvprintw(INIZIO_PRATO+i,0+j," ");
        }
    }
    attroff(COLOR_PAIR(4)); 

    
}
