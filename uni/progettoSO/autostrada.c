#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

void funzAutostrada()
{
    int i, j;
    attron(COLOR_PAIR(TRE));

    /* è alto 9 e c'era prima il marciapiede di 3 */
    for (i = ZERO; i < ALTEZZA_AUTOSTRADA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvprintw(INIZIO_AUTOSTRADA + i, ZERO + j, " ");
        }
    }
    attroff(COLOR_PAIR(TRE));
}

void funzAuto(int p[2])
{
    pid_t macchina[3];

    macchina[ZERO] = fork();
    if (macchina[ZERO] < ZERO)
    {
        printw("Error");
    }
    else if (macchina[ZERO] == ZERO)
    {
        movimentoMacchina(p, ZERO);
    }
    else
    {

        macchina[UNO] = fork();
        if (macchina[UNO] < ZERO)
        {
            printw("Error");
        }
        else if (macchina[UNO] == ZERO)
        {
            movimentoMacchina(p, UNO);
        }
        else
        {

            macchina[DUE] = fork();
            if (macchina[DUE] < ZERO)
            {
                printw("Error");
            }
            else if (macchina[DUE] == ZERO)
            {
                movimentoMacchina(p, DUE);
            }

            return;
        }
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina)
{
    Oggetto macchina;
    int velocita;

    srand(getpid());

    switch (numeroMacchina)
    {
    case ZERO:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 20;
        macchina.id = MACCHINA0;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    case UNO:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 23;
        macchina.id = MACCHINA1;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    case DUE:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 26;
        macchina.id = MACCHINA2;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    }
    
    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina, sizeof(Oggetto));
        macchina.coordinate.x += velocita;
        if(controlloLimiti(macchina.coordinate,MACCHINA0))
            macchina.coordinate.x=ZERO;

        usleep(100000);
    }
}

/*Oggetto cambioCorsia(){
    Oggetto mac
}*/