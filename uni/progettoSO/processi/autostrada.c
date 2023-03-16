#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/_______/^\\_ ", "|_______|___|", " O O O   O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\_______\\", "|___|_______|", " O O   O O O"};

void funzAuto(int p[], int gameDifficulty, int pVeicoli[])
{
    // contatore
    int i;

    // pid delle macchine
    pid_t macchina[MAX_MACCHINE+MAX_CAMION];

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
    Coordinate inizioVeicoli[MAX_CAMION + MAX_MACCHINE];
    direzione = spostamento;

    for (i = 0; i < NUMERO_MACCHINE + NUMERO_CAMION + (gameDifficulty * 2); i++)
    {
        do
        {
            inizioVeicoli[i].x = rand() % LARGHEZZA_SCHERMO;
            inizioVeicoli[i].y = rand() % (NUMERO_CORSIE + gameDifficulty);
        } while (controlloInizioCoordinateCorsie(inizioVeicoli, i));
    }
    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
    {
        direzioneCorsie[i] = direzione;
        direzione = direzione * -1;
    }

    for (i = 0; i < NUMERO_CORSIE + gameDifficulty; i++)
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI)) - 2500 * gameDifficulty;

    // genero i processi macchina
    for (i = ZERO; i < NUMERO_MACCHINE + NUMERO_CAMION +  (2*gameDifficulty); i++)
    {
        macchina[i] = fork();
        if (macchina[i] < ZERO)
            printw("Error");
        else if (macchina[i] == ZERO)
        {
            if (i < (NUMERO_MACCHINE + gameDifficulty ))
                movimentoVeicolo(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie, inizioVeicoli, pVeicoli, MACCHINA0);

            else
                movimentoVeicolo(p, i, gameDifficulty, direzioneCorsie, velocitaCorsie, inizioVeicoli, pVeicoli, CAMION0);
        }
    }


}

void movimentoVeicolo(int p[DUE], int numeroVeicolo, int gameDifficulty, int direzioneCorsie[], int velocitaCorsie[], Coordinate inizioVeicoli[], int pVeicoli[], int tipo)
{
    Oggetto veicolo;
    int velocitaRandom = velocitaCorsie[inizioVeicoli[numeroVeicolo].y];
    srand(getpid());
    int corsia;
    int tempoRandom = rand() % MAX_ATTESA;
      veicolo.coordinate.x = inizioVeicoli[numeroVeicolo].x;
  
    if (tipo == CAMION0)
        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (inizioVeicoli[numeroVeicolo].y * 3) + (gameDifficulty * TRE);
    else
        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (inizioVeicoli[numeroVeicolo].y * 3) + (gameDifficulty * TRE);

    veicolo.velocita = direzioneCorsie[inizioVeicoli[numeroVeicolo].y];
    veicolo.pid = getpid();

    close(p[READ]);
    while (true)
    {
        if (tipo == CAMION0)
            veicolo.id = tipo + numeroVeicolo%( NUMERO_MACCHINE + gameDifficulty);
            else
             veicolo.id = tipo + numeroVeicolo ;

        do
        {

            if (veicolo.velocita < ZERO)
                veicolo.coordinate.x--;
            else
                veicolo.coordinate.x++;

            usleep(velocitaRandom);
            write(p[WRITE], &veicolo, sizeof(Oggetto));

        } while (!controlloLimitiMacchina(veicolo.coordinate));
        veicolo.id = MACCHINA0_OUT;
        write(p[WRITE], &veicolo, sizeof(Oggetto));

        read(pVeicoli[READ], &corsia, sizeof(int));

        veicolo.velocita = direzioneCorsie[corsia];
        if (direzioneCorsie[corsia] < 0)
            veicolo.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
        else
            veicolo.coordinate.x = -LARGHEZZA_CAMION;
        veicolo.coordinate.y = INIZIO_AUTOSTRADA + (corsia * 3) + (gameDifficulty * TRE);

        velocitaRandom = velocitaCorsie[corsia];
    }
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
bool controlloInizioCoordinateCorsie(Coordinate inizioVeicoli[], int i)
{
    bool flag = false;
    int j;
    for (j = 0; j < i; j++)
    {
        if ((inizioVeicoli[j].x <= inizioVeicoli[i].x) && ((inizioVeicoli[j].x + LARGHEZZA_CAMION) >= (inizioVeicoli[i].x + LARGHEZZA_CAMION)) &&
            (inizioVeicoli[j].y == inizioVeicoli[i].y))
        {
            flag = true;
            break;
        }
        return flag;
    }
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
    } while (rgb.r > 100 && rgb.r < 200 &&
             rgb.g > 100 && rgb.g < 200 &&
             rgb.b > 100 && rgb.b < 200);

    return rgb;
}