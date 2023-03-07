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
    pid_t macchina[NUMERO_MACCHINE];

    int velocitaCorsie[NUMERO_CORSIE];
    // velocità di ciascuna macchina
    int velocita[NUMERO_MACCHINE];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    // randomizzo anche la direzione
    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;

    // randomizzo la velocità
    for (i = ZERO; i < NUMERO_CORSIE; i++)
        velocitaCorsie[i] = UNO;
    for (i = ZERO; i < NUMERO_MACCHINE; i++)
    {
        if (!(i % 3 == ZERO))
            spostamento *= -UNO;

        velocita[i] = velocitaCorsie[i % 3] * spostamento;
    }

    // genero i processi macchina
    for (i = ZERO; i < NUMERO_MACCHINE; i++)
    {
        macchina[i] = fork();
        if (macchina[i] < ZERO)
            printw("Error");
        else if (macchina[i] == ZERO)
        {

            movimentoMacchina(p, i, velocita[i]);
        }
    }

    funzCamion(p, velocitaCorsie, spostamento);
}

void funzCamion(int p[DUE], int velocitaCorsie[], int spostamento)
{
    // contatore
    int i;

    // pid dei camion
    pid_t camion[NUMERO_CAMION];

    // velocità di ciascun camion
    int velocita[NUMERO_CAMION];

    for (i = ZERO; i < NUMERO_CAMION; i++)
    {
        spostamento *= -1;

        velocita[i] = velocitaCorsie[i] * spostamento;
    }

    // genero i processi camion
    for (i = ZERO; i < NUMERO_CAMION; i++)
    {
        camion[i] = fork();

        if (camion[i] < ZERO)
            printw("Error");
        else if (camion[i] == ZERO)
        {

            movimentoCamion(p, i, velocita[i]);
        }
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina, int velocita)
{
    Oggetto macchina;

    int velocitaRandom = MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI);
    srand(getpid());
    int tempoRandom = (MIN_ATTESA + rand() % (MAX_ATTESA - MIN_ATTESA));
    usleep(tempoRandom);

    if (velocita < ZERO)
        macchina.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;
    else
        macchina.coordinate.x = ZERO;

    macchina.coordinate.y = INIZIO_AUTOSTRADA + (numeroMacchina % TRE) * TRE;
    macchina.id = MACCHINA0 + numeroMacchina;
    macchina.velocita = velocita;
    macchina.pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina, sizeof(Oggetto));

        if (velocita < ZERO)
            macchina.coordinate.x--;
        else
            macchina.coordinate.x++;
        if (controlloLimiti(macchina.coordinate, MACCHINA0) == DUE)
        {
            macchina.coordinate.x = ZERO;
        }

        else if (controlloLimiti(macchina.coordinate, MACCHINA0) == UNO)
        {
            macchina.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_MACCHINA;
        }

        usleep(velocitaRandom);
    }
}

void movimentoCamion(int p[DUE], int numeroCamion, int velocita)
{
    Oggetto camion;
    int velocitaRandom = MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI);
    srand(getpid());
    int tempoRandom = (MIN_ATTESA + rand() % (MAX_ATTESA - MIN_ATTESA));
    usleep(tempoRandom);

    if (velocita < ZERO)
        camion.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
    else
        camion.coordinate.x = ZERO;

    camion.coordinate.y = INIZIO_AUTOSTRADA + (numeroCamion % TRE) * TRE;
    camion.id = CAMION0 + numeroCamion;
    camion.velocita = velocita;
    camion.pid = getpid();

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &camion, sizeof(Oggetto));

        if (velocita < ZERO)
            camion.coordinate.x--;
        else
            camion.coordinate.x++;

        if (controlloLimiti(camion.coordinate, CAMION0) == DUE)
        {
            camion.coordinate.x = ZERO;
        }
        else if (controlloLimiti(camion.coordinate, CAMION0) == UNO)
        {
            camion.coordinate.x = LARGHEZZA_SCHERMO - LARGHEZZA_CAMION;
        }
        usleep(velocitaRandom);
    }
    usleep(tempoRandom);
}

// stampa le macchine in modo diverso in base alla loro direzione
void stampaMacchina(Oggetto macchina, int indice)
{
    int i, j;
    init_pair(COLORE_MACCHINA0 + indice, COLORE_MACCHINA0 + indice, COLORE_AUTOSTRADA);

    attron(COLOR_PAIR(COLORE_MACCHINA0 + indice));

    if (macchina.velocita < ZERO)
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

    attroff(COLOR_PAIR(COLORE_MACCHINA0 + indice));
}

// stampa i camion in modo diverso in base alla loro direzione
void stampaCamion(Oggetto camion, int indice)
{
    int i, j;

    init_pair(COLORE_CAMION0 + indice, COLORE_CAMION0 + indice, COLORE_AUTOSTRADA);

    attron(COLOR_PAIR(COLORE_CAMION0 + indice));

    if (camion.velocita < ZERO)
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

    attroff(COLOR_PAIR(COLORE_CAMION0 + indice));
}

Colore coloreVeicolo()
{
    Colore rgb;

    /* mi assicuro che il colore non sia troppo simile a quello
    dell'autostrada per evitare che la macchina sia invisibile */
    do
    {
        rgb.r = rand() % 1000;
        rgb.g = rand() % 1000;
        rgb.b = rand() % 1000;
    } while (rgb.r > 120 && rgb.r < 180 &&
             rgb.r > 120 && rgb.r < 180 &&
             rgb.r > 120 && rgb.r < 180);

    return rgb;
}