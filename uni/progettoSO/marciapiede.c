
#include "lib.h"
#include "marciapiede.h"

void funzMarciapiede() {
    int i,j;
    attron( COLOR_PAIR(2));    
    for(i=0;i<3;i++){
        for(j=0;j<LARGHEZZA_SCHERMO;j++)
        mvprintw(29+i,0+j," ");
    }
    attroff( COLOR_PAIR(2));   
    
   


}