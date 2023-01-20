#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/_______/^\\_ ", "|_______|___|", " O O O   O O"};
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

void funzAuto(int p[DUE])
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[CINQUE];

    int velocitaCorsie[TRE];
    // velocità di ciascuna macchina
    int velocita[CINQUE];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    // randomizzo anche
    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;

    // randomizzo la velocità
    for (i = ZERO; i < TRE; i++)
        velocitaCorsie[i] = UNO;
    for (i = ZERO; i < CINQUE; i++)
    {
        if (!(i % TRE == ZERO))
            spostamento *= -UNO;

        velocita[i] = velocitaCorsie[i % TRE] * spostamento;
    }

    // genero i processi macchina

    for (i = ZERO; i < CINQUE; i++)
    {
        macchina[i] = fork();
        if (macchina[i] < ZERO)
            printw("Error");
        else if (macchina[i] == ZERO)
        {

            movimentoMacchina(p, i, velocita);
        }
    }

    funzCamion(p, velocitaCorsie, spostamento);
}

void funzCamion(int p[DUE], int velocitaCorsie[], int spostamento)
{

    // contatore
    int i;

    // pid delle macchine
    pid_t camion[TRE];

    // velocità di ciascuna macchina
    int velocita[TRE];

    for (i = ZERO; i < TRE; i++)
    {
        spostamento *= -UNO;

        velocita[i] = velocitaCorsie[i] * spostamento;
    }

    // genero i processi macchina

    for (i = ZERO; i < TRE; i++)
    {
        camion[i] = fork();

        if (camion[i] < ZERO)
            printw("Error");
        else if (camion[i] == ZERO)
        {

            movimentoCamion(p, i, velocita);
        }
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina, int velocita[])
{
    Oggetto macchina[CINQUE];

    int velocitaRandom = MIN_TEMPO_MACCHINA + rand() % (MAX_TEMPO_MACCHINA - MIN_TEMPO_MACCHINA);
    srand(getpid());
    int tempoRandom = 20000 + rand() % (3000000 - 1000000);
    usleep(tempoRandom);

    if (velocita[numeroMacchina] < ZERO)
        macchina[numeroMacchina].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;
    else

    macchina[numeroMacchina].coordinate.x = ZERO;
    macchina[numeroMacchina].coordinate.y = 20 + (numeroMacchina % TRE) * TRE;
    macchina[numeroMacchina].id = MACCHINA0 + numeroMacchina;
    macchina[numeroMacchina].velocita = velocita[numeroMacchina];
    macchina[numeroMacchina].pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina[numeroMacchina], sizeof(Oggetto));

        if (velocita[numeroMacchina] < ZERO)
            macchina[numeroMacchina].coordinate.x--;
        else
            macchina[numeroMacchina].coordinate.x++;
        if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == DUE)
            macchina[numeroMacchina].coordinate.x = ZERO;

        else if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == UNO)
            macchina[numeroMacchina].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;

        usleep(velocitaRandom);
    }
}

void movimentoCamion(int p[DUE], int numeroCamion, int velocita[])
{
    Oggetto camion[3];
    int tempoRandom;
    int velocitaRandom = MIN_TEMPO_CAMION + rand() % (MAX_TEMPO_CAMION - MIN_TEMPO_CAMION);
    srand(getpid());
    tempoRandom = 1000000 + rand() % (1000000 - 200000),
    usleep(tempoRandom);

    if (velocita[numeroCamion] < UNO)
        camion[numeroCamion].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
    else
        camion[numeroCamion].coordinate.x = UNO;

    camion[numeroCamion].coordinate.y = 20 + (numeroCamion % TRE) * TRE;
    camion[numeroCamion].id = CAMION0 + numeroCamion;
    camion[numeroCamion].velocita = velocita[numeroCamion];
    camion[numeroCamion].pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &camion[numeroCamion], sizeof(Oggetto));

        if (velocita[numeroCamion] < ZERO)
            camion[numeroCamion].coordinate.x--;
        else
            camion[numeroCamion].coordinate.x++;

        if (controlloLimiti(camion[numeroCamion].coordinate, CAMION0) == DUE)
        {

            camion[numeroCamion].coordinate.x = ZERO;
        }
        else if (controlloLimiti(camion[numeroCamion].coordinate, CAMION0) == UNO)
        {
            camion[numeroCamion].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
        }

        usleep(velocitaRandom);
    }
}

void stampaMacchina(Oggetto macchina)
{
    int i, j;
    init_pair(DIECI, COLOR_WHITE, COLORE_AUTOSTRADA);
    attron(COLOR_PAIR(DIECI));

    if (macchina.velocita < ZERO)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
            {
                if ((macchina.coordinate.x - j) < ZERO)
                    break;
                mvprintw(macchina.coordinate.y + i, macchina.coordinate.x - j, "%c", spriteMacchineContrario[i][LARGHEZZA_MACCHINA - UNO - j]);
            }
        }
    }
    else
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
            {
                if ((macchina.coordinate.x + j) >= LARGHEZZA_SCHERMO)
                    break;
                mvprintw(macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchine[i][j]);
            }
        }
    }

    attroff(COLOR_PAIR(DIECI));
}

void stampaCamion(Oggetto camion)
{

    int i, j;
    init_pair(DIECI, COLOR_WHITE, COLORE_AUTOSTRADA);
    attron(COLOR_PAIR(DIECI));

    if (camion.velocita < ZERO)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_CAMION; j++)
            {
                if ((camion.coordinate.x - j) < ZERO)
                    break;
                mvprintw(camion.coordinate.y + i, camion.coordinate.x - j, "%c", spriteCamionContrario[i][LARGHEZZA_CAMION - UNO - j]);
            }
        }
    }
    else
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_CAMION; j++)
            {
                if ((camion.coordinate.x + j) >= LARGHEZZA_SCHERMO)
                    break;
                mvprintw(camion.coordinate.y + i, camion.coordinate.x + j, "%c", spriteCamion[i][j]);
            }
        }
    }

    attroff(COLOR_PAIR(DIECI));
}