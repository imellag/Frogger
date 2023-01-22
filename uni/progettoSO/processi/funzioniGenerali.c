#include "lib.h"
#include "funzioniGenerali.h"
#include "tane.h"

_Bool checkCoordinate(int posizione, int coordinata_da_checkare, int intorni)
{
    return (posizione >= coordinata_da_checkare - intorni && posizione <= coordinata_da_checkare + intorni);
}

int controlloLimiti(Coordinate entita, int tipo)
{
    int flag = ZERO;
    if (tipo == RANA)
    {
        /* prima controllo se le coordinate corrispondono a una tana e restituisco il numero della tana per chiuderla,
        altrimenti restituisco 6 che indica che la rana ha superato il confine */
        const int RAGGIO_HITBOX_TANE = CINQUE; /* La tana e le due caselle ai lati */
        if (entita.y == CINQUE)
        {
            for (int i = ZERO; i < NUMERO_TANE; i++)
            {
                if (checkCoordinate(entita.x, INIZIO_TANE + (LARGHEZZA_TANE * DUE * i), RAGGIO_HITBOX_TANE))
                {
                    /* offsettiamo di uno perché return 0 rappresenta nessuna collisione */
                    flag = i + UNO;
                    break;
                }
            }
            /* necessario per qualche motivo TODO: investigare */
            if (!flag)
                flag = SEI;
            /* da cambiare: la rana perde se va in una tana già chiusa o dove non c'è una tana */
            if (!flag)
                flag = SEI;
        }

        else if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO || entita.y <= SEI || entita.y >= ALTEZZA_SCHERMO - CINQUE)
        {
            flag = SEI;
        }
    }

    else if (tipo == PROIETTILE)
    {
        if (entita.y < NOVE)
        {
            flag = UNO;
        }
    }
    else if (tipo == TRONCO0)
    {
        if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI)
            flag = UNO;
    }
    else if (tipo == MACCHINA0 || tipo == CAMION0) // macchina o camion
    {
        if (entita.x < ZERO)
        {
            flag = UNO;
        }
        else if (entita.x >= LARGHEZZA_SCHERMO)
        {
            flag = DUE;
        }
    }
    return flag;
}

int controlloRanaTronco(Coordinate rana, Oggetto tronco[TRE])
{
    bool flag = false;
    int i, j;

    for (i = ZERO; i < TRE; i++)
    {
        flag = false;
        for (j = ZERO; j < LARGHEZZA_RANA; j++)
        {
            if (rana.x + j == tronco[i].coordinate.x + j && rana.y == tronco[i].coordinate.y)
            {
                flag = true;
            }
        }
        if (flag == true)
        {
            rana.x += tronco[i].velocita;
            return rana.x;
        }
    }
    rana.x = -UNO;
    return rana.x;
}

void colori()
{
    init_color(COLORE_RANA, 75, 890, 20);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 150, 150, 150);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito 259 /259/259
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_color(COLORE_TANA, 541, 271, ZERO);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(4, COLOR_BLACK, COLOR_GREEN); // colore prato
    init_pair(5, COLOR_BLACK, COLOR_BLUE);  // colore fiume
    init_pair(6, COLOR_BLACK, COLORE_TRONCHI);
}

int controlloPosizione(Coordinate rana, _Bool coloreRanaTronco)
{
    if (coloreRanaTronco)
        return COLORE_TRONCHI;
    if (rana.y == INIZIO_MARCIAPIEDE)
        return COLORE_MARCIAPIEDE;
    else if (rana.y >= INIZIO_AUTOSTRADA && rana.y < INIZIO_MARCIAPIEDE)
        return COLORE_AUTOSTRADA;
    else if (rana.y == INIZIO_PRATO)
        return COLOR_GREEN;
    else if (rana.y >= OTTO && rana.y < INIZIO_PRATO)
        return COLOR_BLUE;
}

void funzTempo(int pOrologio[])
{
    pid_t pidTempo;

    pidTempo = fork();

    if (pidTempo < ZERO)
    {

        printw("Error");
        exit(1);
    }
    else if (pidTempo == ZERO)
    {
        orologio(pOrologio);
    }
}

void orologio(int pOrologio[])
{
    Schermo schermo;
    schermo.punteggio = 2000;
    schermo.tempo = 50;
    schermo.pid = getpid();
    close(pOrologio[READ]);
    while (true)
    {
        write(pOrologio[WRITE], &schermo, sizeof(Schermo));
        schermo.tempo--;
        schermo.punteggio = schermo.punteggio - DIECI;
        sleep(UNO);
    }
}

Oggetto posizioneInizialeRana(int pRana[], Oggetto rana)
{

    rana.coordinate.x = ZERO;
    rana.coordinate.y = ALTEZZA_SCHERMO - SEI;
    write(pRana[WRITE], &rana, sizeof(Oggetto));
    clear();
}

bool controlloCollisioneOggetti(Oggetto entita, Coordinate rana, int LARGHEZZA_ENTITA)
{
    bool flag = false;
    if (entita.velocita < ZERO)
    {
        if (entita.coordinate.x > rana.x && (entita.coordinate.x - LARGHEZZA_ENTITA) < rana.x && entita.coordinate.y == rana.y)
            flag = true;
    }
    else
    {
        if (entita.coordinate.x < rana.x && (entita.coordinate.x + LARGHEZZA_ENTITA) > rana.x && entita.coordinate.y == rana.y)
            flag = true;
    }
    return flag;
}
