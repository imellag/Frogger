#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA] = {" o.o ", "+-|-+", "\\-|-/"};

void *movimentoRana(void *_rana)
{

    Oggetto *rana = _rana;

    int inputMovimento;
    int lettura;
    _Bool move;

    while (true)
    {

        timeout(1);
        inputMovimento = getch();
        pthread_mutex_lock(&mutex);
        move = true;
        switch (inputMovimento)
        {
        case w:
        case W:
        case KEY_UP:
            rana->coordinate.y -= ALTEZZA_RANA;
            if (controlloLimitiRana(rana->coordinate, rana->difficolta) == LIMITE_RANA)
                rana->coordinate.y += ALTEZZA_RANA;
            break;
        case s:
        case S:
        case KEY_DOWN:
            rana->coordinate.y += ALTEZZA_RANA;
            if (controlloLimitiRana(rana->coordinate, rana->difficolta) == LIMITE_RANA)
                rana->coordinate.y -= ALTEZZA_RANA;
            break;
        case d:
        case D:
        case KEY_RIGHT:
            rana->coordinate.x += LARGHEZZA_RANA;
            if (controlloLimitiRana(rana->coordinate, rana->difficolta) == LIMITE_RANA)
                rana->coordinate.x -= LARGHEZZA_RANA;
            break;
        case a:
        case A:
        case KEY_LEFT:
            rana->coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimitiRana(rana->coordinate, rana->difficolta) == LIMITE_RANA)
                rana->coordinate.x += LARGHEZZA_RANA;
            break;
        case Q:
        case q:
            rana->id = q;
            move = false;
            break;

        case SPACEBAR:
            move = false;
            rana->id = SPAWN_PROIETTILE;
            break;

        case p:
        case P:
            rana->id = PAUSA;
            move = false;
            break;

        default:
            move = false;
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *funzProiettile(void *_proiettile)
{
    Proiettile *proiettile = _proiettile;

    Coordinate coordinateProiettile;
    // inizializzazione del proiettile, parte dalle coordinate della rana e gli viene assegnato un id
    pthread_mutex_lock(&mutex);
    proiettile->proiettile.id = PROIETTILE0 + proiettile->numeroProiettile;
    proiettile->proiettile.coordinate.x = proiettile->rana.coordinate.x + 2;
    proiettile->proiettile.coordinate.y = proiettile->rana.coordinate.y - 1;
    pthread_mutex_unlock(&mutex);
    while (true)
    {

        pthread_mutex_lock(&mutex);
        coordinateProiettile = proiettile->proiettile.coordinate;
        pthread_mutex_unlock(&mutex);

        if (controlloLimitiProiettile(coordinateProiettile))
        {
            pthread_mutex_lock(&mutex);
            proiettile->proiettile.coordinate.x = FUORI_MAPPA;
            proiettile->proiettile.coordinate.y = FUORI_MAPPA;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_lock(&mutex);
        proiettile->proiettile.coordinate.y--;
        pthread_mutex_unlock(&mutex);

        usleep(50000);
    }
}

void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty)
{
    int i, j, colorePosizione;
    colorePosizione = controlloPosizione(rana, coloreRanaTronco, gameDifficulty);

    init_pair(COLORE_RANA_SFONDO, COLORE_RANA, colorePosizione);
    wattron(finestraGioco, COLOR_PAIR(COLORE_RANA_SFONDO) | A_BOLD);

    for (i = 0; i < ALTEZZA_RANA; i++)
    {
        for (j = 0; j < LARGHEZZA_RANA; j++)
        {
            mvwprintw(finestraGioco, rana.y + i, rana.x + j, "%c", spriteRana[i][j]);
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_RANA_SFONDO) | A_BOLD);
}