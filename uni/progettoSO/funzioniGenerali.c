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
    else if (tipo == MACCHINA0) // macchina
    {
        if (entita.x < ZERO) {
            flag = 1;
        }
        else if(entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA) {
            flag = 2;
        }
    }
    else if (tipo == MACCHINA1) { // camion
        if (entita.x < ZERO) {
            flag = 1;
        }
        else if(entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_CAMION) {
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

void colori()
{
    init_color(COLORE_RANA, 75, 890, 20);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 150, 150, 150);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito 259 /259/259
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(4, COLOR_BLACK, COLOR_GREEN); // colore prato
    init_pair(5, COLOR_BLACK, COLOR_BLUE);  // colore fiume
    init_pair(6, COLOR_BLACK, COLORE_TRONCHI);
}

