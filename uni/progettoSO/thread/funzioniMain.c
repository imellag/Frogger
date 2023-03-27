#include "funzioniMain.h"
#include "collisioni.h"
wchar_t *spriteSconfitta[ALTEZZA_SPRITE] = {
    L"██╗░░██╗░█████╗░██╗  ██████╗░███████╗██████╗░░██████╗░█████╗░██╗",
    L"██║░░██║██╔══██╗██║  ██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗██║",
    L"███████║███████║██║  ██████╔╝█████╗░░██████╔╝╚█████╗░██║░░██║██║",
    L"██╔══██║██╔══██║██║  ██╔═══╝░██╔══╝░░██╔══██╗░╚═══██╗██║░░██║╚═╝",
    L"██║░░██║██║░░██║██║  ██║░░░░░███████╗██║░░██║██████╔╝╚█████╔╝██╗",
    L"╚═╝░░╚═╝╚═╝░░╚═╝╚═╝  ╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═════╝░░╚════╝░╚═╝"};

wchar_t *spriteVittoria[ALTEZZA_SPRITE] = {
    L"██╗░░██╗░█████╗░██╗  ██╗░░░██╗██╗███╗░░██╗████████╗░█████╗░██╗",
    L"██║░░██║██╔══██╗██║  ██║░░░██║██║████╗░██║╚══██╔══╝██╔══██╗██║",
    L"███████║███████║██║  ╚██╗░██╔╝██║██╔██╗██║░░░██║░░░██║░░██║██║",
    L"██╔══██║██╔══██║██║  ░╚████╔╝░██║██║╚████║░░░██║░░░██║░░██║╚═╝",
    L"██║░░██║██║░░██║██║  ░░╚██╔╝░░██║██║░╚███║░░░██║░░░╚█████╔╝██╗",
    L"╚═╝░░╚═╝╚═╝░░╚═╝╚═╝  ░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░░╚════╝░╚═╝"};

wchar_t *continua[2] = {
    L"█░█ █░█ █▀█ █   █▀▀ █▀█ █▄░█ ▀█▀ █ █▄░█ █░█ ▄▀█ █▀█ █▀▀ ▀█",
    L"▀▄▀ █▄█ █▄█ █   █▄▄ █▄█ █░▀█ ░█░ █ █░▀█ █▄█ █▀█ █▀▄ ██▄ ░▄"};

wchar_t *scrittaFinale[2] = {
    L"█▀▀ █▀█ ▄▀█ ▀█ █ █▀▀   █▀█ █▀▀ █▀█   ▄▀█ █░█ █▀▀ █▀█   █▀▀ █ █▀█ █▀▀ ▄▀█ ▀█▀ █▀█ █",
    L"█▄█ █▀▄ █▀█ █▄ █ ██▄   █▀▀ ██▄ █▀▄   █▀█ ▀▄▀ ██▄ █▀▄   █▄█ █ █▄█ █▄▄ █▀█ ░█░ █▄█ ▄"};

void dimensioneFinestra(int maxx, int maxy)
{
    clear();
    // la dimensione necessaria è pari alla dimensione occupata dalle corsie in difficoltà massima,
    // dove sono presenti 5 corsie di autostrada e 5 corsie di fiume
    while (maxy < ALTEZZA_SCHERMO + ALTEZZA_CORSIE * 2 + 15 || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / 2, maxx / 2 - 25, "Ingrandisci lo schermo per giocare! Premere Ctrl -"); // -25 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2, LARGHEZZA_SCHERMO / 2 - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2 + 1, LARGHEZZA_SCHERMO / 2 - 7, "Buona fortuna!");
    refresh();
    sleep(2);
    clear();
    refresh();
}

void stampaVite(WINDOW *finestraGioco, int vite)
{
    Coordinate vita;
    vita.x = LARGHEZZA_SCHERMO - 3;
    vita.y = 2;
    /* inizio a stamparle da in alto a destra, poi mi sposto
    verso sinistra */
    wattron(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
    for (int i = 0; i < vite; i++)
    {
        mvwaddstr(finestraGioco, vita.y, vita.x, "\u2665");

        vita.x -= 3; // lascio 2 di spazio tra le varie vite
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
}

void gameOver(WINDOW *finestraGioco, int punteggio)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = 0; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 5) + i, LARGHEZZA_SCHERMO / 2 - 32, "%ls", spriteSconfitta[i]);

    mvwprintw(finestraGioco, 20, LARGHEZZA_SCHERMO / 2 - 12, "Punteggio finale: %d!", punteggio);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wrefresh(finestraGioco);
    sleep(5);
}

