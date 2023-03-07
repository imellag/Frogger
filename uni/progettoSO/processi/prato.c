#include "lib.h"
#include "prato.h"

void funzPrato(WINDOW* finestraGioco,int gameDifficulty) {
   
    // alto 3
    int i,j;
    wattron(finestraGioco, COLOR_PAIR(QUATTRO));
    for(i = ZERO; i < ALTEZZA_PRATO; i++){
        for(j = ZERO; j < LARGHEZZA_SCHERMO; j++){
            mvwprintw(finestraGioco, INIZIO_PRATO+(gameDifficulty*3) + i, ZERO + j, " ");
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(QUATTRO)); 
}