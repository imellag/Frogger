#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/_______/^\\_ ", "|_______|___|", " O O O   O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\_______\\", "|___|_______|", " O O   O O O"};

void funzAutostrada(WINDOW *finestraGioco, int gameDifficulty)
{
    // contatori
    int i, j;

    // attivo il colore della strada quindi grigio scuro
    wattron(finestraGioco, COLOR_PAIR(TRE));

    // è alto 9 e parte da altezza 20
    for (i = ZERO; i < ALTEZZA_AUTOSTRADA + (gameDifficulty * 3); i++)
    {
        for (j = ZERO; j < LARGHEZZA_SCHERMO; j++)
        {
            mvwprintw(finestraGioco, INIZIO_AUTOSTRADA + (gameDifficulty * 3) + i, j, " ");
        }
    }

    // spengo il colore
    wattroff(finestraGioco, COLOR_PAIR(TRE));
}

void funzAuto(int p[DUE], int gameDifficulty)
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[MAX_MACCHINE];

    int direzione;
    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;

    // randomizzo anche la direzione
    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;
    int velocitaCorsie[MAX_CORSIE];
    int direzioneCorsie[MAX_CORSIE];
    direzione = spostamento;

    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
    {
        direzioneCorsie[i] = direzione;
        direzione = direzione * -1;
    }
    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
    {
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI))  - 2500 * gameDifficulty;
    }

    // genero i processi macchina
    for (i = ZERO; i < NUMERO_MACCHINE + (3 * gameDifficulty); i++)
    {
        macchina[i] = fork();
        if (macchina[i] < ZERO)
            printw("Error");
        else if (macchina[i] == ZERO)
        {
            movimentoMacchina(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie);
        }
    }

    funzCamion(p, direzioneCorsie, spostamento, gameDifficulty, velocitaCorsie);
}

void funzCamion(int p[DUE], int direzioneCorsie[], int spostamento, int gameDifficulty, int velocitaCorsie[])
{
    // contatore
    int i;

    // pid dei camion
    pid_t camion[MAX_CAMION];

    int direzione;

    direzione = spostamento;

    // genero i processi camion
    for (i = ZERO; i < NUMERO_CAMION + (3 * gameDifficulty); i++)
    {
        camion[i] = fork();

        if (camion[i] < ZERO)
            printw("Error");
        else if (camion[i] == ZERO)
        {

            movimentoCamion(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie);
        }
    }
}

void movimentoMacchina(int p[DUE], int numeroMacchina, int gameDifficulty, int direzioneCorsie[], int velocitaCorsie[])
{
    Oggetto macchina;

    int velocitaRandom = velocitaCorsie[numeroMacchina % (NUMERO_CORSIE + gameDifficulty)];
    srand(getpid());
    int tempoRandom = (MIN_ATTESA + rand() % (MAX_ATTESA - MIN_ATTESA));
    usleep(tempoRandom);

    macchina.coordinate.y = INIZIO_AUTOSTRADA + (numeroMacchina % (NUMERO_CORSIE + gameDifficulty) * 3) + (gameDifficulty * TRE);
    macchina.id = MACCHINA0 + numeroMacchina;
    macchina.velocita = direzioneCorsie[numeroMacchina % (NUMERO_CORSIE + gameDifficulty)];
    macchina.pid = getpid();
    if (macchina.velocita < ZERO)
        macchina.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_MACCHINA;
    else
        macchina.coordinate.x = -LARGHEZZA_MACCHINA;

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &macchina, sizeof(Oggetto));

        if (macchina.velocita < ZERO)
        {

            macchina.coordinate.x--;
            if (controlloLimiti(macchina.coordinate, MACCHINA0) == UNO)
                macchina.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_MACCHINA;
        }
        else
        {
            macchina.coordinate.x++;

            if (controlloLimiti(macchina.coordinate, MACCHINA0) == DUE)
                macchina.coordinate.x = -LARGHEZZA_MACCHINA;
        }

        usleep(velocitaRandom);
    }
}

