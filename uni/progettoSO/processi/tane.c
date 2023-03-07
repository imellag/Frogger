#include "lib.h"
#include "tane.h"

char spriteTana[TRE][LARGHEZZA_TANE] = {"     _____     ", "    /     \\    ", "___/       \\___"};

void funzTane(WINDOW *finestraGioco, bool arrayTane[])
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(2));

    for (i = ZERO; i < ALTEZZA_TANE; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_TANE + i, ZERO + j, " ");
        }
    }

    stampaTane(finestraGioco, arrayTane);

    wattroff(finestraGioco, COLOR_PAIR(2));
}

void stampaTane(WINDOW *finestraGioco, bool arrayTane[])
{
    int i, j, k;
    Coordinate pos;
    pos.x = ZERO;
    pos.y = INIZIO_TANE;

    for (i = 0; i < NUMERO_TANE; i++)
    {
        if (arrayTane[i] == false)
        { // tana ancora aperta
            for (j = 0; j < ALTEZZA_TANE; j++)
            {
                for (k = 0; k < LARGHEZZA_TANE; k++)
                {
                    mvwprintw(finestraGioco, pos.y + j, pos.x + k,"%c", spriteTana[j][k]);
                }
            }
        }
        else
        {
            for (k = 0; k < LARGHEZZA_TANE; k++)
            {
                mvwprintw(finestraGioco, pos.y + ALTEZZA_TANE - UNO, pos.x + k, "_");
            }
        }

        pos.x += LARGHEZZA_TANE;

        // ripeto solo per le 4 tane: l'ultima avrà diversa larghezza perché è più vicina al bordo dello schermo
        if (i != NUMERO_TANE - UNO)
        {
            for (k = 0; k < LARGHEZZA_TANE; k++)
            {
                mvwprintw(finestraGioco, INIZIO_TANE + ALTEZZA_TANE - UNO, pos.x + k, "_");
            }
        }
        else
        {
            for (k = 0; k < 5; k++)
            {
                mvwprintw(finestraGioco, INIZIO_TANE + ALTEZZA_TANE - UNO, pos.x + k, "_");
            }
        }
        pos.x += LARGHEZZA_TANE;
    }
}