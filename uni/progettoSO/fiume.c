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
    int ondina = '~';
    int random;
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

int funzTronchi(int p[DUE], int pRana[])
{
    int i;
    pid_t tronco[3];

    int velocita[3];
    int spostamento;

    for (i = 0; i < 3; i++)
        velocita[i] = UNO;

    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    for (i = 0; i < 3; i++)
    {
        tronco[i] = fork();
        if (tronco[i] < ZERO)
        {
            perror("Error");
        }
        else if (tronco[i] == ZERO)
        {
            funzTronco(p, i, velocita[i] * spostamento, pRana);
        }
    }
}
void funzTronco(int p[DUE], int numeroTronco, int velocita, int pRana[])
{
    Oggetto tronco[TRE];
    Oggetto rana;

 

    double diff;
    int spawnNemico;

    srand(getpid());

    tronco[numeroTronco].coordinate.y = 8 + numeroTronco * 3;
    tronco[numeroTronco].coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI);

    tronco[numeroTronco].velocita = velocita;
    tronco[numeroTronco].pid = getpid();

    
    close(p[READ]);

    close(pRana[READ]);
    while (true)
    {

    
            tronco[numeroTronco].id = TRONCO0 + numeroTronco;
            write(p[WRITE], &tronco[numeroTronco], sizeof(Oggetto));
        

        tronco[numeroTronco].coordinate.x += tronco[numeroTronco].velocita;

        if (controlloLimiti(tronco[numeroTronco].coordinate, TRONCO0))
            tronco[numeroTronco].velocita = tronco[numeroTronco].velocita * -UNO;
        usleep(40000);
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
