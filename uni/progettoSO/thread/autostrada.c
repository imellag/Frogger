#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/____/^\\_ ", "|____|___|", " O O  O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\____\\", "|___|____|", " O O  O O"};

void *movimentoVeicolo(void *_veicolo)
{
    parametriVeicolo *veicolo = (parametriVeicolo *)_veicolo;

    // non faccio spawnare tutti i veicoli ma gli assegno del tempo random
    pthread_mutex_lock(&mutex);
    int tempoRandom = rand() % 10;
    pthread_mutex_unlock(&mutex);
    sleep(tempoRandom);
    pthread_mutex_lock(&mutex);
    int velocitaRandom = veicolo->velocitaCorsia;
    int corsia;

    veicolo->veicolo.velocita = veicolo->direzioneCorsia;

    pthread_mutex_unlock(&mutex);
    while (true)
    {
        // ciclo fino a quando il veicolo non esce dalla corsia
        while (!controlloLimitiMacchina(veicolo->veicolo.coordinate))
        {
            pthread_mutex_lock(&mutex);
            velocitaRandom = veicolo->velocitaCorsia;

            // in base alla direzione del veicolo mi sposto
            // verso sinistra o verso destra
            if (veicolo->veicolo.velocita < 0)
                veicolo->veicolo.coordinate.x--;
            else
                veicolo->veicolo.coordinate.x++;

            pthread_mutex_unlock(&mutex);
            usleep(velocitaRandom);
        }
        // uscito il veicolo allora gli assegno un id che gestirà areaGioco per dargli una corsia
        pthread_mutex_lock(&mutex);
        veicolo->veicolo.id = MACCHINA0_OUT;
        pthread_mutex_unlock(&mutex);
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

