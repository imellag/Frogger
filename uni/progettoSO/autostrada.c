#include "lib.h"
#include "autostrada.h"

WINDOW* funzAutostrada() {
    WINDOW *finestraAutostrada = newwin(9, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 15, 0);
    /* -14 perché è alto 9 e c'era prima il marciapiede di 5 */

    wattron(finestraAutostrada, COLOR_PAIR(3));    
    wbkgd(finestraAutostrada, COLOR_PAIR(3));   
    wattroff(finestraAutostrada, COLOR_PAIR(3));   
    
    wrefresh(finestraAutostrada);

    return finestraAutostrada;
}

void funzAuto() {

}