void vittoria(WINDOW *finestraGioco, int punteggio)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO | A_BOLD));

    for (i = 0; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 10) + i, LARGHEZZA_SCHERMO / 2 - 31, "%ls", spriteVittoria[i]);

    mvwprintw(finestraGioco, 16, LARGHEZZA_SCHERMO / 2 - 12, "Punteggio finale: %d!", punteggio);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO) | A_BOLD);

    wrefresh(finestraGioco);
    sleep(5);
}

Oggetto morteRana(WINDOW *finestraGioco, int *vite, Oggetto rana, int difficolta, int *tempo)
{
    Oggetto posizioneRana;
    (*tempo) = TEMPO_INIZIALE - (difficolta * 10);
    (*vite)--;
    posizioneRana = posizioneInizialeRana(rana, difficolta);
    werase(finestraGioco);
    return posizioneRana;
}

void stampaTempo(WINDOW *finestraGioco, int tempo)
{
    /* i secondi rimanenti vengono stampati rossi, mentre i secondi già passati vengono stampati neri.
     questo perché il tempo viene stampato in alto nello schermo, fuori dall'area di gioco, e fare una clear
     creerebbe problemi per gli altri oggetti stampati in questa parte dello schermo, come il punteggio e le vite */
    int i;
    for (i = 0; i < TEMPO_INIZIALE; i++)
    {
        if (i <= tempo)
        {
            wattron(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
            mvwprintw(finestraGioco, 2, 0 + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
        }
        else
        {
            wattron(finestraGioco, COLOR_PAIR(COLORE_NERO));
            mvwprintw(finestraGioco, 2, 0 + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NERO));
        }
    }
}

bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata)
{
#define ALTEZZA_SCRITTE 23
#define PAUSA 1
#define NUOVA_PARTITA 2

    int i, j, input = 0;

    MEVENT event;

    /* Chiamata == 1: pausa
       Chiamata == 2: nuova partita */

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = 0; i < 2; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 10) + i, LARGHEZZA_SCHERMO / 2 - 29, "%ls", continua[i]);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wattron(finestraGioco, COLOR_PAIR(COLORE_NERO_VERDE));

    for (i = 0; i < 2; i++)
        stampaRettangolo(finestraGioco, 21, 31 + (LARGHEZZA_RETTANGOLO * i + 6 * i));

    if (chiamata == PAUSA)
    {
        mvwprintw(finestraGioco, ALTEZZA_SCRITTE, 48, "Si");
        mvwprintw(finestraGioco, ALTEZZA_SCRITTE, 90, "No");
    }
    else if (chiamata == NUOVA_PARTITA)
    {
        mvwprintw(finestraGioco, ALTEZZA_SCRITTE, 43, "Nuova partita");
        mvwprintw(finestraGioco, ALTEZZA_SCRITTE, 84, "Esci dal gioco");
    }
    
    /* in base all'input del mouse la funzione restituisce true o false, che viene poi gestito dalla funzione chiamante
     in base al tipo di chiamata (pausa o nuova partita) */

    wattroff(finestraGioco, COLOR_PAIR(COLORE_NERO_VERDE));
    wrefresh(finestraGioco);
    pthread_mutex_lock(&mutex);
    while (true)
    {
        input = getch();

        wrefresh(finestraGioco);

        if (input == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_PRESSED)
                {
                    if (event.x >= 31 + INIZIO_LARGHEZZA_FINESTRA && event.x <= 67 + INIZIO_LARGHEZZA_FINESTRA &&
                        event.y >= 21 + INIZIO_ALTEZZA_FINESTRA && event.y <= 26 + INIZIO_ALTEZZA_FINESTRA)
                    {
                        pthread_mutex_unlock(&mutex);
                        return true;
                    }

                    else if (event.x >= 73 + INIZIO_LARGHEZZA_FINESTRA && event.x <= 109 + INIZIO_LARGHEZZA_FINESTRA &&
                             event.y >= 21 + INIZIO_ALTEZZA_FINESTRA && event.y <= 26 + INIZIO_ALTEZZA_FINESTRA)
                    {
                        pthread_mutex_unlock(&mutex);
                        return false;
                    }
                }
            }
        }
    }
}

void schermataFinale(WINDOW *finestraGioco)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = 0; i < 2; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 1) + i, LARGHEZZA_SCHERMO / 2 - 41, "%ls", scrittaFinale[i]);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wrefresh(finestraGioco);
    sleep(2);
}

