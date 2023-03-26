#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

// sprite dei veicoli verso destra e verso sinistra
char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/____/^\\_ ", "|____|___|", " O O  O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\____\\", "|___|____|", " O O  O O"};

void funzVeicoli(int p[], int gameDifficulty, int pVeicoli[])
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[MAX_MACCHINE + MAX_CAMION];

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;
    int posizione;
    int j, k = 0;
    int numeroVeicoliCorsia;

    // randomizzo anche la direzione
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    // array che salva la velocità della singola corsia che viene creata randomicamente
    int velocitaCorsie[MAX_CORSIE];

    // direzione dei veicoli nella corsia. Se è +1 vanno a destra e se è -1 vanno a sinistra
    int direzioneCorsie[MAX_CORSIE];

    // la direzione delle corsie è alternata
    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
    {
        direzioneCorsie[i] = spostamento;
        spostamento = spostamento * -1;
    }

    // la velocità è indipendente per ogni corsia e viene creata randomicamente
    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI)) - 7500 * gameDifficulty;

    // genero i processi macchina
    for (i = 0; i < NUMERO_MACCHINE + NUMERO_CAMION; i++)
    {
        macchina[i] = fork();
        if (macchina[i] < 0)
            printw("Error");
        else if (macchina[i] == 0)
        {
            if (i < (NUMERO_MACCHINE))
            {
                movimentoVeicolo(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie, pVeicoli, MACCHINA0);
                exit(0);
            }

            else
            {
                movimentoVeicolo(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie, pVeicoli, CAMION0);
                exit(0);
            }
        }
    }
}

void movimentoVeicolo(int p[], int numeroVeicolo, int gameDifficulty, int direzioneCorsie[], int velocitaCorsie[], int pVeicoli[], int tipo)
{
    Oggetto veicolo;
    close(p[READ]);
    close(pVeicoli[WRITE]);
    srand(time(NULL) * getpid());
    int corsia = rand() % (NUMERO_CORSIE + gameDifficulty);
    int velocitaRandom = velocitaCorsie[corsia];
    int tempoRandom = rand() % 10;
    if (tipo == CAMION0)
        veicolo.id = tipo + numeroVeicolo % NUMERO_MACCHINE;
    else
        veicolo.id = tipo + numeroVeicolo;

    veicolo.pid = getpid();
    write(p[WRITE], &veicolo, sizeof(Oggetto));

    sleep(tempoRandom);

    if (direzioneCorsie[corsia] > 0)
        veicolo.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
    else
        veicolo.coordinate.x = -LARGHEZZA_CAMION;

    // i veicoli all'inizio della partita vengono creati all'inizio della corsia
    if (tipo == CAMION0)
        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (corsia * 3) + (gameDifficulty * 3);
    else
        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (corsia * 3) + (gameDifficulty * 3);

    veicolo.velocita = direzioneCorsie[corsia];

    write(p[WRITE], &veicolo, sizeof(Oggetto));

    while (true)
    {
        if (tipo == CAMION0)
            veicolo.id = tipo + numeroVeicolo % NUMERO_MACCHINE;
        else
            veicolo.id = tipo + numeroVeicolo;

        // aggiorno le coordinate se il veicolo è ancora dentro lo schermo
        while (!controlloLimitiMacchina(veicolo.coordinate))
        {
            if (veicolo.velocita < 0)
                veicolo.coordinate.x--;
            else
                veicolo.coordinate.x++;

            usleep(velocitaRandom);
            write(p[WRITE], &veicolo, sizeof(Oggetto));
        }

        veicolo.id = MACCHINA0_OUT;
        write(p[WRITE], &veicolo, sizeof(Oggetto));

        read(pVeicoli[READ], &corsia, sizeof(int));

        veicolo.velocita = direzioneCorsie[corsia];
        if (direzioneCorsie[corsia] < 0)
            veicolo.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
        else
            veicolo.coordinate.x = -LARGHEZZA_CAMION;

        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (corsia * 3) + (gameDifficulty * 3);

        velocitaRandom = velocitaCorsie[corsia];
    }
}

// stampa le macchine in modo diverso in base alla loro direzione
void stampaMacchina(WINDOW *finestraGioco, Oggetto macchina, int indice)
{
    int i, j;
    init_pair(COLORE_MACCHINA0 + indice, COLORE_MACCHINA0 + indice, COLORE_AUTOSTRADA);

    wattron(finestraGioco, COLOR_PAIR(COLORE_MACCHINA0 + indice) | A_BOLD);

    if (macchina.velocita < 0)
    {
        for (i = 0; i < ALTEZZA_RANA; i++)
        {
            for (j = 0; j < LARGHEZZA_MACCHINA; j++)
            {
                if ((macchina.coordinate.x - j) < 0)
                    break;
                mvwprintw(finestraGioco, macchina.coordinate.y + i, macchina.coordinate.x - j, "%c", spriteMacchineContrario[i][LARGHEZZA_MACCHINA - 1 - j]);
            }
        }
    }
    else
    {
        for (i = 0; i < ALTEZZA_RANA; i++)
        {
            for (j = 0; j < LARGHEZZA_MACCHINA; j++)
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

    if (camion.velocita < 0)
    {
        for (i = 0; i < ALTEZZA_RANA; i++)
        {
            for (j = 0; j < LARGHEZZA_CAMION; j++)
            {
                if ((camion.coordinate.x - j) < 0)
                    break;
                mvwprintw(finestraGioco, camion.coordinate.y + i, camion.coordinate.x - j, "%c", spriteCamionContrario[i][LARGHEZZA_CAMION - 1 - j]);
            }
        }
    }
    else
    {
        for (i = 0; i < ALTEZZA_RANA; i++)
        {
            for (j = 0; j < LARGHEZZA_CAMION; j++)
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
    } while (rgb.r > 150 && rgb.r < 350 &&
             rgb.g > 150 && rgb.g < 350 &&
             rgb.b > 150 && rgb.b < 350);

    return rgb;
}