#include "funzioniMain.h"
#include "collisioni.h"
#include "lib.h"
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

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = 0; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 10) + i, LARGHEZZA_SCHERMO / 2 - 31, "%ls", spriteVittoria[i]);

    mvwprintw(finestraGioco, 16, LARGHEZZA_SCHERMO / 2 - 12, "Punteggio finale: %d!", punteggio);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    wrefresh(finestraGioco);
    sleep(5);
}

Oggetto morteRana(WINDOW *finestraGioco, int *vite, int pRana[], Oggetto ranocchio, int difficolta, int *tempo)
{

    Oggetto posizioneRana;
    (*tempo) = TEMPO_INIZIALE - (difficolta * 10);
    (*vite)--;
    posizioneRana = posizioneInizialeRana(pRana, ranocchio, difficolta);
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
            mvwprintw(finestraGioco, 2, i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
        }
        else
        {
            wattron(finestraGioco, COLOR_PAIR(COLORE_NERO));
            mvwprintw(finestraGioco, 2, i, " ");
            wattroff(finestraGioco, COLOR_PAIR(COLORE_NERO));
        }
    }
}

void creaProiettile(int p[], Oggetto ranocchio, int *offset, bool audio)
{
    pid_t pidProiettile;

    (*offset)++;
    if (audio)
        system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
    pidProiettile = fork();
    if (pidProiettile < 0)
        perror("error");

    else if (pidProiettile == 0)
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
    bool partitaFinita = false;

    for (i = 0; i < NUMERO_MACCHINE; i++)
        kill(macchine[i].pid, SIGSTOP);

    for (i = 0; i < NUMERO_CAMION; i++)
        kill(camion[i].pid, SIGSTOP);

    for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
        kill(tronchi[i].pid, SIGSTOP);

    kill(pidTempo, SIGSTOP);

    kill(pidRana, SIGSTOP);

    scelta = pausaeNuovaPartita(finestraGioco, 1);

    if (scelta)
        segnaleInviato = SIGCONT;
    else
    {
        segnaleInviato = SIGKILL;
        partitaFinita = true;
    }

    for (i = 0; i < NUMERO_MACCHINE; i++)
        kill(macchine[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_CAMION; i++)
        kill(camion[i].pid, segnaleInviato);

    for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
        kill(tronchi[i].pid, segnaleInviato);

    kill(pidTempo, segnaleInviato);

    kill(pidRana, segnaleInviato);

    return partitaFinita;
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

    wattron(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

    for (i = 0; i < 2; i++)
        mvwprintw(finestraGioco, (ALTEZZA_SCHERMO / 2 - 1) + i, LARGHEZZA_SCHERMO / 2 - 41, "%ls", scrittaFinale[i]);

    wattroff(finestraGioco, COLOR_PAIR(COLORE_VERDE_NERO));

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

bool finePartita(WINDOW *finestraGioco, Oggetto ranocchio, int vite, bool buffer, int punteggio,
                 int difficolta, Oggetto tempo, Oggetto macchina[], Oggetto camion[], Oggetto tronco[], bool *partitaInCorso, bool partitaFinita)
{
    /* se anche solo una delle condizioni di fine della partita viene rispettata si entra nell'if, che poi stampa la scritta giusta in base a 
     quale di queste condizioni è vera. viene poi chiesto se si vuole fare una nuova partita e in base al valore restituito si prosegue. 
     i processi vengono killati indipendentemente da questo dato che vengono ricreati all'avvio della partita */
    int i;
    bool riniziaPartita;
    if (ranocchio.id == q || vite == 0 || buffer == false || partitaFinita)
    {
        (*partitaInCorso) = false;
        werase(finestraGioco);

        if (vite == 0 || partitaFinita)
            gameOver(finestraGioco, punteggio);

        else if (buffer == false)
            vittoria(finestraGioco, punteggio);

        werase(finestraGioco);
        wrefresh(finestraGioco);

        riniziaPartita = pausaeNuovaPartita(finestraGioco, 2);

        if (!riniziaPartita)
        {
            werase(finestraGioco);
            wrefresh(finestraGioco);
            schermataFinale(finestraGioco);
        }

        for (i = 0; i < NUMERO_MACCHINE; i++)
            kill(macchina[i].pid, SIGKILL);

        for (i = 0; i < NUMERO_CAMION; i++)
            kill(camion[i].pid, SIGKILL);

        for (i = 0; i < NUMERO_TRONCHI + difficolta; i++)
            kill(tronco[i].pid, SIGKILL);

        kill(tempo.pid, SIGKILL);

        kill(ranocchio.pid, SIGKILL);

        system("killall ffplay");
    }

    return riniziaPartita;
}

bool corsiaOccupata(Oggetto macchinina[], Oggetto camioncino[], int corsia, int difficolta)
{
    bool flag = false;
    int i;

    for (i = 0; i < NUMERO_MACCHINE + difficolta; i++)
    {
        if (camioncino[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
        {

            if (macchinina[i].velocita < 0)
            {
                if (macchinina[i].coordinate.x >= LARGHEZZA_SCHERMO)
                    return true;
            }
            else
            {
                if (macchinina[i].coordinate.x <= 0)
                    return true;
            }
        }
    }

    for (i = 0; i < NUMERO_CAMION + difficolta; i++)
    {
        if (camioncino[i].velocita < 0)
        {
            if (camioncino[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
            {
                if (camioncino[i].coordinate.x >= LARGHEZZA_SCHERMO)
                    return true;
            }
            else
            {
                if (camioncino[i].coordinate.x <= 0)
                    return true;
            }
        }
    }

    return false;
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

void inizializzaArray(Oggetto tronco[], Oggetto camion[], Oggetto macchina[], Oggetto proiettile[], Oggetto proiettileNemico[])
{
    /* i proiettili non vengono messi alle stesse coordinate degli altri oggetti per evitare che si generino delle collisioni 
     quando non sono rappresentati a schermo */
    int i;

    for (i = 0; i < MAX_TRONCHI; i++)
    {
        tronco[i].coordinate.x = FUORI_MAPPA;
        tronco[i].coordinate.y = FUORI_MAPPA;

        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
    }

    for (i = 0; i < NUMERO_CAMION; i++)
    {
        camion[i].coordinate.x = FUORI_MAPPA;
        camion[i].coordinate.y = FUORI_MAPPA;
    }

    for (i = 0; i < NUMERO_MACCHINE; i++)
    {
        macchina[i].coordinate.x = FUORI_MAPPA;
        macchina[i].coordinate.y = FUORI_MAPPA;
    }

    for (i = 0; i < NUMERO_PROIETTILI; i++)
    {
        proiettile[i].coordinate.x = FUORI_MAPPA - 1;
        proiettile[i].coordinate.y = FUORI_MAPPA - 1;
    }
}
