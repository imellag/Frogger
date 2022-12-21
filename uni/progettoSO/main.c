#include "lib.h"
#include "funzioniGenerali.h"
#include "rana.h"
#include "marciapiede.h"
#include "autostrada.h"
#include "prato.h"
#include "fiume.h"
#include "tane.h"

char spriteProiettile = '^';
char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA + UNO] = {" o.o ", "+-|-+", "\\-|-/"};
char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI + UNO] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA] = {" /^\\_", "| __ |", "o   o"};
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

        perror("Error");
    }
    fcntl(pRana[ZERO], F_SETFL, fcntl(pRana[ZERO], F_GETFL) | O_NONBLOCK);

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

    pid_t pidRana, pidMacchine, pidTronchi, pidNemici, pidProiettile;
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
        Oggetto tronchino[TRE];
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
                direzione = controlloLimiti(ranocchio.coordinate, RANA);
                switch (direzione)
                {
                case ZERO:
                    break;
                case UNO:
                    ranocchio.coordinate.x += LARGHEZZA_RANA;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    break;

                case DUE:
                    ranocchio.coordinate.x -= LARGHEZZA_RANA;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    break;
                case TRE:
                    ranocchio.coordinate.y += ALTEZZA_RANA;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    break;
                case QUATTRO:
                    ranocchio.coordinate.y -= ALTEZZA_RANA;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    break;
                }

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

            //  stampa
            stampaVite(vite);

            funzMarciapiede();
            funzAutostrada();
            funzPrato();
            funzFiume();
            for (i = ZERO; i < TRE; i++)
            {
                stampaTronco(tronchino[i].coordinate);
                stampaMacchina(macchinina[i].coordinate);
            }

            stampaRana(ranocchio.coordinate);

            if (!fuorischermo)
                mvwprintw(stdscr, proiettilino.coordinate.y, proiettilino.coordinate.x, "%c", spriteProiettile);

            mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
            mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
            refresh();

            if (ranocchio.id == q)
            {
                endwin();
                kill(pidRana, SIGKILL);
                kill(pidMacchine, SIGKILL);
                kill(pidTronchi, SIGKILL);

                return EXIT_SUCCESS;
            }
        }
    }
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

void stampaRana(Coordinate rana)
{
    int i, j, colorePosizione;
    colorePosizione = controlloPosizione(rana);
    init_pair(SETTE, COLORE_RANA, colorePosizione);
    attron(COLOR_PAIR(SETTE));
    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_RANA; j++)
        {
            mvaddch(rana.y + i, rana.x + j, spriteRana[i][j]);
        }
    }
    attroff(COLOR_PAIR(7));
}

void stampaVite(int vite)
{
    Coordinate vita;
    vita.x = LARGHEZZA_SCHERMO - TRE;
    vita.y = ALTEZZA_SCHERMO - ALTEZZA_RANA;
    /* inizio a stamparle da in basso a destra, poi mi sposto
    verso sinistra */
    attron(COLOR_RED);
    for (int i = ZERO; i < vite; i++)
    {
        for (int j = ZERO; j < DUE; j++)
            mvprintw(vita.y, vita.x, "%c", spriteCuore[j]);
            
        vita.x -= DUE; // lascio 1 di spazio tra le varie vite
    }
    attroff(COLOR_RED);
}

void stampaTronco(Coordinate tronco)
{

    int i, j;
    attron(COLOR_PAIR(SEI));
    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_TRONCHI; j++)
            mvaddch(tronco.y + i, tronco.x + j, spriteTronchi[i][j]);
    }
    attroff(COLOR_PAIR(SEI));
}

void stampaMacchina(Coordinate macchina)
{
    int i, j;
    attron(COLOR_PAIR(UNO));
    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_MACCHINA; j++)
            mvprintw(macchina.y + i, macchina.x + j, "%c", spriteMacchine[i][j]);
    }
    attroff(COLOR_PAIR(UNO));
}

int controlloPosizione(Coordinate rana)
{

    if (rana.y == 29)
        return COLORE_MARCIAPIEDE;
    else if (rana.y >= 20 && rana.y < 29)
        return COLORE_AUTOSTRADA;
    else if (rana.y == 17)
        return COLOR_GREEN;
    else if (rana.y >= 8 && rana.y < 17)
        return COLOR_BLUE;
}
