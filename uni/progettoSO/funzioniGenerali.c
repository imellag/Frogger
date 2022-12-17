#include "lib.h"
#include "funzioniGenerali.h"

bool controlloLimiti(Coordinate entita, int tipo) {
    bool flag = false;
    if (tipo == RANA) { 
        if (entita.x < 0 || entita.x >= LARGHEZZA_SCHERMO-LARGHEZZA_RANA 
        || entita.y < 0
         /* per ora */ || entita.y >= ALTEZZA_SCHERMO-3-ALTEZZA_RANA) {
            flag == true;
        }
    }

    if (tipo == PROIETTILE) { 
        if (entita.x < 0)
            flag == true;
    }

    return flag;
}