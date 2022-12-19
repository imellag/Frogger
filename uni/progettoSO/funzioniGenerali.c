#include "lib.h"
#include "funzioniGenerali.h"

bool controlloLimiti(Coordinate entita, int tipo) {
    bool flag = false;
    if (tipo == RANA) { 
        if (entita.x < 0 || entita.x >= LARGHEZZA_SCHERMO 
        || entita.y < 6
         /* per ora */ || entita.y > ALTEZZA_SCHERMO-6) {
            flag = true;
        }
    }

    else if (tipo == PROIETTILE) { 
        if (entita.y<9){
            flag = true;
            mvprintw(0,0,"%d",flag);
            refresh();
        }

        
    }

    return flag;
}