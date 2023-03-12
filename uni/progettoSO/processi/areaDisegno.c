#include "lib.h"
#include "areaDisegno.h"

void funzPrato(WINDOW *finestraGioco, int gameDifficulty)
{

    // alto 3
    int i, j;
    wattron(finestraGioco, COLOR_PAIR(QUATTRO));
    for (i = ZERO; i < ALTEZZA_PRATO; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
            mvwprintw(finestraGioco, INIZIO_PRATO + (gameDifficulty * 3) + i, j, " ");
    }
    wattroff(finestraGioco, COLOR_PAIR(QUATTRO));
}

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

void funzAutostrada(WINDOW *finestraGioco, int gameDifficulty)
{
    // contatori
    int i, j;

    // attivo il colore della strada quindi grigio scuro
    wattron(finestraGioco, COLOR_PAIR(TRE));

    // è alto 9 e parte da altezza 20
    for (i = ZERO; i < ALTEZZA_AUTOSTRADA + (gameDifficulty * 3); i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_AUTOSTRADA + (gameDifficulty * 3) + i, j, " ");
        }
    }

    // spengo il colore
    wattroff(finestraGioco, COLOR_PAIR(TRE));
}

void funzFiume(WINDOW *finestraGioco, int gameDifficulty)
{
    int i, j;
    wattron(finestraGioco, COLOR_PAIR(CINQUE));
    for (i = ZERO; i < ALTEZZA_FIUME + (gameDifficulty * 3); i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_FIUME + i, j, " ");
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(CINQUE));
}
