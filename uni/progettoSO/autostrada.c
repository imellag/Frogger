#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/_______/^\\_", "|_______|___|", " O O O   O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\_______\\", "|___|_______|", " O O   O O O"};

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
    pid_t macchina[7];

    int velocitaCorsie[3];
    // velocità di ciascuna macchina
    int velocita[7];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    // randomizzo anche
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;
    // randomizzo la velocità
    for(i=0;i<3;i++)
    velocitaCorsie[i] = (DUE + rand() % (CINQUE - DUE)) ;
    for (i = 0; i < 7; i++)
    {
        if (!(i % 3 == 0))
            spostamento *= -1;

        velocita[i] =velocitaCorsie[i%3] * spostamento;
    }

    // genero i processi macchina

    for (i = 0; i < 7; i++)
    {
        macchina[i] = fork();
        if (macchina[i] < ZERO)
            printw("Error");
        else if (macchina[i] == ZERO)
            movimentoMacchina(p, i, velocita);
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina, int velocita[])
{
    Oggetto macchina[7];

    srand(getpid());
    macchina[numeroMacchina].coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA);
    macchina[numeroMacchina].coordinate.y = 20 + (numeroMacchina % 3) * 3;
    macchina[numeroMacchina].id = MACCHINA0 + numeroMacchina;
    macchina[numeroMacchina].velocita = velocita[numeroMacchina];
    macchina[numeroMacchina].pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina[numeroMacchina], sizeof(Oggetto));
        macchina[numeroMacchina].coordinate.x += macchina[numeroMacchina].velocita;
        if (numeroMacchina < 5) {
            if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 2)
                macchina[numeroMacchina]=cambioCorsia(velocita,numeroMacchina);

            else if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 1)
                macchina[numeroMacchina]=cambioCorsia(velocita,numeroMacchina);
        }
        else {
            if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA1) == 2)
                macchina[numeroMacchina]=cambioCorsia(velocita,numeroMacchina);

            else if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA1) == 1)
                macchina[numeroMacchina]=cambioCorsia(velocita,numeroMacchina);
        }
        

        usleep(100000);
    }
}

void stampaMacchina(Oggetto macchina, int veicolo)
{
    int i, j;
    init_pair(DIECI, COLOR_WHITE, COLORE_AUTOSTRADA);
    attron(COLOR_PAIR(DIECI));

    if (i < 5) {
        if (macchina.velocita < 0) {
            for (i = ZERO; i < ALTEZZA_RANA; i++) {
                for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
                    mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchineContrario[i][j]);
            }
        }
        else
        {
            for (i = ZERO; i < ALTEZZA_RANA; i++) {
                for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
                    mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchine[i][j]);
            }
        }
    }
    else {
        if (macchina.velocita < 0) {
            for (i = ZERO; i < ALTEZZA_RANA; i++) {
                for (j = ZERO; j < LARGHEZZA_CAMION; j++)
                    mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteCamionContrario[i][j]);
            }
        }
        else
        {
            for (i = ZERO; i < ALTEZZA_RANA; i++) {
                for (j = ZERO; j < LARGHEZZA_CAMION; j++)
                    mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteCamion[i][j]);
            }
        }
    }

    

    attroff(COLOR_PAIR(DIECI));
}

Oggetto cambioCorsia(int velocita[],int numeroMacchina){

    Oggetto macchina;
    int corsiaCasuale= rand()%3;

    if (numeroMacchina < 5) {
        macchina.coordinate.y=  20 + corsiaCasuale * 3;
        if(velocita[corsiaCasuale]<0)
            macchina.coordinate.x=LARGHEZZA_SCHERMO-LARGHEZZA_MACCHINA;
        else
            macchina.coordinate.x=ZERO;

        macchina.id=MACCHINA0+numeroMacchina;
        macchina.velocita=velocita[corsiaCasuale];
        // macchina.pid=getpid();
    }
    else { // sono i 2 camion
        macchina.coordinate.y=  20 + corsiaCasuale * 3;
        if(velocita[corsiaCasuale]<0)
            macchina.coordinate.x=LARGHEZZA_SCHERMO-LARGHEZZA_CAMION;
        else
            macchina.coordinate.x=ZERO;

        macchina.id=MACCHINA0+numeroMacchina;
        macchina.velocita=velocita[corsiaCasuale];
        // macchina.pid=getpid();
    }

    

    return macchina;

}