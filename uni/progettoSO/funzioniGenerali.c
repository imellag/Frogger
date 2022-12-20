#include "lib.h"
#include "funzioniGenerali.h"

bool controlloLimiti(Coordinate entita, int tipo)
{
    bool flag = false;
    if (tipo == RANA)
    {
        if (entita.x < 0 || entita.x >= LARGHEZZA_SCHERMO || entita.y < 6 || entita.y >= ALTEZZA_SCHERMO - 5)
        {
            flag = true;
        }
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
        if (entita.x < 0 || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI - 1)
            flag = true;
    }
    else if (tipo == MACCHINA0)
    {
        if (entita.x < 0 || entita.x > LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA)
            flag = true;
    }

    return flag;
}