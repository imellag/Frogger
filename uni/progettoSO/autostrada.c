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

void funzAuto(int p[2])
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[5];

    int velocitaCorsie[3];
    // velocità di ciascuna macchina
    int velocita[5];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    // randomizzo anche
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    // randomizzo la velocità
    for (i = 0; i < 3; i++)
        velocitaCorsie[i] = (DUE + rand() % (CINQUE - DUE));
    for (i = 0; i < 5; i++)
    {
        if (!(i % 3 == 0))
            spostamento *= -1;

        velocita[i] = velocitaCorsie[i % 3] * spostamento;
    }

    // genero i processi macchina

    for (i = 0; i < 5; i++)
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

void funzCamion(int p[2], int velocitaCorsie[], int spostamento)
{

    // contatore
    int i;

    // pid delle macchine
    pid_t camion[3];

    // velocità di ciascuna macchina
    int velocita[3];

    for (i = 0; i < 3; i++)
    {
        spostamento *= -1;

        velocita[i] = velocitaCorsie[i % 3] * spostamento;
    }

    // genero i processi macchina

    for (i = 0; i < 3; i++)
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
    Oggetto macchina[5];

    int tempoRandom = MIN_TEMPO_MACCHINA + rand() % (MAX_TEMPO_MACCHINA - MIN_TEMPO_MACCHINA);
    usleep(100000 + tempoRandom);

    /* if (velocita[numeroMacchina] < 0)
         macchina[numeroMacchina].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;
     else*/

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
        // macchina[numeroMacchina].coordinate.x += macchina[numeroMacchina].velocita;
        if (velocita[numeroMacchina] < 0)
            macchina[numeroMacchina].coordinate.x--;
        else
            macchina[numeroMacchina].coordinate.x++;
        if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 2)
            macchina[numeroMacchina].coordinate.x = ZERO;

        else if (controlloLimiti(macchina[numeroMacchina].coordinate, MACCHINA0) == 1)
            macchina[numeroMacchina].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;

        usleep(tempoRandom);
    }
}

void movimentoCamion(int p[DUE], int numeroCamion, int velocita[])
{
    Oggetto camion[3];

    int tempoRandom = MIN_TEMPO_CAMION + rand() % (MAX_TEMPO_CAMION - MIN_TEMPO_CAMION);
    usleep(100000 + tempoRandom);

    /*
    if (velocita[numeroCamion] < 0)
        camion[numeroCamion].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
    else*/
    srand(getpid());
    camion[numeroCamion].coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_CAMION);
    camion[numeroCamion].coordinate.y = 20 + (numeroCamion % 3) * 3;
    camion[numeroCamion].id = CAMION0 + numeroCamion;
    camion[numeroCamion].velocita = velocita[numeroCamion];
    camion[numeroCamion].pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &camion[numeroCamion], sizeof(Oggetto));
        // camion[numeroCamion].coordinate.x += camion[numeroCamion].velocita;
        if (velocita[numeroCamion] < 0)
            camion[numeroCamion].coordinate.x--;
        else
            camion[numeroCamion].coordinate.x++;

        if (controlloLimiti(camion[numeroCamion].coordinate, CAMION0) == 2)
        {

            camion[numeroCamion].coordinate.x = ZERO;
        }
        else if (controlloLimiti(camion[numeroCamion].coordinate, CAMION0) == 1)
        {
            camion[numeroCamion].coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
        }

        usleep(tempoRandom);
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
            {
                if ((macchina.coordinate.x - j) < ZERO)
                    break;
                mvprintw(macchina.coordinate.y + i, macchina.coordinate.x - j, "%c", spriteMacchineContrario[i][LARGHEZZA_MACCHINA - 1 - j]);
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

    if (camion.velocita < 0)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_CAMION; j++)
            {
                if ((camion.coordinate.x - j) < ZERO)
                    break;
                mvprintw(camion.coordinate.y + i, camion.coordinate.x - j, "%c", spriteCamionContrario[i][LARGHEZZA_CAMION - 1 - j]);
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

Oggetto cambioCorsia(int velocita[], int numeroMacchina, int veicolo)
{

    Oggetto macchina;
    int corsiaCasuale;

    macchina.coordinate.y = 20 + corsiaCasuale * 3;

    // controllo corsia non occupata

    corsiaCasuale = rand() % 3;

    // corsiaCasuale = rand() % 3;
    macchina.coordinate.y = 20 + corsiaCasuale * 3;

    macchina.velocita = velocita[corsiaCasuale];

    macchina.pid = getpid();

    if (veicolo == MACCHINA0)
    {

        if (velocita[corsiaCasuale] < 0)
            macchina.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;
        else
            macchina.coordinate.x = ZERO;

        macchina.id = MACCHINA0 + numeroMacchina;
    }
    else
    {

        if (velocita[corsiaCasuale] < 0)
            macchina.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
        else
            macchina.coordinate.x = ZERO;

        macchina.id = CAMION0 + numeroMacchina;
    }

    usleep(10000);
    return macchina;
}

_Bool postoOccupato(Coordinate veicolo, int corsia, int velocita[], int tipoveicolo)
{
    // sappiamo che la prima corsia parte da 20y

    int i;
    _Bool flag = true;

    char inizioStrada;
    char fineStrada = mvinch(20 + (corsia * 3), LARGHEZZA_SCHERMO - DUE);

    if (tipoveicolo == CAMION0)
    {
        for (i = 0; i < LARGHEZZA_CAMION; i++)
        {

            if (velocita[corsia] > 0)
            {

                inizioStrada = mvinch(21 + (corsia * 3), ZERO + i);
                if (inizioStrada == ' ')
                    flag = false;
                else
                {
                    flag = true;

                    break;
                }
            }
            else
            {

                fineStrada = mvinch(21 + (corsia * 3), LARGHEZZA_SCHERMO - i);

                if (fineStrada == ' ')
                    flag = false;
                else
                {

                    flag = true;
                    break;
                }
            }
        }
    }
    else
    {

        for (i = 0; i < LARGHEZZA_MACCHINA; i++)
        {
            if (velocita[corsia] > 0)
            {
                if (i == 0)
                    inizioStrada = mvinch(21 + (corsia * 3), i);
                else
                    inizioStrada = mvinch(20 + (corsia * 3), i);
                if (inizioStrada == ' ')
                    flag = false;
                else
                {

                    flag = true;
                    break;
                }
            }
            else
            {
                if (i == LARGHEZZA_MACCHINA - 1)
                    fineStrada = mvinch(21 + (corsia * 3), LARGHEZZA_SCHERMO - i);
                else
                    fineStrada = mvinch(20 + (corsia * 3), LARGHEZZA_SCHERMO - i);
                if (fineStrada == ' ')
                    flag = false;
                else
                {

                    flag = true;
                    break;
                }
            }
        }
    }

    return flag;
}
