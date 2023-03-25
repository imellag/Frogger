#include "lib.h"
#include "areaDisegno.h"

char spriteTana[3][LARGHEZZA_TANE] = {"     _____     ", "    /     \\    ", "___/       \\___"};

/* per il prato, l'autostrada, il marciapiede, il fiume e le tane stampiamo degli spazi del colore dello sfondo 
che poi vengono eventualmente sovrascritti con il colore degli oggetti che ci passano sopra */

void funzPrato(WINDOW *finestraGioco, int gameDifficulty)
{
    // alto 3
    int i, j;
    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_PRATO));
    for (i = 0; i < ALTEZZA_PRATO; i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
            mvwprintw(finestraGioco, INIZIO_PRATO + (gameDifficulty * 3) + i, j, " ");
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_PRATO));
}

void funzMarciapiede(WINDOW *finestraGioco, int gameDifficulty)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_MARCIAPIEDE));

    for (i = 0; i < ALTEZZA_MARCIAPIEDE; i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
            mvwprintw(finestraGioco, INIZIO_MARCIAPIEDE + (gameDifficulty * 6) + i, j, " ");
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_MARCIAPIEDE));
}

void funzAutostrada(WINDOW *finestraGioco, int gameDifficulty)
{
    // contatori
    int i, j;

    // attivo il colore della strada quindi grigio scuro
    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_AUTOSTRDA));

    // è alto 9 e parte da altezza 20
    for (i = 0; i < ALTEZZA_AUTOSTRADA + (gameDifficulty * 3); i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_AUTOSTRADA + (gameDifficulty * 3) + i, j, " ");
        }
    }

    // spengo il colore
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_AUTOSTRDA));
}

void funzFiume(WINDOW *finestraGioco, int gameDifficulty)
{
    int i, j;
    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_FIUME));
    for (i = 0; i < ALTEZZA_FIUME + (gameDifficulty * 3); i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_FIUME + i, j, " ");
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_FIUME));
}

void stampaPunteggio(WINDOW *finestraGioco,int punteggio)
{
    wattron(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
    /* lo voglio stampare al centro dello schermo, quindi larghezza / 2, poi prendo la larghezza della scritta (in questo caso 5), la divido 
    per 2 (in questo caso usiamo 2) e la sottraggo alle coordinate, in modo che sia perfettamente centrata */
    mvwprintw(finestraGioco, 2, LARGHEZZA_SCHERMO / 2 - 4, "Score: %d", punteggio);
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
}


void funzTane(WINDOW *finestraGioco, bool arrayTane[])
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_MARCIAPIEDE));

    for (i = 0; i < ALTEZZA_TANE; i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_TANE + i, 0 + j, " ");
        }
    }

    stampaTane(finestraGioco, arrayTane);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_MARCIAPIEDE));
}

void stampaTane(WINDOW *finestraGioco, bool arrayTane[])
{
    int i, j, k;
    Coordinate pos;
    pos.x = 0;
    pos.y = INIZIO_TANE;

    for (i = 0; i < NUMERO_TANE; i++)
    {
        if (arrayTane[i] == false)
        { // tana ancora aperta, quindi la stampo
            for (j = 0; j < ALTEZZA_TANE; j++)
            {
                for (k = 0; k < LARGHEZZA_TANE; k++)
                {
                    mvwprintw(finestraGioco, pos.y + j, pos.x + k,"%c", spriteTana[j][k]);
                }
            }
        }
        else
        { // se la tana è chiusa stampo solamente degli underscore
            for (k = 0; k < LARGHEZZA_TANE; k++)
            {
                mvwprintw(finestraGioco, pos.y + ALTEZZA_TANE - 1, pos.x + k, "_");
            }
        }

        pos.x += LARGHEZZA_TANE;

        // ripeto solo per le prime 4 tane: l'ultima avrà diversa larghezza perché è più vicina al bordo dello schermo
        if (i != NUMERO_TANE - 1)
        {
            for (k = 0; k < LARGHEZZA_TANE; k++)
            {
                mvwprintw(finestraGioco, INIZIO_TANE + ALTEZZA_TANE - 1, pos.x + k, "_");
            }
        }
        else
        {
            for (k = 0; k < 5; k++)
            {
                mvwprintw(finestraGioco, INIZIO_TANE + ALTEZZA_TANE - 1, pos.x + k, "_");
            }
        }
        pos.x += LARGHEZZA_TANE;
    }
}