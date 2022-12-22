#include "lib.h"
#include "funzioniGenerali.h"
#include "rana.h"
#include "marciapiede.h"
#include "autostrada.h"
#include "prato.h"
#include "fiume.h"
#include "tane.h"

char spriteProiettile = '^';
char spriteCuore[] = {"<3"};

int main()
{
    srand(time(NULL));

    int maxx, maxy;
    int maxx_precedente, maxy_precedente;
    int tempo = 30, punteggio = ZERO, vite = TRE;
    Coordinate rana;

    rana.x = ZERO;
    rana.y = ALTEZZA_SCHERMO - SEI;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    colori();
    keypad(stdscr, true);
    getmaxyx(stdscr, maxy, maxx);

    int p[DUE];
    if (pipe(p) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }

    int pRana[DUE];
    if (pipe(pRana) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }
    fcntl(pRana[0], F_SETFL, fcntl(pRana[0], F_GETFL) | O_NONBLOCK);

    clear();
    refresh();

    dimensioneFinestra(maxx, maxy);

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(vite);

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(rana);
    mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
    mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
    refresh();

    pid_t pidRana, pidMacchine[3], pidTronchi[3], pidNemici, pidProiettile;
    pidRana = fork();

    if (pidRana < ZERO)
    {
        printw("Error");
        exit(-1);
    }
    else if (pidRana == ZERO)
    {
        funzRana(p, pRana);
    }
    else
    {
        funzAuto(p);
        funzTronchi(p);

        int i;
        Oggetto pacchetto;
        Oggetto proiettilino;
        Oggetto ranocchio;
        Coordinate nuoveCoordinate;
        ranocchio.coordinate.x = -4;
        ranocchio.coordinate.y = -4;

        Oggetto tronchino[TRE];

        for (i = 0; i < 3; i++)
        {
            tronchino[i].coordinate.x = -CINQUE;
            tronchino[i].coordinate.y = -CINQUE;
        }
        Oggetto macchinina[TRE];
        proiettilino.coordinate.x = -UNO;
        proiettilino.coordinate.y = -UNO;
        _Bool fuorischermo = false;

        int direzione;

        close(p[WRITE]);
        close(pRana[READ]);

        while (true)
        {

            read(p[READ], &pacchetto, sizeof(Oggetto));
            switch (pacchetto.id)
            {
            case RANA:
                ranocchio = pacchetto;

                break;

            case PROIETTILE:
                proiettilino = pacchetto;
                fuorischermo = false;
                break;
            case PROIETTILE_OUT:
                fuorischermo = true;
                break;
            case TRONCO0:
                tronchino[ZERO] = pacchetto;
                break;
            case TRONCO1:
                tronchino[UNO] = pacchetto;
                break;
            case TRONCO2:
                tronchino[DUE] = pacchetto;
                break;

            case MACCHINA0:
                macchinina[ZERO] = pacchetto;
                break;

            case MACCHINA1:
                macchinina[UNO] = pacchetto;
                break;

            case MACCHINA2:
                macchinina[DUE] = pacchetto;
                break;

            case q:
                ranocchio = pacchetto;
                break;

            default:
                break;
            }

            // se la dimensione della finestra cambiata allora pulisce lo schermo per evitare problemi
            maxx_precedente = maxx;
            maxy_precedente = maxy;
            getmaxyx(stdscr, maxy, maxx);
            if (maxx != maxx_precedente || maxy != maxy_precedente)
                clear();

          /*  nuoveCoordinate.x = controlloRanaTronco(ranocchio.coordinate, tronchino);
            if (!(nuoveCoordinate.x == -1))
            {
                ranocchio.coordinate.x = nuoveCoordinate.x;
                write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
            }
*/
            stampaVite(vite);

            funzMarciapiede();
            funzAutostrada();
            funzPrato();
            funzFiume();
            for (i = ZERO; i < TRE; i++)
            {
                stampaTronco(tronchino[i].coordinate);
                stampaMacchina(macchinina[i]);
                pidMacchine[i]=macchinina[i].pid;
                pidTronchi[i]=tronchino[i].pid;
            }

            stampaRana(ranocchio.coordinate);

            if (!fuorischermo)
                mvwprintw(stdscr, proiettilino.coordinate.y, proiettilino.coordinate.x, "%c", spriteProiettile);

            mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
            mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
            refresh();

            if (ranocchio.id == q)
            {
                for (i = 0; i < 3; i++)
                {
                    kill(pidMacchine[i], SIGKILL);
                    kill(pidTronchi[i], SIGKILL);
                }
                endwin();
                kill(pidRana, SIGKILL);

                return EXIT_SUCCESS;
            }
        }
    }
}

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

void stampaVite(int vite)
{
    Coordinate vita;
    vita.x = LARGHEZZA_SCHERMO - TRE;
    vita.y = ALTEZZA_SCHERMO - DUE;
    /* inizio a stamparle da in basso a destra, poi mi sposto
    verso sinistra */
    init_pair(OTTO, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(OTTO));
    for (int i = ZERO; i < vite; i++)
    {
        for (int j = ZERO; j < DUE; j++)
            mvprintw(vita.y, vita.x + j, "%c", spriteCuore[j]);

        vita.x -= TRE; // lascio 1 di spazio tra le varie vite
    }
    attroff(COLOR_PAIR(OTTO));
}

int controlloPosizione(Coordinate rana)
{

    if (rana.y == INIZIO_MARCIAPIEDE)
        return COLORE_MARCIAPIEDE;
    else if (rana.y >= INIZIO_AUTOSTRADA && rana.y < INIZIO_MARCIAPIEDE)
        return COLORE_AUTOSTRADA;
    else if (rana.y == INIZIO_PRATO)
        return COLOR_GREEN;
    else if (rana.y >= 8 && rana.y < INIZIO_PRATO)
        return COLOR_BLUE;
}

void menu()
{
    curs_set(true);
    mousemask(BUTTON1_PRESSED, NULL);

    attron(COLOR_PAIR(QUATTRO));

    /*
    8-13   nuova partita
    15-20  impostazioni
    22-27  esci
    */

    /*
    40 larghezza
    */

    attroff(COLOR_PAIR(QUATTRO));

    // printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

    int input = wgetch(stdscr);

    if (input == KEY_MOUSE)
    {

        MEVENT event;
        if (getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            { // click sinistro
                if (event.x && event.y)
                { // controllo le coordinate del click
                }
            }
        }
    }

    // printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

    curs_set(false);
}

void colori()
{
    start_color();
    init_color(COLORE_RANA, 75, 890, 20);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 150, 150, 150);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito 259 /259/259
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(4, COLOR_BLACK, COLOR_GREEN); // colore prato
    init_pair(5, COLOR_BLACK, COLOR_BLUE);  // colore fiume
    init_pair(6, COLOR_BLACK, COLORE_TRONCHI);
}

int controlloRanaTronco(Coordinate rana, Oggetto tronco[3])
{
    bool flag = false;
    int i, j;

    for (i = 0; i < TRE; i++)
    {
        flag = false;
        for (j = 0; j < LARGHEZZA_RANA; j++)
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
    rana.x = -1;
    return rana.x;
}