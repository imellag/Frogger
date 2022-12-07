#include "lib.h"
#include "autostrada.h"

void funzAutostrada() {
    WINDOW *finestraAutostrada = newwin(9, LARGHEZZA_SCHERMO, 14, 0);

    wattron(finestraAutostrada, COLOR_PAIR(3));    
    wbkgd(finestraAutostrada, COLOR_PAIR(3));   
    wattroff(finestraAutostrada, COLOR_PAIR(3));   
    
    wrefresh(finestraAutostrada);

}
