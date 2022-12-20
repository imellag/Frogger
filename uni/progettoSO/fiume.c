#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"
void funzFiume()
{
    int i,j;
    int corsia = 27;

    attron( COLOR_PAIR(5));
    for(i=0;i<9;i++){
        for(j=0;j<LARGHEZZA_SCHERMO;j++)
            mvprintw(8+i,0+j," ");
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
    int spostamento;
    spostamento = 2;
    Oggetto tronco;

    switch (numeroTronco)
    {
    case 0:
        tronco.coordinate.y = 8;
        tronco.coordinate.x = 10;
        tronco.id = TRONCO0;

        break;
    case 1:
        tronco.coordinate.y = 11;
        tronco.coordinate.x = 0;
        tronco.id = TRONCO1;

        break;
    case 2:
        tronco.coordinate.y = 14;
        tronco.coordinate.x = 20;
        tronco.id = TRONCO2;
        break;
    default:
        break;
    }
    close(p[0]);
    while (true)
    {
        write(p[1], &tronco, sizeof(Oggetto));
        tronco.coordinate.x += spostamento;
        if (controlloLimiti(tronco.coordinate, TRONCO0))
            spostamento = spostamento * -1;

        usleep(100000);
    }
}



/*void funzTronchi(int ptronchi[], int velocitaTronchi[3], Oggetto arrayTronchi[]) {

/*void funzTronchi(int ptronchi[], int velocitaTronchi[3], Oggetto arrayTronchi[]) {
    for (int i = 0; i < 3; i++) {
        if (arrayTronchi[i].coordinate.x + velocitaTronchi[i] + LARGHEZZA_TRONCHI > LARGHEZZA_SCHERMO-1
        || arrayTronchi[i].coordinate.x + velocitaTronchi[i] <= 0)
        {
            velocitaTronchi[i] *= -1;
        }
        else {
            arrayTronchi[i].coordinate.x += velocitaTronchi[i];
        }


    }
    close(ptronchi[0]);
    for (int i = 0; i < 3; i++) {
        write(ptronchi[1], &arrayTronchi[i], sizeof (Oggetto));
    }
}
*/

/* Stavo pensando di aggiungere un controllo del tipo:
       Se il tronco si deve spostare di 3 verso destra ma sono rimasti solo 2 spazi, si sposta
       Prima di 2 verso destra, poi rimbalza e si sposta di 1 verso sinistra. */