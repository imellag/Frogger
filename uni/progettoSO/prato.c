#include "lib.h"
#include "prato.h"

WINDOW* funzPrato() {
    WINDOW *finestraPrato = newwin(3, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 18, 0);
    // alto 3

    wattron(finestraPrato, COLOR_PAIR(4));
    wbkgd(finestraPrato, COLOR_PAIR(4));
    wattroff(finestraPrato, COLOR_PAIR(4)); 

    wrefresh(finestraPrato);
    
    return finestraPrato;
}