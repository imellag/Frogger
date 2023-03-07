#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteNemicosulTronco[ALTEZZA_NEMICO][LARGHEZZA_TRONCHI] = {"<~~~~~o\\/o~~~~>", "<~~~~~:||:~~~~>", "<~~~~~./\\.~~~~>"};

void funzFiume()
{
    int i, j;
    attron(COLOR_PAIR(CINQUE));
    // alto 9
    for (i = ZERO; i < ALTEZZA_FIUME; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvprintw(INIZIO_FIUME + i, ZERO + j, " ");
        }
    }
    attroff(COLOR_PAIR(CINQUE));
}

int funzTronchi(int p[DUE])
{
    int i;
    pid_t tronco[NUMERO_TRONCHI];

    int velocita[NUMERO_TRONCHI];
    int spostamento;

    for (i = ZERO; i < NUMERO_TRONCHI; i++)
        velocita[i] = UNO;

    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;

    for (i = ZERO; i < NUMERO_TRONCHI; i++)
    {
        tronco[i] = fork();
        if (tronco[i] < ZERO)
        {
            perror("Error");
        }
        else if (tronco[i] == ZERO)
        {
            funzTronco(p, i, velocita[i] * spostamento);
        }
    }
}

void funzTronco(int p[DUE], int numeroTronco, int velocita)
{
    Oggetto tronco;

    srand(getpid());

    int tempoRandom = TEMPO_TRONCO_MIN + rand() % (TEMPO_TRONCO_MIN + TEMPO_TRONCO_MAX);

    tronco.coordinate.y = INIZIO_FIUME + numeroTronco * 3;
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

void stampaTronco(Coordinate tronco)
{
    int i, j;

    attron(COLOR_PAIR(SEI));

    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvaddch(tronco.y + i, tronco.x + j, spriteTronchi[i][j]);
    }

    attroff(COLOR_PAIR(SEI));
}

void stampaNemico(Coordinate nemico)
{
    int i, j;

    attron(COLOR_PAIR(UNO)); // ROSSO

    for (i = ZERO; i < ALTEZZA_NEMICO; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvaddch(nemico.y + i, nemico.x + j, spriteNemicosulTronco[i][j]);
    }

    attroff(COLOR_PAIR(UNO));
}

void funzProiettileNemico(Coordinate tronco, int p[], int i)
{
    pid_t proiettileNemico;

    // system("ffplay ../file_audio/sparo.mp3 2> /dev/null &");
    proiettileNemico = fork();
    if (proiettileNemico < 0)
    {
        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (proiettileNemico == 0)
    {
        movimentoProiettileNemico(tronco, p, i);
        exit(EXIT_FAILURE);
    }
}

void movimentoProiettileNemico(Coordinate tronco, int p[], int i)
{
    Oggetto proiettile;
    proiettile.coordinate.x = tronco.x + LARGHEZZA_TRONCHI / DUE;
    proiettile.coordinate.y = tronco.y + UNO;
    proiettile.id = PROIETTILE_NEMICO0 + i;
    proiettile.pid = getpid();
    close(p[READ]);
    while (true)
    {
        if (proiettile.coordinate.y > ALTEZZA_SCHERMO - QUATTRO)
        {
            proiettile.id = PROIETTILE_NEMICO0_OUT + i;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        proiettile.coordinate.y++;
        usleep(60000);
    }
}
