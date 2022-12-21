#include "lib.h"
#include "funzioniGenerali.h"

int controlloLimiti(Coordinate entita, int tipo)
{
    int flag = false;
    if (tipo == RANA)
    {
        if (entita.x < ZERO) 
        {
            flag = UNO;
        }
        else if(entita.x > LARGHEZZA_SCHERMO){
            flag = DUE;
        }
        else if(entita.y < SEI){
            flag = TRE;
        }
        else if(entita.y >= ALTEZZA_SCHERMO - CINQUE){
            flag = QUATTRO;
        }
        else 
            flag = ZERO;
    }

    else if (tipo == PROIETTILE)
    {
        if (entita.y <NOVE)
        {
            flag = true;
        }
    }
    else if (tipo == TRONCO0)
    {
        if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI - DUE)
            flag = true;
    }
    else if (tipo == MACCHINA0)
    {
        if (entita.x < ZERO || entita.x > LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA)
            flag = true;
    }

    return flag;
}
