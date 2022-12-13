#include "lib.h"
#include "marciapiede.h"

WINDOW* funzMarciapiede(WINDOW *finestraMarciapiede) {

    wattron(finestraMarciapiede, COLOR_PAIR(2));    
    wbkgd(finestraMarciapiede, COLOR_PAIR(2));   
    wattroff(finestraMarciapiede, COLOR_PAIR(2));   
    
    // wrefresh(finestraMarciapiede);

    return finestraMarciapiede;
}