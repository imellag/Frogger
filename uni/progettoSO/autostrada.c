#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

void funzAutostrada()
{
    int i, j;
    attron(COLOR_PAIR(3));
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < LARGHEZZA_SCHERMO; j++)
        {
            mvprintw(20 + i, 0 + j, " ");
        }
    }
    attroff(COLOR_PAIR(3));
}

void funzAuto(int p[2])
{
    pid_t macchina[3];

    macchina[0] = fork();
    if (macchina[0] < 0)
    {
        printw("Error");
    }
    else if (macchina[0] == 0)
    {
        movimentoMacchina(p, 0);
    }
    else
    {

        macchina[1] = fork();
        if (macchina[1] < 0)
        {
            printw("Error");
        }
        else if (macchina[1] == 0)
        {
            movimentoMacchina(p, 1);
        }
        else
        {

            macchina[2] = fork();
            if (macchina[2] < 0)
            {
                printw("Error");
            }
            else if (macchina[2] == 0)
            {
                movimentoMacchina(p, 2);
            }

            return;
        }
    }
}

void movimentoMacchina(int p[2], int numeroMacchina)
{
    Oggetto macchina;
    int velocita;

    srand(getpid());

    switch (numeroMacchina)
    {
    case 0:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 20;
        macchina.id = MACCHINA0;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    case 1:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 23;
        macchina.id = MACCHINA1;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    case 2:
        macchina.coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 26;
        macchina.id = MACCHINA2;
        velocita = DUE + rand()%(CINQUE-DUE+UNO);
        break;
    }
    
    close(p[0]);
    while (true)
    {
        write(p[1], &macchina, sizeof(Oggetto));
        macchina.coordinate.x += velocita;
        if(controlloLimiti(macchina.coordinate,MACCHINA0))
            macchina.coordinate.x=0;

        usleep(100000);
    }
}

/*Oggetto cambioCorsia(){
    Oggetto mac
}*/