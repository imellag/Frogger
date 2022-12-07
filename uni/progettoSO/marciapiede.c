#include "lib.h"
#include "marciapiede.h"

void marciapiede() {
    WINDOW *finestraMarciapiede = newwin(3, LARGHEZZA_SCHERMO, 26, 0);
    wattron(finestraMarciapiede, COLOR_PAIR(1));    
    wbkgd(finestraMarciapiede, COLOR_PAIR(1));   
    wattroff(finestraMarciapiede, COLOR_PAIR(1));   
    
    wrefresh(finestraMarciapiede);

    

}