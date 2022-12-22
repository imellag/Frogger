#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI + UNO] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};

void funzFiume()
{
    int i, j;


    attron(COLOR_PAIR(CINQUE));

      // alto 9
    for (i = ZERO; i < ALTEZZA_FIUME; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
            mvprintw(INIZIO_FIUME + i, ZERO + j, " ");
    }
    attroff(COLOR_PAIR(CINQUE));
}

int funzTronchi(int p[DUE])
{
    pid_t tronco0, tronco1, tronco2;

    tronco0 = fork();
    if (tronco0 < ZERO)
    {
        perror("Error");
    }
    else if (tronco0 == ZERO)
    {
        funzTronco(p, ZERO);
    }
    else
    {
        tronco1 = fork();
        if (tronco1 < ZERO)
        {
            perror("Error");
        }
        else if (tronco1 == ZERO)
        {
            funzTronco(p, UNO);
        }
        else
        {
            tronco2 = fork();
            if (tronco2 < ZERO)
            {
                perror("Error");
            }
            else if (tronco2 == ZERO)
            {
                funzTronco(p, DUE);
            }
            return 1;
        }
    }
}

void funzTronco(int p[DUE], int numeroTronco)
{
    int velocita;
    srand(getpid());
    Oggetto tronco;
    int spostamento=rand()%2;
    if(spostamento==0)
        spostamento=-1;
    else 
        spostamento = 1;
    
    switch (numeroTronco)
    {
    case ZERO:
        tronco.coordinate.y = 8;
        tronco.coordinate.x = LARGHEZZA_TRONCHI;
        tronco.id = TRONCO0;
        tronco.velocita = (DUE + rand() % (CINQUE - DUE))*spostamento;
        tronco.pid=getpid();
        break;
    case UNO:
        tronco.coordinate.y = 11;
        tronco.coordinate.x = ZERO;
        tronco.id = TRONCO1;
        tronco.velocita = (DUE + rand() % (CINQUE - DUE))*spostamento*-1;
        tronco.pid=getpid();
        break;
    case DUE:
        tronco.coordinate.y = 14;
        tronco.coordinate.x = LARGHEZZA_TRONCHI*DUE;
        tronco.id = TRONCO2;
        tronco.velocita = (DUE + rand() % (CINQUE - DUE) )*spostamento;
        tronco.pid=getpid();
        break;
    }
    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &tronco, sizeof(Oggetto));
        tronco.coordinate.x += tronco.velocita;
        if (controlloLimiti(tronco.coordinate, TRONCO0))
            tronco.velocita = tronco.velocita * -UNO;

        usleep(100000);
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
