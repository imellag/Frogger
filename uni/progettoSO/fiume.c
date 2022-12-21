#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"
void funzFiume()
{
    int i, j;
    int corsia = 27;

    attron(COLOR_PAIR(5));
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
            mvprintw(8 + i, 0 + j, " ");
    }
    attroff(COLOR_PAIR(5));
}

void funzNemici()
{
}

int funzTronchi(int p[2])
{
    pid_t tronco0, tronco1, tronco2;

    tronco0 = fork();
    if (tronco0 < 0)
    {
        perror("Error");
    }
    else if (tronco0 == 0)
    {
        funzTronco(p, 0);
    }
    else
    {
        tronco1 = fork();
        if (tronco1 < 0)
        {
            perror("Error");
        }
        else if (tronco1 == 0)
        {
            funzTronco(p, 1);
        }
        else
        {
            tronco2 = fork();
            if (tronco2 < 0)
            {
                perror("Error");
            }
            else if (tronco2 == 0)
            {
                funzTronco(p, 2);
            }
            return 1;
        }
    }
}

void funzTronco(int p[2], int numeroTronco)
{
    int velocita;
    srand(getpid());
    Oggetto tronco;

    switch (numeroTronco)
    {
    case 0:
        tronco.coordinate.y = 8;
        tronco.coordinate.x = LARGHEZZA_TRONCHI;
        tronco.id = TRONCO0;
        velocita = DUE + rand() % (CINQUE - DUE) + 1;

        break;
    case 1:
        tronco.coordinate.y = 11;
        tronco.coordinate.x = ZERO;
        tronco.id = TRONCO1;
        velocita = DUE + rand() % (CINQUE - DUE) + 1;
        break;
    case 2:
        tronco.coordinate.y = 14;
        tronco.coordinate.x = LARGHEZZA_TRONCHI*DUE;
        tronco.id = TRONCO2;
        velocita = DUE + rand() % (CINQUE - DUE) + 1;
        break;
    }
    close(p[0]);
    while (true)
    {
        write(p[1], &tronco, sizeof(Oggetto));
        tronco.coordinate.x += velocita;
        if (controlloLimiti(tronco.coordinate, TRONCO0))
            velocita = velocita * -1;

        usleep(100000);
    }
}