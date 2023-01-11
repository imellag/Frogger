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
#include "funzioniMain.h"

char spriteProiettile = '^';

int main()
{
    srand(time(NULL));

    int maxx, maxy;
    int differenza;
    int maxx_precedente, maxy_precedente;
    int tempo = 40, punteggio = ZERO, vite = TRE;

    bool arrayTane[NUMERO_TANE] = {false, false, false, false, false};
    int risultato;
    int gameDifficulty;
    _Bool coloreTroncoRana = false;
    _Bool sulTronco = false;

    Oggetto ranocchio;
    ranocchio.coordinate.x = ZERO;
    ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;

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

    // pipe principale che viene utilizzata per inviare le posizioni dei vari oggetti
    // e stamparli successivamente nel main
    int p[DUE];
    if (pipe(p) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }

    // pipe non bloccante che mi serve per comunicare con il processo rana
    // in caso di collisioni o spostamenti(es: rana sul tronco)
    int pRana[DUE];
    if (pipe(pRana) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }

    // funzione per rendere non bloccante la pipe
    fcntl(pRana[0], F_SETFL, fcntl(pRana[0], F_GETFL) | O_NONBLOCK);

    // pipe non bloccante che mi serve per comunicare con il processo rana
    // in caso di collisioni o spostamenti(es: rana sul tronco)
    int pOrologio[DUE];
    if (pipe(pOrologio) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }

    // funzione per rendere non bloccante la pipe
    fcntl(pOrologio[0], F_SETFL, fcntl(pOrologio[0], F_GETFL) | O_NONBLOCK);

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(vite);

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(ranocchio.coordinate, coloreTroncoRana);

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
        funzRana(p, pRana);
    }
    else
    {
        funzAuto(p);
        funzTronchi(p, pRana);
        funzTempo(pOrologio);

        int i;
        Oggetto pacchetto;
        Oggetto proiettilino;

        Coordinate nuoveCoordinate;

        Oggetto tronchino[TRE];
        Oggetto macchinina[CINQUE];
        Oggetto camioncino[TRE];

        for (i = 0; i < 5; i++)
        {
            if (i < 3)
            {
                tronchino[i].coordinate.x = -CINQUE;
                tronchino[i].coordinate.y = -CINQUE;

                camioncino[i].coordinate.x = -CINQUE;
                camioncino[i].coordinate.y = -CINQUE;
            }

            macchinina[i].coordinate.x = -CINQUE;
            macchinina[i].coordinate.y = -CINQUE;
        }

        proiettilino.coordinate.x = -UNO;
        proiettilino.coordinate.y = -UNO;
        _Bool fuorischermo = false;

        int direzione;

        close(p[WRITE]);
        close(pRana[READ]);
        close(pOrologio[WRITE]);
        while (true)
        {

            read(p[READ], &pacchetto, sizeof(Oggetto));
            read(pOrologio[READ], &tempo, sizeof(int));
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

            // controllo se la rana è entrata nelle tane allora la porto alla posizione iniziale e aggiorno il punteggio
            if (risultato < SEI && risultato >= UNO)
            {
                if (!(arrayTane[risultato - 1] == true))
                {
                    arrayTane[risultato - 1] = true;
                    ranocchio=posizioneInizialeRana(pRana, ranocchio);
                    punteggio += 2000;
                }
            }

            // stampa dei colori di background
            funzMarciapiede();
            funzAutostrada();
            funzPrato();
            funzFiume();

            // stampa la tane e ne tiene controllo
            funzTane(arrayTane);

            coloreTroncoRana = false;

            // ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
            // con le varia macchine o se la rana è presente sul tronco
            for (i = 0; i < CINQUE; i++)
            {
                if (i < 3)
                {
                    // stampo i 3 tronchi
                    stampaTronco(tronchino[i].coordinate);
                    // mi prendo i pid dei tronchi per poi utilizzare questo array per
                    // killare correttamente i vari processi tronchi
                    pidTronchi[i] = tronchino[i].pid;

                    // stampo i camion
                    stampaCamion(camioncino[i]);

                    // mi prendo i pid dei camion per poi utilizzare questo array per
                    // killare correttamente i vari processi camion
                    pidCamion[i] = camioncino[i].pid;

                    // controllo se la rana è salita sul tronco
                    if (tronchino[i].coordinate.x <= ranocchio.coordinate.x && (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= ranocchio.coordinate.x && ranocchio.coordinate.y == tronchino[i].coordinate.y)
                    {

                        // appena salita la rana mi calcolo la differenza tra l'inizio del tronco e la posizione della rana
                        // così facendo la rana salirà sul tronco nel punto esatto
                        if (!sulTronco)
                        {

                            differenza = ranocchio.coordinate.x - tronchino[i].coordinate.x;
                            sulTronco = true;
                        }
                        ranocchio.coordinate.x = tronchino[i].coordinate.x + differenza;
                        coloreTroncoRana = true;

                        // comunico la posizione al processo rana
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    }

                    // in questo casa la rana è sull'altezza del tronco però una x diversa dal range di esso
                    // pertanto non è riuscita a salire sul tronco successivo quindi comunico le coordinate di partenza
                    // e tolgo una vita
                    else if (ranocchio.coordinate.y == tronchino[i].coordinate.y)
                    {
                        vite--;
                        ranocchio=posizioneInizialeRana(pRana, ranocchio);
                        sulTronco = false;
                    }

                    // controllo la direzione inversa
                    if (camioncino[i].velocita < 0)
                    {
                        // controllo se la rana è stata investita da un camioncino
                        if (camioncino[i].coordinate.x > ranocchio.coordinate.x && (camioncino[i].coordinate.x - LARGHEZZA_CAMION) < ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
                        {
                            vite--;
                            ranocchio=posizioneInizialeRana(pRana, ranocchio);
                        }
                    }
                    else
                    {
                        if (camioncino[i].coordinate.x < ranocchio.coordinate.x && (camioncino[i].coordinate.x + LARGHEZZA_CAMION) > ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
                        {
                            vite--;
                            ranocchio=posizioneInizialeRana(pRana, ranocchio);
                        }
                    }
                }

                stampaMacchina(macchinina[i]);
                pidMacchine[i] = macchinina[i].pid;

                // se la macchina ha direzione inversa
                if (macchinina[i].velocita < 0)
                {
                    // controllo se la rana è dentro il range dello sprite della macchina
                    // allora la porto alla alla posizione di partenza e tolgo una vita
                    if (macchinina[i].coordinate.x > ranocchio.coordinate.x && (macchinina[i].coordinate.x - LARGHEZZA_MACCHINA) < ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio=posizioneInizialeRana(pRana, ranocchio);
                    }
                }
                else
                {
                    // controllo se la rana è dentro il range dello sprite della macchina
                    // allora la porto alla alla posizione di partenza e tolgo una vita
                    if (macchinina[i].coordinate.x < ranocchio.coordinate.x && (macchinina[i].coordinate.x + LARGHEZZA_MACCHINA) > ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
                    {
                        vite--;
                        ranocchio=posizioneInizialeRana(pRana, ranocchio);
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

            if (ranocchio.id == q || vite == ZERO || tempo <= 0)
            {
                if (vite == ZERO)
                    gameOver();

                for (i = 0; i < 5; i++)
                {
                    if (i < 3)
                    {
                        kill(pidCamion[i], SIGKILL);
                        kill(pidTronchi[i], SIGKILL);
                    }

                    kill(pidMacchine[i], SIGKILL);
                }

                endwin();
                kill(pidRana, SIGKILL);

                return EXIT_SUCCESS;
            }
        }
    }
}
