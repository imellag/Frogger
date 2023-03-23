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

wchar_t *continua[DUE] = {
    L"█░█ █░█ █▀█ █   █▀▀ █▀█ █▄░█ ▀█▀ █ █▄░█ █░█ ▄▀█ █▀█ █▀▀ ▀█",
    L"▀▄▀ █▄█ █▄█ █   █▄▄ █▄█ █░▀█ ░█░ █ █░▀█ █▄█ █▀█ █▀▄ ██▄ ░▄",
};

wchar_t *scrittaFinale[DUE] = {
    L"█▀▀ █▀█ ▄▀█ ▀█ █ █▀▀   █▀█ █▀▀ █▀█   ▄▀█ █░█ █▀▀ █▀█   █▀▀ █ █▀█ █▀▀ ▄▀█ ▀█▀ █▀█ █",
    L"█▄█ █▀▄ █▀█ █▄ █ ██▄   █▀▀ ██▄ █▀▄   █▀█ ▀▄▀ ██▄ █▀▄   █▄█ █ █▄█ █▄▄ █▀█ ░█░ █▄█ ▄",
};

void dimensioneFinestra(int maxx, int maxy)
{
    clear();
    while (maxy < ALTEZZA_SCHERMO || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / DUE, maxx / DUE - 17, "Ingrandisci lo schermo per giocare!"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / DUE, LARGHEZZA_SCHERMO / DUE - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / DUE + UNO, LARGHEZZA_SCHERMO / DUE - SETTE, "Buona fortuna!");
    refresh();
    clear();
    refresh();
}

void stampaVite(WINDOW *finestraGioco, int vite)
{
    Coordinate vita;
    vita.x = LARGHEZZA_SCHERMO - TRE;
    vita.y = DUE;
    /* inizio a stamparle da in alto a destra, poi mi sposto
    verso sinistra */
    wattron(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
    for (int i = ZERO; i < vite; i++)
    {
        mvwaddstr(finestraGioco, vita.y, vita.x, "\u2665");

        vita.x -= TRE; // lascio 2 di spazio tra le varie vite
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_SCRITTE_INFO));
}

void gameOver(WINDOW *finestraGioco, int punteggio)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 5) + i, LARGHEZZA_SCHERMO / 2 - 32, "%ls", spriteSconfitta[i]);

    mvwprintw(finestraGioco, 20, LARGHEZZA_SCHERMO / DUE - 12, "Punteggio finale: %d!", punteggio);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wrefresh(finestraGioco);
    sleep(3);
}

void vittoria(WINDOW *finestraGioco, int punteggio)
{

    int i;

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO | A_BOLD));

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 31, "%ls", spriteVittoria[i]);

    mvwprintw(finestraGioco, 16, LARGHEZZA_SCHERMO / DUE - 12, "Punteggio finale: %d!", punteggio);

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
            mvwprintw(finestraGioco, DUE, ZERO + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
        }
        else
        {
            wattron(finestraGioco, COLOR_PAIR(COLORE_NERO));
            mvwprintw(finestraGioco, DUE, ZERO + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NERO));
        }
    }
}

void creaProiettile(Oggetto rana, int *offset)
{
    pid_t pidProiettile;

    pidProiettile = fork();
    if (pidProiettile < ZERO)
        perror("error");

    else if (pidProiettile == ZERO)
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

    for (i = ZERO; i < DUE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 29, "%ls", continua[i]);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wattron(finestraGioco, COLOR_PAIR(COLORE_NERO_VERDE));

    for (i = ZERO; i < DUE; i++)
        stampaRettangolo(finestraGioco, 21, 31 + (LARGHEZZA_RETTANGOLO * i + 6 * i));

    if (chiamata == UNO)
    {
        mvwprintw(finestraGioco, 23, 48, "Si");
        mvwprintw(finestraGioco, 23, 90, "No");
    }
    else if (chiamata == DUE)
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
                        return false;
                    }

                    else if (event.x >= 73 + INIZIO_LARGHEZZA_FINESTRA && event.x <= 109 + INIZIO_LARGHEZZA_FINESTRA &&
                             event.y >= 21 + INIZIO_ALTEZZA_FINESTRA && event.y <= 26 + INIZIO_ALTEZZA_FINESTRA)
                    {
                        pthread_mutex_unlock(&mutex);
                        return true;
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

    for (i = ZERO; i < DUE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 1) + i, LARGHEZZA_SCHERMO / 2 - 41, "%ls", scrittaFinale[i]);

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

time_t spawnNemico(time_t fine_nemico, time_t inizio_nemico, int difficolta, bool sulTronco,
                   bool nemico[], Oggetto tronchino[], time_t inizio_proiettile[], Oggetto rana)
{

    int contaNemici = 0;
    int i;
    int troncoNemico = 0;
}

Oggetto controlloCollisioneVeicoliProiettile(int i, Oggetto proiettilino[], Oggetto proiettileNemico[], Oggetto macchinina[], bool hitProiettile[], pthread_t threadProiettile, pthread_t threadProiettileNemico[])
{
    int j;
    for (j = 0; j < NUMERO_PROIETTILI; j++)
    {
        if (proiettiliVeicoli(proiettilino[j], proiettileNemico, macchinina[i], LARGHEZZA_MACCHINA, hitProiettile, threadProiettileNemico))
            proiettilino[j] = uccidiProiettile(proiettilino[j], threadProiettile);
    }

    return proiettilino[j];
}

bool controlloTaneChiuse(bool arrayTane[])
{
    int i;
    bool buffer = false;

    for (i = ZERO; i < NUMERO_TANE; i++)
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
                 int difficolta, bool *partitaInCorso, bool partitaFinita)
{
    int i;
    bool riniziaPartita;
    if (rana.id == q || vite == 0 || buffer == false || partitaFinita)
    {
        (*partitaInCorso) = false;
        wclear(finestraGioco);

        if (vite == ZERO || partitaFinita)
            gameOver(finestraGioco, punteggio);

        else if (buffer == false)
            vittoria(finestraGioco, punteggio);

        wclear(finestraGioco);
        wrefresh(finestraGioco);

        riniziaPartita = pausaeNuovaPartita(finestraGioco, DUE);

        if (!riniziaPartita)
        {
            wclear(finestraGioco);
            wrefresh(finestraGioco);
            schermataFinale(finestraGioco);
        }

        endwin();
    }

    return riniziaPartita;
}

bool CorsiaOccupata(parametriVeicolo macchina[], parametriVeicolo camion[], int corsia, int difficolta)
{
    bool flag = false;
    int i;

    for (i = 0; i < NUMERO_MACCHINE + difficolta; i++)
    {
        if (macchina[i].veicolo.velocita < ZERO)
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
            if (camion[i].veicolo.velocita < ZERO)
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
    for (i = ZERO; i < NUMERO_MACCHINE + difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_MACCHINA0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
    for (i = ZERO; i < NUMERO_CAMION + difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_CAMION0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
}
