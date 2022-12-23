#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};

void funzAutostrada()
{
    // contatori
    int i, j;

    // attivo il colore della strada quindi grigio scuro
    attron(COLOR_PAIR(TRE));

    // è alto 9 e parte da altezza 20 
    for (i = ZERO; i < ALTEZZA_AUTOSTRADA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvprintw(INIZIO_AUTOSTRADA + i, ZERO + j, " ");
        }
    }

    // spengo il colore
    attroff(COLOR_PAIR(TRE));
}

void funzAuto(int p[2])
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[3];

    // velocità di ciascuna macchina
    int velocita[3];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    
    // randomizzo la velocità
    for(i=0;i<3;i++)
        velocita[i]= (DUE + rand() % (CINQUE - DUE)); 

    // randomizzo anche 
    spostamento=rand()%2;
    
    if(spostamento==0)
        spostamento=-1;
        else
        spostamento=1;

    
    // genero i processi macchina
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
    Oggetto macchina[3];
   
    macchina[numeroMacchina].coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA);
    macchina[numeroMacchina].coordinate.y = 20+numeroMacchina*3;
    macchina[numeroMacchina].id = MACCHINA0+numeroMacchina;
    macchina[numeroMacchina].velocita = velocita;
    macchina[numeroMacchina].pid = getpid();    

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina[numeroMacchina], sizeof(Oggetto));
        macchina[numeroMacchina].coordinate.x += macchina[numeroMacchina].velocita;
        if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 2)
            macchina[numeroMacchina].coordinate.x = ZERO;

        else if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 1)
            macchina[numeroMacchina].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;

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