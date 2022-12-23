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
    int i;
    pid_t tronco0, tronco1, tronco2;


     int velocita[3];
    int spostamento;

    
    for(i=0;i<3;i++)
        velocita[i]= (DUE + rand() % (CINQUE - DUE)); 

    spostamento=rand()%2;
    
    if(spostamento==0)
        spostamento=-1;
        else
        spostamento=1;


    tronco0 = fork();
    if (tronco0 < ZERO)
    {
        perror("Error");
    }
    else if (tronco0 == ZERO)
    {
        funzTronco(p, ZERO,velocita[ZERO]*spostamento);
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
            funzTronco(p, UNO,velocita[UNO]*spostamento*-1);
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
                funzTronco(p, DUE,velocita[DUE]*spostamento);
            }
            return 1;
        }
    }
}

void funzTronco(int p[DUE], int numeroTronco,int velocita)
{
   Oggetto tronco;
    srand(getpid());
    switch (numeroTronco)
    {
    case ZERO:
        tronco.coordinate.y = 8;
        tronco.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_TRONCHI) ; 
        tronco.id = TRONCO0;
        tronco.velocita = velocita;
        tronco.pid=getpid();
        break;
    case UNO:
        tronco.coordinate.y = 11;
        tronco.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_TRONCHI) ; 
        tronco.id = TRONCO1;
          tronco.velocita = velocita;
        tronco.pid=getpid();
        break;
    case DUE:
        tronco.coordinate.y = 14;
        tronco.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_TRONCHI) ; 
        tronco.id = TRONCO2;
          tronco.velocita = velocita;
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