void movimentoCamion(int p[DUE], int numeroCamion, int gameDifficulty, int direzioneCorsie[], int velocitaCorsie[])
{
    Oggetto camion;
    int velocitaRandom = velocitaCorsie[numeroCamion % (NUMERO_CORSIE + gameDifficulty)];
    srand(getpid());
    int tempoRandom = (MIN_ATTESA + rand() % (MAX_ATTESA - MIN_ATTESA));
    usleep(tempoRandom);

    camion.coordinate.y = INIZIO_AUTOSTRADA + (gameDifficulty * 3) + (numeroCamion % (NUMERO_CORSIE + gameDifficulty) * 3);
    camion.id = CAMION0 + numeroCamion;
    camion.velocita = direzioneCorsie[numeroCamion % (NUMERO_CORSIE + gameDifficulty)];
    camion.pid = getpid();

    if (camion.velocita < ZERO)
        camion.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
    else
        camion.coordinate.x = -LARGHEZZA_CAMION;

    close(p[READ]);
    while (true)
    {
        write(p[WRITE], &camion, sizeof(Oggetto));

        if (camion.velocita < ZERO)
        {
            camion.coordinate.x--;
            if (controlloLimiti(camion.coordinate, CAMION0) == UNO)
                camion.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
        }

        else
        {
            camion.coordinate.x++;
            if (controlloLimiti(camion.coordinate, CAMION0) == DUE)
                camion.coordinate.x = -LARGHEZZA_CAMION;
        }

        usleep(velocitaRandom);
    }
    usleep(1000000);
}

// stampa le macchine in modo diverso in base alla loro direzione
void stampaMacchina(WINDOW *finestraGioco, Oggetto macchina, int indice)
{
    int i, j;
    init_pair(COLORE_MACCHINA0 + indice, COLORE_MACCHINA0 + indice, COLORE_AUTOSTRADA);

    wattron(finestraGioco, COLOR_PAIR(COLORE_MACCHINA0 + indice) | A_BOLD);

    if (macchina.velocita < ZERO)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
            {
                if ((macchina.coordinate.x - j) < ZERO)
                    break;
                mvwprintw(finestraGioco, macchina.coordinate.y + i, macchina.coordinate.x - j, "%c", spriteMacchineContrario[i][LARGHEZZA_MACCHINA - 1 - j]);
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
                mvwprintw(finestraGioco, macchina.coordinate.y + i, macchina.coordinate.x + j, "%c", spriteMacchine[i][j]);
            }
        }
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_MACCHINA0 + indice) | A_BOLD);
}

// stampa i camion in modo diverso in base alla loro direzione
void stampaCamion(WINDOW *finestraGioco, Oggetto camion, int indice)
{
    int i, j;

    init_pair(COLORE_CAMION0 + indice, COLORE_CAMION0 + indice, COLORE_AUTOSTRADA);

    wattron(finestraGioco, COLOR_PAIR(COLORE_CAMION0 + indice) | A_BOLD);

    if (camion.velocita < ZERO)
    {
        for (i = ZERO; i < ALTEZZA_RANA; i++)
        {
            for (j = ZERO; j < LARGHEZZA_CAMION; j++)
            {
                if ((camion.coordinate.x - j) < ZERO)
                    break;
                mvwprintw(finestraGioco, camion.coordinate.y + i, camion.coordinate.x - j, "%c", spriteCamionContrario[i][LARGHEZZA_CAMION - 1 - j]);
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
                mvwprintw(finestraGioco, camion.coordinate.y + i, camion.coordinate.x + j, "%c", spriteCamion[i][j]);
            }
        }
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_CAMION0 + indice) | A_BOLD);
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
             rgb.g > 120 && rgb.g < 180 &&
             rgb.b > 120 && rgb.b < 180);

    return rgb;
}
