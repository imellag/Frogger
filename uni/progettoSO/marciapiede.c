#include "lib.h"
#include "marciapiede.h"

WINDOW* funzMarciapiede() {
    WINDOW *finestraMarciapiede = newwin(3, LARGHEZZA_SCHERMO,  ALTEZZA_SCHERMO -6, 0);
    /* -6 perché lascio tre righe in basso per le info (tempo ecc) e il marciapiede è alto 3 */

    wattron(finestraMarciapiede, COLOR_PAIR(2));    
    wbkgd(finestraMarciapiede, COLOR_PAIR(2));   
    wattroff(finestraMarciapiede, COLOR_PAIR(2));   
    
    wrefresh(finestraMarciapiede);

    return finestraMarciapiede;
}