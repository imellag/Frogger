#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};

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
    int i;
    pid_t macchina[3];
    int velocita[3];
    int spostamento;

    
    for(i=0;i<3;i++)
        velocita[i]= (DUE + rand() % (CINQUE - DUE)); 

    spostamento=rand()%2;
    
    if(spostamento==0)
        spostamento=-1;
        else
        spostamento=1;

    
    macchina[ZERO] = fork();
    if (macchina[ZERO] < ZERO)
    {
        printw("Error");
    }
    else if (macchina[ZERO] == ZERO)
    {
        movimentoMacchina(p, ZERO, velocita[ZERO]*spostamento);
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
            movimentoMacchina(p, UNO, velocita[UNO]*spostamento*-1);
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
                movimentoMacchina(p, DUE, velocita[DUE]*spostamento);
            }

            return;
        }
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina, int velocita)
{
    Oggetto macchina;
   
    switch (numeroMacchina)
    {
    case ZERO:
   
            
        macchina.coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 20;
        macchina.id = MACCHINA0;
        macchina.velocita = velocita;
        macchina.pid = getpid();
        
        
        break;
    case UNO:
    
        macchina.coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 23;
        macchina.id = MACCHINA1;
        macchina.velocita = velocita;
        macchina.pid = getpid();
        break;
    case DUE:
        macchina.coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA);
        macchina.coordinate.y = 26;
        macchina.id = MACCHINA2;
        macchina.velocita = velocita;
        macchina.pid = getpid();
        break;
    }

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina, sizeof(Oggetto));
        macchina.coordinate.x += macchina.velocita;
        if (controlloLimiti(macchina.coordinate, MACCHINA0) == 2)
            macchina.coordinate.x = ZERO;

        else if (controlloLimiti(macchina.coordinate, MACCHINA0) == 1)
            macchina.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;

        usleep(100000);
    }
}

void stampaMacchina(Oggetto macchina)
{
    int i, j;
    init_pair(DIECI, COLOR_WHITE, COLORE_AUTOSTRADA);
    attron(COLOR_PAIR(DIECI));
    if (macchina.velocita < 0)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
                mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchineContrario[i][j]);
        }
    }
    else
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
                mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchine[i][j]);
        }
    }

    attroff(COLOR_PAIR(DIECI));
}

/*Oggetto cambioCorsia(){
    Oggetto mac
}*/