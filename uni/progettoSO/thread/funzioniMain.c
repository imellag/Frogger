#include "funzioniMain.h"
#include "collisioni.h"
wchar_t *spriteSconfitta[ALTEZZA_SPRITE] = {
    L"██╗░░██╗░█████╗░██╗  ██████╗░███████╗██████╗░░██████╗░█████╗░██╗",
    L"██║░░██║██╔══██╗██║  ██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗██║",
    L"███████║███████║██║  ██████╔╝█████╗░░██████╔╝╚█████╗░██║░░██║██║",
    L"██╔══██║██╔══██║██║  ██╔═══╝░██╔══╝░░██╔══██╗░╚═══██╗██║░░██║╚═╝",
    L"██║░░██║██║░░██║██║  ██║░░░░░███████╗██║░░██║██████╔╝╚█████╔╝██╗",
    L"╚═╝░░╚═╝╚═╝░░╚═╝╚═╝  ╚═╝░░░░░╚══════╝╚═╝░░╚═╝╚═════╝░░╚════╝░╚═╝",
};

wchar_t *spriteVittoria[ALTEZZA_SPRITE] = {
    L"██╗░░██╗░█████╗░██╗  ██╗░░░██╗██╗███╗░░██╗████████╗░█████╗░██╗",
    L"██║░░██║██╔══██╗██║  ██║░░░██║██║████╗░██║╚══██╔══╝██╔══██╗██║",
    L"███████║███████║██║  ╚██╗░██╔╝██║██╔██╗██║░░░██║░░░██║░░██║██║",
    L"██╔══██║██╔══██║██║  ░╚████╔╝░██║██║╚████║░░░██║░░░██║░░██║╚═╝",
    L"██║░░██║██║░░██║██║  ░░╚██╔╝░░██║██║░╚███║░░░██║░░░╚█████╔╝██╗",
    L"╚═╝░░╚═╝╚═╝░░╚═╝╚═╝  ░░░╚═╝░░░╚═╝╚═╝░░╚══╝░░░╚═╝░░░░╚════╝░╚═╝",
};

wchar_t *continua[2] = {
    L"█░█ █░█ █▀█ █   █▀▀ █▀█ █▄░█ ▀█▀ █ █▄░█ █░█ ▄▀█ █▀█ █▀▀ ▀█",
    L"▀▄▀ █▄█ █▄█ █   █▄▄ █▄█ █░▀█ ░█░ █ █░▀█ █▄█ █▀█ █▀▄ ██▄ ░▄",
};

wchar_t *scrittaFinale[2] = {
    L"█▀▀ █▀█ ▄▀█ ▀█ █ █▀▀   █▀█ █▀▀ █▀█   ▄▀█ █░█ █▀▀ █▀█   █▀▀ █ █▀█ █▀▀ ▄▀█ ▀█▀ █▀█ █",
    L"█▄█ █▀▄ █▀█ █▄ █ ██▄   █▀▀ ██▄ █▀▄   █▀█ ▀▄▀ ██▄ █▀▄   █▄█ █ █▄█ █▄▄ █▀█ ░█░ █▄█ ▄",
};

void dimensioneFinestra(int maxx, int maxy)
{
    clear();
    while (maxy < ALTEZZA_SCHERMO + ALTEZZA_CORSIE*2 +15 || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / 2, maxx / 2 - 17, "Ingrandisci lo schermo per giocare! Premere Ctrl -"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2, LARGHEZZA_SCHERMO / 2 - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2 + 1, LARGHEZZA_SCHERMO / 2 - 7, "Buona fortuna!");
    refresh();
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
    sleep(3);
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
    wclear(finestraGioco);
    return posizioneRana;
}

void stampaTempo(WINDOW *finestraGioco, int tempo)
{
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

void creaProiettile(Oggetto rana, int *offset)
{
    pid_t pidProiettile;

    pidProiettile = fork();
    if (pidProiettile < 0)
        perror("error");

    else if (pidProiettile == 0)
    {
        //  funzProiettile();
        exit(0);
    }
}

bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata)
{
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

    if (chiamata == 1)
    {
        mvwprintw(finestraGioco, 23, 48, "Si");
        mvwprintw(finestraGioco, 23, 90, "No");
    }
    else if (chiamata == 2)
    {
        mvwprintw(finestraGioco, 23, 43, "Nuova partita");
        mvwprintw(finestraGioco, 23, 84, "Esci dal gioco");
    }

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
                 int difficolta, bool *partitaInCorso, bool partitaFinita, pthread_t threadRana, pthread_t threadProiettile[],
                 pthread_t threadTronchi[], pthread_t threadMacchine[],
                  pthread_t threadCamion[], pthread_t threadTempo, pthread_t threadCambioCorsia)
{
    int i;
    bool riniziaPartita;
    if (rana.id == q || vite == 0 || buffer == false || partitaFinita)
    {
        (*partitaInCorso) = false;
        wclear(finestraGioco);

        if (vite == 0 || partitaFinita)
            gameOver(finestraGioco, punteggio);

        else if (buffer == false)
            vittoria(finestraGioco, punteggio);

        wclear(finestraGioco);
        wrefresh(finestraGioco);

        riniziaPartita = pausaeNuovaPartita(finestraGioco, 2);

        if (!riniziaPartita)
        {
            wclear(finestraGioco);
            wrefresh(finestraGioco);
            schermataFinale(finestraGioco);
        }

        for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
            pthread_cancel(threadTronchi[i]);

        for (i = 0; i < NUMERO_CAMION; i++)
            pthread_cancel(threadCamion[i]);

        for (i = 0; i < NUMERO_MACCHINE; i++)
            pthread_cancel(threadMacchine[i]);


        pthread_cancel(threadTempo);
        pthread_cancel(threadRana);
         pthread_cancel(threadCambioCorsia);
         system("killall ffplay");
    }

    return riniziaPartita;
}

bool CorsiaOccupata(parametriVeicolo macchina[], parametriVeicolo camion[], int corsia, int difficolta)
{
    bool flag = false;
    int i;

    for (i = 0; i < NUMERO_MACCHINE + difficolta; i++)
    {
        if (macchina[i].veicolo.velocita < 0)
        {
            if (macchina[i].veicolo.coordinate.x >= LARGHEZZA_SCHERMO && macchina[i].veicolo.coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
            {
                flag = true;
                break;
            }
        }
        else
        {
            if (macchina[i].veicolo.coordinate.x <= 0 && macchina[i].veicolo.coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
            {
                flag = true;
                break;
            }
        }
    }
    if (!flag)
    {
        for (i = 0; i < NUMERO_CAMION + difficolta; i++)
        {
            if (camion[i].veicolo.velocita < 0)
            {
                if (camion[i].veicolo.coordinate.x >= LARGHEZZA_SCHERMO && camion[i].veicolo.coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
                {
                    flag = true;
                    break;
                }
            }
            else
            {
                if (camion[i].veicolo.coordinate.x <= 0 && camion[i].veicolo.coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
                {
                    flag = true;
                    break;
                }
            }
        }
    }

    return flag;
}

void creaColoriRandom(int difficolta)
{
    int i;
    Colore bufferColori;
    for (i = 0; i < NUMERO_MACCHINE + difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_MACCHINA0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
    for (i = 0; i < NUMERO_CAMION + difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_CAMION0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
}

void inizializzaArray(Oggetto tronco[], Oggetto proiettileNemico[], Oggetto proiettilinoNemico[], parametriVeicolo camion[],
                      parametriVeicolo macchina[], Proiettile proiettilino[], Proiettile proiettile[])
{
    int i;

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
}
