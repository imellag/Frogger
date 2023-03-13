#include "lib.h"
#include "areaDisegno.h"

char spriteTana[TRE][LARGHEZZA_TANE] = {"     _____     ", "    /     \\    ", "___/       \\___"};

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

void stampaPunteggio(WINDOW *finestraGioco,int punteggio)
{
    wattron(finestraGioco, COLOR_PAIR(12));
    mvwprintw(finestraGioco, DUE, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
    wattroff(finestraGioco, COLOR_PAIR(12));
}


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
