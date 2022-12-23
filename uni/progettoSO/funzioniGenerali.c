#include "lib.h"
#include "funzioniGenerali.h"

int controlloLimiti(Coordinate entita, int tipo)
{
    int flag = false;
    if (tipo == RANA)
    {
        if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO || entita.y <= SEI || entita.y >= ALTEZZA_SCHERMO - CINQUE) 
            flag = true;
    
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
        if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI - UNO)
            flag = true;
    }
    else if (tipo == MACCHINA0)
    {
        if (entita.x < ZERO) {
            flag = 1;
        }
        else if(entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA) {
            flag = 2;
        }
    }

    return flag;
}

int controlloRanaTronco(Coordinate rana, Oggetto tronco[3])
{
    bool flag = false;
    int i, j;

    for (i = 0; i < TRE; i++)
    {
        flag = false;
        for (j = 0; j < LARGHEZZA_RANA; j++)
        {
            if (rana.x + j == tronco[i].coordinate.x + j && rana.y == tronco[i].coordinate.y)
            {
                flag = true;
            }
        }
        if (flag == true)
        {
            rana.x += tronco[i].velocita;
            return rana.x;
        }
    }
    rana.x = -1;
    return rana.x;
}
