#include "lib.h"
#include "autostrada.h"

WINDOW* funzAutostrada(WINDOW* finestraAutostrada) {
    
    wattron(finestraAutostrada, COLOR_PAIR(3));    
    wbkgd(finestraAutostrada, COLOR_PAIR(3));   
    wattroff(finestraAutostrada, COLOR_PAIR(3));   
    
    // wrefresh(finestraAutostrada);

    return finestraAutostrada;
}

void funzAuto() {

}