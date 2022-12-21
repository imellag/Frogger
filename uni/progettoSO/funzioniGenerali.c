#include "lib.h"
#include "funzioniGenerali.h"

int controlloLimiti(Coordinate entita, int tipo)
{
    int flag = false;
    if (tipo == RANA)
    {
        if (entita.x < 0) 
        {
            flag = 1;
        }
        else if(entita.x > LARGHEZZA_SCHERMO){
            flag=2;
        }
        else if(entita.y < 6){
            flag=3;
        }
        else if(entita.y >= ALTEZZA_SCHERMO - 5){
            flag=4;
        }
        else 
            flag=0;
    }

    else if (tipo == PROIETTILE)
    {
        if (entita.y < 9)
        {
            flag = true;
        }
    }
    else if (tipo == TRONCO0)
    {
        if (entita.x < 0 || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI - 2)
            flag = true;
    }
    else if (tipo == MACCHINA0)
    {
        if (entita.x < 0 || entita.x > LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA)
            flag = true;
    }

    return flag;
}
