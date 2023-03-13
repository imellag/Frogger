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
    init_pair(OTTO, COLOR_RED, COLOR_BLACK);
    wattron(finestraGioco, COLOR_PAIR(OTTO));
    for (int i = ZERO; i < vite; i++)
    {
        mvwaddstr(finestraGioco, vita.y, vita.x, "\u2665");

        vita.x -= TRE; // lascio 2 di spazio tra le varie vite
    }
    wattroff(finestraGioco, COLOR_PAIR(OTTO));
}

void gameOver(WINDOW *finestraGioco)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(10));

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 5) + i, LARGHEZZA_SCHERMO / 2 - 32, "%ls", spriteSconfitta[i]);
    wattroff(finestraGioco, COLOR_PAIR(10));

    wrefresh(finestraGioco);
    sleep(3);
}

void vittoria(WINDOW *finestraGioco, int punteggio)
{

    int i;

    wattron(finestraGioco, COLOR_PAIR(10) | A_BOLD);

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 31, "%ls", spriteVittoria[i]);

    mvwprintw(finestraGioco, 16, LARGHEZZA_SCHERMO / DUE - 10, "Punteggio finale: %d!", punteggio);

    wattroff(finestraGioco, COLOR_PAIR(10) | A_BOLD);

    wrefresh(finestraGioco);
    sleep(5);
}

Oggetto morteRana(WINDOW *finestraGioco, int *vite, int pRana[], Oggetto ranocchio, int difficolta, int *tempo)
{

    Oggetto posizioneRana;
    (*tempo) = TEMPO_INIZIALE - (difficolta * 10);
    (*vite)--;
    posizioneRana = posizioneInizialeRana(pRana, ranocchio, difficolta);
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
            wattron(finestraGioco, COLOR_PAIR(1));
            mvwprintw(finestraGioco, DUE, ZERO + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(1));
        }
        else
        {
            wattron(finestraGioco, COLOR_PAIR(11));
            mvwprintw(finestraGioco, DUE, ZERO + i, " ");
            wattroff(finestraGioco, COLOR_PAIR(11));
        }
    }
}

void creaProiettile(int p[], Oggetto ranocchio, int *offset)
{
    pid_t pidProiettile;

    (*offset)++;
    system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
    pidProiettile = fork();
    if (pidProiettile < ZERO)
        perror("error");

    else if (pidProiettile == ZERO)
    {
        funzProiettile(ranocchio, p, (*offset) % NUMERO_PROIETTILI);
        exit(0);
    }
}

bool funzPausa(WINDOW *finestraGioco, int difficolta, Oggetto camion[], Oggetto tronchi[], Oggetto macchine[], pid_t pidTempo, pid_t pidRana)
{
    int i;

    bool scelta;

    int segnaleInviato;
    bool partitaFinita=false;

    for (i = 0; i < NUMERO_MACCHINE + (3 * difficolta); i++)
        kill(macchine[i].pid, SIGSTOP);

    for (i = 0; i < NUMERO_CAMION + (3 * difficolta); i++)
        kill(camion[i].pid, SIGSTOP);

    for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
        kill(tronchi[i].pid, SIGSTOP);

    kill(pidTempo, SIGSTOP);

    kill(pidRana, SIGSTOP);

    scelta = pausaeNuovaPartita(finestraGioco, UNO);

    if (scelta)
        segnaleInviato = SIGCONT;
    else{
        segnaleInviato = SIGKILL;
        partitaFinita=true;
        }

    for (i = 0; i < NUMERO_MACCHINE + (3 * difficolta); i++)
        kill(macchine[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_CAMION + (3 * difficolta); i++)
        kill(camion[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
        kill(tronchi[i].pid, segnaleInviato);

    kill(pidTempo, segnaleInviato);

    kill(pidRana, segnaleInviato);

    return partitaFinita;
}

bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata)
{
    int i, input = 0;

    MEVENT event;

    /* Chiamata == 1: pausa
       Chiamata == 2: nuova partita */

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    wattron(finestraGioco, COLOR_PAIR(10));

    for (i = ZERO; i < DUE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 29, "%ls", continua[i]);

    wattroff(finestraGioco, COLOR_PAIR(10));
    wattron(finestraGioco, COLOR_PAIR(4));

    for (i = ZERO; i < DUE; i++)
        stampaRettangolo(finestraGioco, 31 + (LARGHEZZA_RETTANGOLO * i + 6 * i), 21);

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

    wattroff(finestraGioco, COLOR_PAIR(4));
    wrefresh(finestraGioco);
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
                        return true;

                    else if (event.x >= 73 + INIZIO_LARGHEZZA_FINESTRA && event.x <= 109 + INIZIO_LARGHEZZA_FINESTRA &&
                             event.y >= 21 + INIZIO_ALTEZZA_FINESTRA && event.y <= 26 + INIZIO_ALTEZZA_FINESTRA)
                        return false;
                }
            }
        }
    }
}

void schermataFinale(WINDOW *finestraGioco)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(10));

    for (i = ZERO; i < DUE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 1) + i, LARGHEZZA_SCHERMO / 2 - 41, "%ls", scrittaFinale[i]);

    wattroff(finestraGioco, COLOR_PAIR(10));

    wrefresh(finestraGioco);
    sleep(2);
}

Oggetto uccidiProiettile(Oggetto proiettile)
{

    kill(proiettile.pid, SIGKILL);
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

Oggetto controlloCollisioneVeicoliProiettile(int i, Oggetto proiettilino[], Oggetto proiettileNemico[], Oggetto macchinina[], bool hitProiettile[])
{
    int j;
    for (j = 0; j < NUMERO_PROIETTILI; j++)
    {
        if (proiettiliVeicoli(proiettilino[j], proiettileNemico, macchinina[i], LARGHEZZA_MACCHINA, hitProiettile))
            proiettilino[j] = uccidiProiettile(proiettilino[j]);
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

bool finePartita(WINDOW *finestraGioco, Oggetto ranocchio, int vite, bool buffer, int punteggio,
                 int difficolta, Oggetto tempo, Oggetto macchina[], Oggetto camion[], Oggetto tronco[], bool *partitaInCorso,bool partitaFinita)
{
    int i;
    bool riniziaPartita;
    if (ranocchio.id == q || vite == 0 || buffer == false || partitaFinita)
    {
        (*partitaInCorso) = false;
        wclear(finestraGioco);

        if (vite == ZERO || partitaFinita)
            gameOver(finestraGioco);

        else if (buffer == false)
            vittoria(finestraGioco, punteggio);

        riniziaPartita = pausaeNuovaPartita(finestraGioco, DUE);

        if (!riniziaPartita)
        {
            wclear(finestraGioco);
            schermataFinale(finestraGioco);
        }

        endwin();

        for (i = ZERO; i < NUMERO_MACCHINE + (difficolta * 3); i++)
            kill(macchina[i].pid, SIGKILL);

        for (i = ZERO; i < NUMERO_CAMION + (difficolta * 3); i++)
            kill(camion[i].pid, SIGKILL);

        for (i = ZERO; i < NUMERO_TRONCHI; i++)
            kill(tronco[i].pid, SIGKILL);

        kill(ranocchio.pid, SIGKILL);
        kill(tempo.pid, SIGKILL);
    }

    return riniziaPartita;
}