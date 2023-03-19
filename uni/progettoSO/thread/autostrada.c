#include "lib.h"
#include "autostrada.h"
#include "funzioniGenerali.h"

char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
char spriteMacchineContrario[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" _/^\\", "| __ |", " o   o"};
char spriteCamion[ALTEZZA_RANA][LARGHEZZA_CAMION] = {"/_______/^\\_ ", "|_______|___|", " O O O   O O"};
char spriteCamionContrario[ALTEZZA_RANA][LARGHEZZA_CAMION] = {" _/^\\_______\\", "|___|_______|", " O O   O O O"};

void funzAuto(int gameDifficulty)
{
    // contatore
    int i;

    int direzione;

    // direzione della macchina(se -1 va da destra verso sinistra se 1 il contrario)
    int spostamento;
}

void *movimentoVeicolo(void *_veicolo)
{
    parametriVeicolo *veicolo = (parametriVeicolo *)_veicolo;
    pthread_mutex_lock(&mutex);
    int velocitaRandom = veicolo->velocitaCorsia;
    int corsia;
    int tempoRandom = rand() % MAX_ATTESA;

    veicolo->veicolo.velocita = veicolo->direzioneCorsia;
    pthread_mutex_unlock(&mutex);

    while (true)
    {

        do
        {
            pthread_mutex_lock(&mutex);
            if (veicolo->veicolo.velocita < ZERO)
                veicolo->veicolo.coordinate.x--;
            else
                veicolo->veicolo.coordinate.x++;
            pthread_mutex_unlock(&mutex);

            usleep(velocitaRandom);
        } while (!controlloLimitiMacchina(veicolo->veicolo.coordinate));
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