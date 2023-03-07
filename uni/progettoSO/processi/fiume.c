#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteNemicosulTronco[ALTEZZA_NEMICO][LARGHEZZA_TRONCHI] = {"<~~~~~o\\/o~~~~>", "<~~~~~:||:~~~~>", "<~~~~~./\\.~~~~>"};

void funzFiume(WINDOW *finestraGioco, int gameDifficulty)
{
    int i, j;
    wattron(finestraGioco, COLOR_PAIR(CINQUE));
    for (i = ZERO; i < ALTEZZA_FIUME + (gameDifficulty * 3); i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_FIUME + i, ZERO + j, " ");
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(CINQUE));
}

int funzTronchi(int p[DUE], int gameDifficulty)
{
    int i;
    pid_t tronco[MAX_TRONCHI];

    int velocita[MAX_TRONCHI];
    int spostamento;

    for (i = ZERO; i < NUMERO_TRONCHI + gameDifficulty; i++)
        velocita[i] = UNO;

    spostamento = rand() % DUE;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    for (i = ZERO; i < NUMERO_TRONCHI + gameDifficulty; i++)
    {
        tronco[i] = fork();
        if (tronco[i] < ZERO)
        {
            perror("Error");
        }
        else if (tronco[i] == ZERO)
        {
            funzTronco(p, i, velocita[i] * spostamento, gameDifficulty);
        }
    }
}

void funzTronco(int p[DUE], int numeroTronco, int velocita, int gameDifficulty)
{
    Oggetto tronco;

    srand(getpid());

    int tempoRandom = TEMPO_TRONCO_MIN + rand() % (TEMPO_TRONCO_MIN + TEMPO_TRONCO_MAX) - 2500 * gameDifficulty;

    tronco.coordinate.y = INIZIO_FIUME + numeroTronco * ALTEZZA_TRONCHI;
    tronco.coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI);
    tronco.id = TRONCO0 + numeroTronco;
    tronco.velocita = velocita;
    tronco.pid = getpid();

    close(p[READ]);

    while (true)
    {
        write(p[WRITE], &tronco, sizeof(Oggetto));

        tronco.coordinate.x += tronco.velocita;

        if (controlloLimiti(tronco.coordinate, TRONCO0))
            tronco.velocita = tronco.velocita * -UNO;
        usleep(tempoRandom);
    }
}

void stampaTronco(WINDOW *finestraGioco, Coordinate tronco)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(SEI));

    for (i = ZERO; i < ALTEZZA_TRONCHI; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, tronco.y + i, tronco.x + j, "%c", spriteTronchi[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(SEI));
}

void stampaNemico(WINDOW *finestraGioco, Coordinate nemico)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(UNO)); // ROSSO

    for (i = ZERO; i < ALTEZZA_NEMICO; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, nemico.y + i, nemico.x + j, "%c", spriteNemicosulTronco[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(UNO));
}

void funzProiettileNemico(Coordinate tronco, int p[], int i, int gameDifficulty)
{
    pid_t proiettileNemico;

    // system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
    proiettileNemico = fork();
    if (proiettileNemico < 0)
    {
        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (proiettileNemico == 0)
    {
        movimentoProiettileNemico(tronco, p, i, gameDifficulty);
        exit(EXIT_FAILURE);
    }
   
}

void movimentoProiettileNemico(Coordinate tronco, int p[], int i, int gameDifficulty)
{
    Oggetto proiettile;
    proiettile.coordinate.x = tronco.x + LARGHEZZA_TRONCHI / DUE;
    proiettile.coordinate.y = tronco.y + UNO;
    proiettile.id = PROIETTILE_NEMICO0 + i;
    proiettile.pid = getpid();
    close(p[READ]);
    while (true)
    {
        if (proiettile.coordinate.y >= ALTEZZA_SCHERMO)
        {
            proiettile.id = PROIETTILE_NEMICO0_OUT + i;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y++;
        usleep(40000);
    }
}