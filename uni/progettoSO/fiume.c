#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"
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

    switch (numeroTronco)
    {
    case ZERO:
        tronco.coordinate.y = 8;
        tronco.coordinate.x = LARGHEZZA_TRONCHI;
        tronco.id = TRONCO0;
        velocita = DUE + rand() % (CINQUE - DUE) + UNO;

        break;
    case UNO:
        tronco.coordinate.y = 11;
        tronco.coordinate.x = ZERO;
        tronco.id = TRONCO1;
        velocita = DUE + rand() % (CINQUE - DUE) + UNO;
        break;
    case DUE:
        tronco.coordinate.y = 14;
        tronco.coordinate.x = LARGHEZZA_TRONCHI*DUE;
        tronco.id = TRONCO2;
        velocita = DUE + rand() % (CINQUE - DUE) + UNO;
        break;
    }
    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &tronco, sizeof(Oggetto));
        tronco.coordinate.x += velocita;
        if (controlloLimiti(tronco.coordinate, TRONCO0))
            velocita = velocita * -UNO;

        usleep(100000);
    }
}
