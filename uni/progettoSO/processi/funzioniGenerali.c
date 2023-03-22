#include "lib.h"
#include "funzioniGenerali.h"

char spriteProiettile = '|';

_Bool checkCoordinate(int posizione, int coordinata_da_checkare, int intorni)
{
    return (posizione >= coordinata_da_checkare - intorni && posizione <= coordinata_da_checkare + intorni);
}

int controlloLimitiRana(Coordinate entita, int gameDifficulty)
{

    int flag = -1;

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
        if (flag==-1)
            flag = SEI;
    }

    else if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO || entita.y <= SEI ||
             entita.y > ALTEZZA_SCHERMO - ALTEZZA_RANA + gameDifficulty * 6)
        flag = SEI;

    return flag;
}
int controlloLimitiMacchina(Coordinate entita)
{

    int flag = 0;

    if (entita.x < -LARGHEZZA_CAMION)
        flag = 1;

    else if (entita.x > LARGHEZZA_SCHERMO+LARGHEZZA_CAMION)
        flag = 2;

    return flag;
}
int controlloLimitiCamion(Coordinate entita)
{

    int flag = 0;
    if (entita.x < -UNO)
        flag = 1;

    else if (entita.x >= LARGHEZZA_SCHERMO)
        flag = 2;

    return flag;
}

int controlloLimitiProiettile(Coordinate entita)
{
    bool flag = false;

    if (entita.y < 9)
        flag = true;

    return flag;
}

int controlloLimitiTronco(Coordinate entita)
{
    bool flag = false;
    if (entita.x < ZERO || entita.x >= LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI)
        flag = true;

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
                flag = true;
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

void funzTempo(int p[])
{
    pid_t pidTempo;

    pidTempo = fork();

    if (pidTempo < ZERO)
    {

        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (pidTempo == ZERO){
        orologio(p);
        exit(0);
    }
}

void orologio(int p[])
{
    Oggetto secondo;
    secondo.id = TEMPO;
    secondo.velocita = 0;
    secondo.pid = getpid();
    close(p[READ]);
    while (true)
    {

        write(p[WRITE], &secondo, sizeof(Oggetto));
        secondo.velocita = 1;

        sleep(UNO);
    }
}

Oggetto posizioneInizialeRana(int pRana[], Oggetto rana, int gameDifficulty)
{
    rana.coordinate.x = ZERO;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + (gameDifficulty * 6);
    write(pRana[WRITE], &rana, sizeof(Oggetto));
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

void colori(Avvio info)
{

    init_color(COLORE_RANA, info.colore.r, info.colore.g, info.colore.b);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 150, 150, 150);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito 259 /259/259
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_color(COLORE_TANA, 541, 271, 0);
    init_color(COLORE_NEMICI, 875, 313, 273); // 224, 80, 70
    init_color(COLORE_FIUME, 59, 699, 996);   // 15,179,255
    init_color(COLORE_PRATO, 114, 569, 251); // 29, 145, 64
    init_pair(COLORE_NEMICI_TRONCO, COLOR_BLACK, COLORE_NEMICI);
    init_pair(COLORE_SFONDO_MARCIAPIEDE, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(COLORE_SFONDO_AUTOSTRDA, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(COLORE_SFONDO_PRATO, COLOR_BLACK, COLORE_PRATO); // colore prato
    init_pair(COLORE_SFONDO_FIUME, COLOR_BLACK, COLORE_FIUME); // colore fiume
    init_pair(COLORE_SFONDO_TRONCHI, COLOR_BLACK, COLORE_TRONCHI);
    init_pair(COLORE_VERDE_NERO, COLOR_GREEN, COLOR_BLACK); // colore delle scritte
    init_pair(COLORE_NERO, COLOR_BLACK, COLOR_BLACK); // colore nero
    init_pair(COLORE_SCRITTE_INFO, COLORE_NEMICI, COLOR_BLACK); // colore scritte schermata(puntaggio tempo e vite)
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
        return COLORE_PRATO;
    else if (oggetto.y >= OTTO && oggetto.y < INIZIO_PRATO + (gameDifficulty * 3))
        return COLOR_BLUE;
}

void stampaProiettili(WINDOW *finestraGioco, Oggetto arrayTronchi[], bool nemico[], Coordinate proiettile, int gameDifficulty)
{

    int i, j, colore;

    if (controlloPosizione(proiettile, false, gameDifficulty) == COLOR_BLUE)
    {
        colore = COLORE_SFONDO_FIUME;
        for (j = ZERO; j < NUMERO_TRONCHI + gameDifficulty; j++)
        {
            if (proiettile.x >= arrayTronchi[j].coordinate.x &&
                proiettile.x <= arrayTronchi[j].coordinate.x + LARGHEZZA_TRONCHI &&
                proiettile.y >= arrayTronchi[j].coordinate.y &&
                proiettile.y <= arrayTronchi[j].coordinate.y + ALTEZZA_TRONCHI)
            {
                colore = COLORE_SFONDO_TRONCHI;
                if (nemico[i] == true)
                {
                    colore = COLORE_NEMICI_TRONCO;
                }
            }
        }
        soloStampa(finestraGioco, proiettile, colore);
    }
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLORE_AUTOSTRADA)
        soloStampa(finestraGioco, proiettile, COLORE_SFONDO_AUTOSTRDA);
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLORE_MARCIAPIEDE)
        soloStampa(finestraGioco, proiettile, COLORE_SFONDO_MARCIAPIEDE);
    else if (controlloPosizione(proiettile, false, gameDifficulty) == COLORE_PRATO)
        soloStampa(finestraGioco, proiettile, COLORE_SFONDO_PRATO);
}

void soloStampa(WINDOW *finestraGioco, Coordinate proiettile, int colore)
{
    wattron(finestraGioco, COLOR_PAIR(colore));
    mvwprintw(finestraGioco, proiettile.y, proiettile.x, "%c", spriteProiettile);
    wattroff(finestraGioco, COLOR_PAIR(colore));
}