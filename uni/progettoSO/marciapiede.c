#include "lib.h"
#include "marciapiede.h"

void funzMarciapiede() {
    WINDOW *finestraMarciapiede = newwin(3, LARGHEZZA_SCHERMO, 23, 0);

    wattron(finestraMarciapiede, COLOR_PAIR(2));    
    wbkgd(finestraMarciapiede, COLOR_PAIR(2));   
    wattroff(finestraMarciapiede, COLOR_PAIR(2));   
    
    wrefresh(finestraMarciapiede);

    

}