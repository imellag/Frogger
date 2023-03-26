#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteNemicosulTronco[ALTEZZA_NEMICO][LARGHEZZA_TRONCHI] = {"<~~~~~o\\/o~~~~>", "<~~~~~:||:~~~~>", "<~~~~~./\\.~~~~>"};

// funzione che gestisce il movimento del tronco
void *movimentoTronco(void *_tronco)
{
    Oggetto *tronco = _tronco;
    int i;

    // gli assegno una velocità random a ciascun tronco in base alla difficolta inserita
    pthread_mutex_lock(&mutex);
    int tempoRandom = TEMPO_TRONCO_MIN + rand() % (TEMPO_TRONCO_MIN + TEMPO_TRONCO_MAX) - 1000 * tronco->difficolta;

    // anche le coordinate sono casuali
    tronco->coordinate.y = INIZIO_FIUME + tronco->id * ALTEZZA_TRONCHI;
    tronco->coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI);
    pthread_mutex_unlock(&mutex);

    while (true)
    {
        pthread_mutex_lock(&mutex);
        tronco->coordinate.x += tronco->velocita;

        // se il tronco tocca la fine del fiume allora cambia direzione
        if (controlloLimitiTronco(tronco->coordinate))
            tronco->velocita = tronco->velocita * -1;

        pthread_mutex_unlock(&mutex);
        usleep(tempoRandom);
    }
}

void stampaTronco(WINDOW *finestraGioco, Coordinate tronco)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));

    for (i = 0; i < ALTEZZA_TRONCHI; i++)
    {
        for (j = 0; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, tronco.y + i, tronco.x + j, "%c", spriteTronchi[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));
}

void stampaNemico(WINDOW *finestraGioco, Coordinate nemico)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO)); // ROSSO

    for (i = 0; i < ALTEZZA_NEMICO; i++)
    {
        for (j = 0; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, nemico.y + i, nemico.x + j, "%c", spriteNemicosulTronco[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
}

// funzione che gestisce il proiettile nemico
void *movimentoProiettileNemico(void *_proiettileNemico)
{
    Oggetto *proiettileNemico = (Oggetto *)_proiettileNemico;
    bool proiettileFuori = false;
    while (!proiettileFuori)
    {

        // il proiettile nemico va verso il basso quindi aumento la y
        pthread_mutex_lock(&mutex);

        // se il proiettile raggiunge la fine dello schermo in basso allora lo metto fuori mappa
        if (proiettileNemico->coordinate.y > ALTEZZA_SCHERMO + (6 * proiettileNemico->difficolta) || proiettileNemico->coordinate.x == FUORI_MAPPA - 2)
        {

            proiettileNemico->coordinate.x = FUORI_MAPPA - 2;
            proiettileNemico->coordinate.y = FUORI_MAPPA - 2;
            proiettileFuori = true;
        }
        proiettileNemico->coordinate.y++;
        pthread_mutex_unlock(&mutex);

        usleep(50000);
    }
}