#include "funzioniMain.h"

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
    /* inizio a stamparle da in basso a destra, poi mi sposto
    verso sinistra */
    init_pair(OTTO, COLOR_RED, COLOR_BLACK);
    wattron(finestraGioco, COLOR_PAIR(OTTO));
    for (int i = ZERO; i < vite; i++)
    {
        mvwaddstr(finestraGioco, vita.y, vita.x, "\u2665");

        vita.x -= TRE; // lascio 1 di spazio tra le varie vite
    }
    wattroff(finestraGioco, COLOR_PAIR(OTTO));
}

void gameOver(WINDOW *finestraGioco)
{
    int i;

    wattron(finestraGioco, COLOR_PAIR(10));

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 35, "%ls", spriteSconfitta[i]);
    wattroff(finestraGioco, COLOR_PAIR(10));

    wrefresh(finestraGioco);
    sleep(3);
}

void vittoria(WINDOW *finestraGioco, int punteggio)
{

    int i;

    wattron(finestraGioco, COLOR_PAIR(10) | A_BOLD);

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 45, "%ls", spriteVittoria[i]);

    mvwprintw(finestraGioco, 16, LARGHEZZA_SCHERMO / DUE - 25, "Punteggio finale: %d!", punteggio);

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

void funzPausa(WINDOW *finestraGioco, int difficolta, Oggetto camion[], Oggetto tronchi[], Oggetto macchine[], pid_t pidTempo, pid_t pidRana)
{
    int i;

    bool scelta;

    int segnaleInviato;

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
    else
        segnaleInviato = SIGKILL;

    for (i = 0; i < NUMERO_MACCHINE + (3 * difficolta); i++)
        kill(macchine[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_CAMION + (3 * difficolta); i++)
        kill(camion[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
        kill(tronchi[i].pid, segnaleInviato);

    kill(pidTempo, segnaleInviato);

    kill(pidRana, segnaleInviato);
}



bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata)
{
    int i, input=0;

       MEVENT event;

    /* Chiamata == 1: pausa
       Chiamata == 2: nuova partita */

    werase(finestraGioco);
    wrefresh(finestraGioco);

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    wattron(finestraGioco, COLOR_PAIR(10));

    for (i = ZERO; i < DUE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / DUE - 10) + i, LARGHEZZA_SCHERMO / 2 - 29, "%ls", continua[i]);

    wattroff(finestraGioco, COLOR_PAIR(10));
    wattron(finestraGioco, COLOR_PAIR(4));

    for (i = ZERO; i < DUE; i++)
    {
        stampaRettangolo(finestraGioco, 31 + (36 * i) + 6, 20);
    }

    if (chiamata == UNO)
    {
        mvwprintw(finestraGioco, 22, 48, "Si");
        mvwprintw(finestraGioco, 22, 90, "No");
    }
    else if (chiamata == DUE)
    {
        mvwprintw(finestraGioco, 22, 43, "Nuova partita");
        mvwprintw(finestraGioco, 22, 84, "Esci dal gioco");
    }

    wattroff(finestraGioco, COLOR_PAIR(4));

    while (true)
    {
        input = wgetch(finestraGioco);

        mvwprintw(finestraGioco,0,0,"%d",input);
        wrefresh(finestraGioco);
        if (input == KEY_MOUSE)
        {
         
            if (getmouse(&event) == OK)
            {
                if (event.bstate && BUTTON1_PRESSED)
                {
                    wrefresh(finestraGioco);
                    if (event.x >= 31 && event.x <= 67 && event.y >= 20 && event.y <= 25)
                        return true;
                    else if (event.x >= 73 && event.x <= 109 && event.y >= 20 && event.y <= 25)
                        return false;
                }
            }
        }
    }
}

Oggetto uccidiProiettile(Oggetto proiettile)
{

    kill(proiettile.pid, SIGKILL);
    proiettile.coordinate.x = FUORI_MAPPA;
    proiettile.coordinate.y = FUORI_MAPPA;

    return proiettile;
}