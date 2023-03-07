#include "lib.h"
#include "funzioniGenerali.h"
#include "tane.h"

char spriteProiettile = '|';

_Bool checkCoordinate(int posizione, int coordinata_da_checkare, int intorni)
{
    return (posizione >= coordinata_da_checkare - intorni && posizione <= coordinata_da_checkare + intorni);
}

int controlloLimitiRana(Coordinate entita, int gameDifficulty)
{

    int flag=-1;

    /* prima controllo se le coordinate corrispondono a una tana e restituisco il numero della tana per chiuderla,
    altrimenti restituisco 6 che indica che la rana ha superato il confine */
    const int RAGGIO_HITBOX_TANE = 5; /* La tana e le due caselle ai lati */
    if (entita.y == CINQUE)
    {
        for (int i = ZERO; i < NUMERO_TANE; i++)
        {
            if (checkCoordinate(entita.x, INIZIO_TANE + (LARGHEZZA_TANE * 2 * i), RAGGIO_HITBOX_TANE))
            {
                flag = i;
                break;
            }
        }
        if (!flag)
            flag = SEI;
    }

    else if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO || entita.y <= SEI || entita.y > ALTEZZA_SCHERMO - ALTEZZA_RANA + gameDifficulty*6)
    {
        flag = SEI;
    }

    return flag;
}

int controlloLimiti(Coordinate entita, int tipo)
{
    int flag = ZERO;

    if (tipo >= PROIETTILE0 && tipo <= PROIETTILE29)
    {
        if (entita.y < 9)
        {
            flag = 1;
        }
    }
    else if (tipo == TRONCO0)
    {
        if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI)
            flag = 1;
    }
    else if (tipo == MACCHINA0 || tipo == CAMION0) // macchina o camion
    {
        if (entita.x < -UNO)
        {
            flag = 1;
        }
        else if (entita.x >= LARGHEZZA_SCHERMO)
        {
            flag = 2;
        }
    }
    return flag;
}

int controlloRanaTronco(Coordinate rana, Oggetto tronco[])
{
    /* se la rana è sul tronco aggiorno la sua posizione in base al movimento del tronco e
     restituisco la posizione aggiornata, altrimenti restituisco la posizione -1 di default */
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

void funzTempo(int pOrologio[])
{
    pid_t pidTempo;

    pidTempo = fork();

    if (pidTempo < ZERO)
    {

        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (pidTempo == ZERO)
    {
        orologio(pOrologio);
    }
}

void orologio(int pOrologio[])
{
    Tempo secondo;
    secondo.tempo = false;
    secondo.pid = getpid();
    close(pOrologio[READ]);
    while (true)
    {

        write(pOrologio[WRITE], &secondo, sizeof(Tempo));
        secondo.tempo = true;

        sleep(UNO);
    }
}

Oggetto posizioneInizialeRana(int pRana[], Oggetto rana, int gameDifficulty)
{
    rana.coordinate.x = ZERO;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + (gameDifficulty * 6);
    write(pRana[WRITE], &rana, sizeof(Oggetto));
    clear();

    return rana;
}

// usata per controllare la collisione della rana con i veicoli
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

void colori()
{
    init_color(COLORE_RANA, 75, 890, 20);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 150, 150, 150);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito 259 /259/259
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_color(COLORE_TANA, 541, 271, 0);
    init_color(COLORE_NEMICI, 875, 313, 273); // 224, 80, 70
    init_color(COLORE_FIUME, 59, 699, 996); // 15,179,255
    init_pair(1, COLOR_BLACK, COLORE_NEMICI);
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(4, COLOR_BLACK, COLOR_GREEN); // colore prato
    init_pair(5, COLOR_BLACK, COLORE_FIUME); 
    init_pair(6, COLOR_BLACK, COLORE_TRONCHI);
}

int controlloPosizione(Coordinate oggetto, bool coloriFiume, int gameDifficulty)
{
    if (coloriFiume)
        return COLORE_TRONCHI;
    else if (oggetto.y >= INIZIO_MARCIAPIEDE + (gameDifficulty * 6))
        return COLORE_MARCIAPIEDE;
    else if (oggetto.y >= INIZIO_AUTOSTRADA + (gameDifficulty * 3) && oggetto.y < INIZIO_MARCIAPIEDE + (gameDifficulty * 6))
        return COLORE_AUTOSTRADA;
    else if (oggetto.y >= INIZIO_PRATO + (gameDifficulty * 3))
        return COLOR_GREEN;
    else if (oggetto.y >= OTTO && oggetto.y < INIZIO_PRATO + (gameDifficulty * 3))
        return COLOR_BLUE;
}

void stampaProiettili(WINDOW *finestraGioco, Oggetto arrayTronchi[], bool nemico[], Coordinate proiettile, int gameDifficulty)
{

    int i, j, colore;

    if (controlloPosizione(proiettile, false, gameDifficulty) == COLOR_BLUE)
    {
        colore = 5;
        for (j = ZERO; j < NUMERO_TRONCHI + gameDifficulty; j++)
        {
            if (proiettile.x >= arrayTronchi[j].coordinate.x &&
                proiettile.x <= arrayTronchi[j].coordinate.x + LARGHEZZA_TRONCHI &&
                proiettile.y >= arrayTronchi[j].coordinate.y &&
                proiettile.y <= arrayTronchi[j].coordinate.y + ALTEZZA_TRONCHI)
            {
                colore = 6;
                if (nemico[i] == true)
                {
                    colore = 1;
                }
            }
        }
        soloStampa(finestraGioco, proiettile, colore);
    }
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLORE_AUTOSTRADA)
        soloStampa(finestraGioco, proiettile, 3);
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLORE_MARCIAPIEDE)
        soloStampa(finestraGioco, proiettile, 2);
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLOR_GREEN)
        soloStampa(finestraGioco, proiettile, 4);
}

void soloStampa(WINDOW *finestraGioco, Coordinate proiettile, int colore)
{
    wattron(finestraGioco, COLOR_PAIR(colore));
    mvwprintw(finestraGioco, proiettile.y, proiettile.x, "%c", spriteProiettile);
    wattroff(finestraGioco, COLOR_PAIR(colore));
}