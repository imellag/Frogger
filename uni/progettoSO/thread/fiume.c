#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteNemicosulTronco[ALTEZZA_NEMICO][LARGHEZZA_TRONCHI] = {"<~~~~~o\\/o~~~~>", "<~~~~~:||:~~~~>", "<~~~~~./\\.~~~~>"};

int funzTronchi(int gameDifficulty)
{
    int i;
    pid_t tronco[MAX_TRONCHI];

    int velocita[MAX_TRONCHI];
    int spostamento;

}

void *movimentoTronco(void *_tronco)
{
    Oggetto *tronco = _tronco;
    int i;


    int tempoRandom = TEMPO_TRONCO_MIN + rand() % (TEMPO_TRONCO_MIN + TEMPO_TRONCO_MAX) - 2500 * tronco->difficolta;

    tronco->coordinate.y = INIZIO_FIUME + tronco->id * ALTEZZA_TRONCHI;
    tronco->coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI);
    //  tronco->id = TRONCO0 + numeroTronco;
    // tronco->velocita = velocita;

    while (true)
    {
        pthread_mutex_lock(&mutex);
        tronco->coordinate.x += tronco->velocita;

        if (controlloLimitiTronco(tronco->coordinate))
            tronco->velocita = tronco->velocita * -UNO;

        pthread_mutex_unlock(&mutex);
        usleep(tempoRandom);
    }
}

void stampaTronco(WINDOW *finestraGioco, Coordinate tronco)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));

    for (i = ZERO; i < ALTEZZA_TRONCHI; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, tronco.y + i, tronco.x + j, "%c", spriteTronchi[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));
}

void stampaNemico(WINDOW *finestraGioco, Coordinate nemico)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO)); // ROSSO

    for (i = ZERO; i < ALTEZZA_NEMICO; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, nemico.y + i, nemico.x + j, "%c", spriteNemicosulTronco[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
}

void funzProiettileNemico(Coordinate tronco, int i, int gameDifficulty)
{
    pid_t proiettileNemico;

    // system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
    /*
    proiettileNemico = fork();
    if (proiettileNemico < 0)
    {
        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (proiettileNemico == 0)
    {
        movimentoProiettileNemico(tronco, i, gameDifficulty);
        exit(EXIT_FAILURE);
    }
    */
}

void movimentoProiettileNemico(Coordinate tronco, int i, int gameDifficulty)
{
    Oggetto proiettile;
    proiettile.coordinate.x = tronco.x + LARGHEZZA_TRONCHI / DUE;
    proiettile.coordinate.y = tronco.y + ALTEZZA_CORSIE;
    //  proiettile.id = PROIETTILE_NEMICO0 + i;

    while (true)
    {
        if (proiettile.coordinate.y >= ALTEZZA_SCHERMO)
        {
            proiettile.id = PROIETTILE_NEMICO0_OUT + i;
            break;
        }

        usleep(50000);
        proiettile.coordinate.y++;
    }
}