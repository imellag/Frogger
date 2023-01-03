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
    pid_t tronco[3];

    int velocita[3];
    int spostamento;

    for(i=0;i<3;i++)
        velocita[i]= UNO; 

    spostamento=rand()%2;
    
    if(spostamento==0)
        spostamento=-1;
    else
        spostamento=1;

    for(i=0;i<3;i++){
        tronco[i] = fork();
        if (tronco[i] < ZERO)
        {   
            perror("Error");
        }
        else if (tronco[i] == ZERO)
        {
            funzTronco(p, i,velocita[i]*spostamento);
        }
    }
}

void funzTronco(int p[DUE], int numeroTronco,int velocita)
{
    Oggetto tronco[TRE];
    
    int tempoRandom= TEMPO_TRONCO_MIN+rand()%(TEMPO_TRONCO_MAX-TEMPO_TRONCO_MIN);
    
    srand(getpid());
   
    tronco[numeroTronco].coordinate.y = 8+numeroTronco*3;
    tronco[numeroTronco].coordinate.x = rand()%(LARGHEZZA_SCHERMO-LARGHEZZA_TRONCHI) ; 
    tronco[numeroTronco].id = TRONCO0+numeroTronco;
    tronco[numeroTronco].velocita = velocita;
    tronco[numeroTronco].pid=getpid();
     
    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &tronco[numeroTronco], sizeof(Oggetto));
        
          tronco[numeroTronco].coordinate.x +=tronco[numeroTronco].velocita;
         
        if (controlloLimiti(tronco[numeroTronco].coordinate, TRONCO0))
            tronco[numeroTronco].velocita = tronco[numeroTronco].velocita * -UNO;

        usleep(tempoRandom);
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
