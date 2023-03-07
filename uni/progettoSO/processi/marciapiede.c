#include "lib.h"
#include "marciapiede.h"

void funzMarciapiede(WINDOW *finestraGioco, int gameDifficulty)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(DUE));

    for (i = ZERO; i < ALTEZZA_MARCIAPIEDE; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
            mvwprintw(finestraGioco, INIZIO_MARCIAPIEDE + (gameDifficulty * 6) + i, j, " ");
    }
    wattroff(finestraGioco, COLOR_PAIR(DUE));
}