Oggetto uccidiProiettile(Oggetto proiettile, pthread_t threadProiettile)
{

    pthread_cancel(threadProiettile);
    proiettile.coordinate.x = FUORI_MAPPA;
    proiettile.coordinate.y = FUORI_MAPPA;

    return proiettile;
}

bool controlloTaneChiuse(bool arrayTane[])
{
    int i;
    bool buffer = false;

    for (i = 0; i < NUMERO_TANE; i++)
    {
        if (arrayTane[i] == false)
        {
            buffer = true;
            break;
        }
    }

    return buffer;
}

bool finePartita(WINDOW *finestraGioco, Oggetto rana, int vite, bool buffer, int punteggio,
                 int difficolta, bool partitaFinita)
{  
    int i;
    bool partitaInCorso = true;
    if (rana.id == q || vite == 0 || buffer == false || partitaFinita)
    {
        partitaInCorso = false;
        werase(finestraGioco);

        if (vite == 0 || partitaFinita)
            gameOver(finestraGioco, punteggio);

        else if (buffer == false)
            vittoria(finestraGioco, punteggio);

        werase(finestraGioco);
        wrefresh(finestraGioco);

        
    }

    return partitaInCorso;
}

void creaColoriRandom(int difficolta)
{
    // ogni colore viene assegnato all'id del veicolo
    int i;
    Colore bufferColori;
    for (i = 0; i < NUMERO_MACCHINE; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_MACCHINA0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
    for (i = 0; i < NUMERO_CAMION; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_CAMION0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
}

void inizializzaArray(Oggetto tronco[], Oggetto proiettileNemico[], Oggetto proiettilinoNemico[], parametriVeicolo camion[],
                      parametriVeicolo macchina[], Proiettile proiettilino[], Proiettile proiettile[])
{
    int i;

    /* i proiettili non vengono messi alle stesse coordinate degli altri oggetti per evitare che si generino delle collisioni 
     quando non sono rappresentati a schermo */
    for (i = 0; i < MAX_TRONCHI; i++)
    {
        tronco[i].coordinate.x = FUORI_MAPPA;
        tronco[i].coordinate.y = FUORI_MAPPA;

        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;

        proiettilinoNemico[i].coordinate.x = FUORI_MAPPA - 2;
        proiettilinoNemico[i].coordinate.y = FUORI_MAPPA - 2;
    }

    for (i = 0; i < MAX_CAMION; i++)
    {

        camion[i].veicolo.coordinate.x = FUORI_MAPPA;
        camion[i].veicolo.coordinate.y = FUORI_MAPPA;
    }

    for (i = 0; i < MAX_MACCHINE; i++)
    {
        macchina[i].veicolo.coordinate.x = FUORI_MAPPA;
        macchina[i].veicolo.coordinate.y = FUORI_MAPPA;
    }
    for (i = 0; i < NUMERO_PROIETTILI; i++)
    {
        proiettilino[i].proiettile.coordinate.x = FUORI_MAPPA - 1;
        proiettilino[i].proiettile.coordinate.y = FUORI_MAPPA - 1;

        proiettile[i].proiettile.coordinate.x = FUORI_MAPPA - 1;
        proiettile[i].proiettile.coordinate.y = FUORI_MAPPA - 1;
    }
}

void inizializzaVeicolo(parametriVeicolo veicolo, int difficolta, int direzioneCorsie[], int velocitaCorsie[])
{
    int corsia = rand() % (NUMERO_CORSIE + difficolta);

    veicolo.veicolo.coordinate.y = INIZIO_AUTOSTRADA + difficolta * 3 + corsia * 3;
    veicolo.direzioneCorsia = direzioneCorsie[corsia];
    veicolo.velocitaCorsia = velocitaCorsie[corsia];
    if (direzioneCorsie[corsia] < 0)
        veicolo.veicolo.coordinate.x = -LARGHEZZA_CAMION;
    else
        veicolo.veicolo.coordinate.x = LARGHEZZA_CAMION + LARGHEZZA_SCHERMO;
    veicolo.veicolo.difficolta = difficolta;
}

void *delayCambioCorsia(void *_flagCambioCorsia)
{
    bool *flagCambioCorsia = _flagCambioCorsia;

    while (true)
    {
        pthread_mutex_lock(&mutex);
        (*flagCambioCorsia) = true;
        pthread_mutex_unlock(&mutex);
        usleep(40000);
    }

    return NULL;
}
