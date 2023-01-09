#include "lib.h"
#include "funzioniGenerali.h"
#include "rana.h"
#include "marciapiede.h"
#include "autostrada.h"
#include "prato.h"
#include "fiume.h"
#include "tane.h"
#include "avvio.h"
#include "collisioni.h"

char spriteProiettile = '^';
char spriteCuore[] = {"<3"};

int main()
{
    srand(time(NULL));

    int maxx, maxy;
    int differenza;
    int maxx_precedente, maxy_precedente;
    int tempo = 30, punteggio = ZERO, vite = TRE;
    Coordinate rana;
    bool arrayTane[CINQUE] = {false, false, false, false, false};
    int risultato;
    int gameDifficulty;
    _Bool coloreTroncoRana = false;
    _Bool sulTronco = false;

    rana.x = ZERO;
    rana.y = ALTEZZA_SCHERMO - SEI;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    start_color();
    keypad(stdscr, true);
    getmaxyx(stdscr, maxy, maxx);

    clear();
    refresh();

    dimensioneFinestra(maxx, maxy);

    // gameDifficulty = menuIniziale();
    clear();

    colori();

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

    int pTronchi[DUE];
    if (pipe(pTronchi) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }
    fcntl(pTronchi[0], F_SETFL, fcntl(pTronchi[0], F_GETFL) | O_NONBLOCK);

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(vite);

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(rana, coloreTroncoRana);

    mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
    mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
    refresh();

    pid_t pidRana, pidMacchine[CINQUE], pidTronchi[TRE], pidNemici, pidProiettile, pidCamion[TRE], schermo;
    pidRana = fork();

    if (pidRana < ZERO)
    {
        printw("Error");
        exit(-1);
    }
    else if (pidRana == ZERO)
    {
        funzRana(p, pRana, pTronchi);
    }
    else
    {
        funzAuto(p);
        funzTronchi(p, pTronchi, pRana);

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
        Oggetto macchinina[CINQUE];
        for (i = 0; i < 5; i++)
        {
            macchinina[i].coordinate.x = -CINQUE;
            macchinina[i].coordinate.y = -CINQUE;
        }
        Oggetto camioncino[TRE];
        for (i = 0; i < 3; i++)
        {
            camioncino[i].coordinate.x = -CINQUE;
            camioncino[i].coordinate.y = -CINQUE;
        }

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
            case MACCHINA3:
                macchinina[TRE] = pacchetto;
                break;
            case MACCHINA4:
                macchinina[QUATTRO] = pacchetto;
                break;

            case CAMION0:
                camioncino[ZERO] = pacchetto;
                break;
            case CAMION1:
                camioncino[UNO] = pacchetto;
                break;
            case CAMION2:
                camioncino[DUE] = pacchetto;
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

            risultato = controlloLimiti(ranocchio.coordinate, RANA);

            if (risultato < SEI && risultato >= UNO)
            {
                arrayTane[risultato - 1] = true;
                ranocchio.coordinate.x = ZERO;
                ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                clear();
                punteggio += 2000;
            }

            funzMarciapiede();
            funzAutostrada();
            funzPrato();
            funzFiume();
            funzTane(arrayTane);

            for (i = ZERO; i < TRE; i++)
            {
                stampaTronco(tronchino[i].coordinate);

                pidTronchi[i] = tronchino[i].pid;
            }

            for (i = 0; i < CINQUE; i++)
            {
                stampaMacchina(macchinina[i]);
                pidMacchine[i] = macchinina[i].pid;
            }

            for (i = 0; i < TRE; i++)
            {
                stampaCamion(camioncino[i]);
                pidCamion[i] = camioncino[i].pid;
            }

            for (i = 0; i < CINQUE; i++)
            {
                if (macchinina[i].velocita < 0)
                {
                    if (macchinina[i].coordinate.x > ranocchio.coordinate.x && (macchinina[i].coordinate.x - LARGHEZZA_MACCHINA) < ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio.coordinate.x = ZERO;
                        ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                        clear();
                    }
                }
                else
                {
                    if (macchinina[i].coordinate.x < ranocchio.coordinate.x && (macchinina[i].coordinate.x + LARGHEZZA_MACCHINA) > ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio.coordinate.x = ZERO;
                        ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                        clear();
                    }
                }
            }
            coloreTroncoRana = false;
            for (i = 0; i < TRE; i++)
            {

                if (tronchino[i].coordinate.x <= ranocchio.coordinate.x && (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= ranocchio.coordinate.x && ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    if (!sulTronco)
                    {

                        differenza = ranocchio.coordinate.x - tronchino[i].coordinate.x;
                        sulTronco = true;
                    }
                    ranocchio.coordinate.x = tronchino[i].coordinate.x + differenza;
                    coloreTroncoRana = true;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                }
                else if (ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    vite--;
                    ranocchio.coordinate.x = ZERO;
                    ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    clear();
                    sulTronco = false;
                }
                else
                {
                }

                if (camioncino[i].velocita < 0)
                {
                    if (camioncino[i].coordinate.x > ranocchio.coordinate.x && (camioncino[i].coordinate.x - LARGHEZZA_CAMION) < ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio.coordinate.x = ZERO;
                        ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                        clear();
                    }
                }
                else
                {
                    if (camioncino[i].coordinate.x < ranocchio.coordinate.x && (camioncino[i].coordinate.x + LARGHEZZA_CAMION) > ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio.coordinate.x = ZERO;
                        ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                        clear();
                    }
                }
            }
            stampaRana(ranocchio.coordinate, coloreTroncoRana);

            stampaVite(vite);
            if (!fuorischermo)
                mvwprintw(stdscr, proiettilino.coordinate.y, proiettilino.coordinate.x, "%c", spriteProiettile);

            mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
            mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
            refresh();

            if (ranocchio.id == q || vite == ZERO)
            {
                if (vite == ZERO)
                    gameOver();

                for (i = 0; i < 3; i++)

                    kill(pidTronchi[i], SIGKILL);
                for (i = 0; i < 5; i++)

                    kill(pidMacchine[i], SIGKILL);

                for (i = 0; i < 2; i++)
                    kill(pidCamion[i], SIGKILL);

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
