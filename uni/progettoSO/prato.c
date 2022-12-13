#include "lib.h"
#include "prato.h"

WINDOW* funzPrato(WINDOW* finestraPrato) {
   
    // alto 3

    wattron(finestraPrato, COLOR_PAIR(4));
    wbkgd(finestraPrato, COLOR_PAIR(4));
    wattroff(finestraPrato, COLOR_PAIR(4)); 

    // wrefresh(finestraPrato);
    
    return finestraPrato;
}