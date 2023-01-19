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
    int differenza[TRE];
    int maxx_precedente, maxy_precedente;
    int vite = TRE;

    double diff_nemico, diff_proiettile[3];
    int spawnNemico;
    int troncoNemico;

    bool arrayTane[NUMERO_TANE] = {false, false, false, false, false};

    bool nemico[TRE] = {false, false, false};
    int risultato;
    int gameDifficulty;
    _Bool coloreTroncoRana = false;
    _Bool sulTronco[TRE] = {false, false, false};

    time_t inizio_nemico, fine_nemico, inizio_proiettile[TRE], fine_proiettile[TRE];
    Oggetto proiettileNemico[TRE];
    Oggetto ranocchio;
    Schermo statistiche;

    statistiche.tempo = 50;
    statistiche.punteggio = ZERO;
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
    fcntl(pRana[ZERO], F_SETFL, fcntl(pRana[ZERO], F_GETFL) | O_NONBLOCK);

    // pipe non bloccante che mi serve per comunicare con il processo rana
    // in caso di collisioni o spostamenti(es: rana sul tronco)
    int pOrologio[DUE];
    if (pipe(pOrologio) == -UNO)
    {
        perror("Error\n");
        exit(-UNO);
    }

    // funzione per rendere non bloccante la pipe
    fcntl(pOrologio[ZERO], F_SETFL, fcntl(pOrologio[ZERO], F_GETFL) | O_NONBLOCK);

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

    // mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
    // mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %d", tempo);
    refresh();

    pid_t pidRana, pidMacchine[CINQUE], pidTronchi[TRE], pidNemici, pidProiettile, pidCamion[TRE], pidSchermo;

    funzRana(p, pRana);
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
    // Oggetto nemico[TRE];
    Coordinate vecchieNemico[TRE];
    int j;
    for (i = ZERO; i < CINQUE; i++)
    {
        if (i < TRE)
        {
            tronchino[i].coordinate.x = -CINQUE;
            tronchino[i].coordinate.y = -CINQUE;

            vecchieNemico[i].x = -CINQUE;
            vecchieNemico[i].y = -CINQUE;
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
    time(&inizio_nemico);
    for (i = ZERO; i < TRE; i++)
        time(&inizio_proiettile[i]);

    close(p[WRITE]);
    close(pRana[READ]);
    close(pOrologio[WRITE]);
    while (true)
    {

        read(p[READ], &pacchetto, sizeof(Oggetto));
        read(pOrologio[READ], &statistiche, sizeof(Schermo));
        
        switch (pacchetto.id)
        {
        case RANA:
            ranocchio = pacchetto;
            break;

        case PROIETTILE:
            proiettilino = pacchetto;
            fuorischermo = false;
            break;

        case PROIETTILE_NEMICO0:
            proiettileNemico[ZERO] = pacchetto;
            break;
        case PROIETTILE_NEMICO1:
            proiettileNemico[UNO] = pacchetto;
            break;
        case PROIETTILE_NEMICO2:
            proiettileNemico[DUE] = pacchetto;
            break;

        case PROIETTILE_NEMICO0_OUT:
            proiettileNemico[ZERO].coordinate.x = -20;
            proiettileNemico[ZERO].coordinate.y = -20;
            break;
        case PROIETTILE_NEMICO1_OUT:
            proiettileNemico[UNO].coordinate.x = -20;
            proiettileNemico[UNO].coordinate.y = -20;
            break;
        case PROIETTILE_NEMICO2_OUT:
            proiettileNemico[DUE].coordinate.x = -20;
            proiettileNemico[DUE].coordinate.y = -20;
            break;
        case PROIETTILE_OUT:
            proiettilino.coordinate.x = -20;
            proiettilino.coordinate.y = -20;
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
            if (!(arrayTane[risultato - UNO] == true))
            {
                arrayTane[risultato - UNO] = true;
                ranocchio = posizioneInizialeRana(pRana, ranocchio);
                statistiche.punteggio += 2000;
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
        time(&fine_nemico);

        if ((diff_nemico = difftime(fine_nemico, inizio_nemico)) >= CINQUE)
        {
            do
            {
                troncoNemico = rand() % TRE;
            } while (tronchino[troncoNemico].coordinate.y == ranocchio.coordinate.y);

            nemico[troncoNemico] = true;
            time(&inizio_nemico);
        }

        // ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
        // con le varia macchine o se la rana è presente sul tronco
        for (i = ZERO; i < CINQUE; i++)
        {
            if (i < TRE)
            {
                time(&fine_proiettile[i]);

                if (proiettilino.coordinate.x >= tronchino[i].coordinate.x &&
                    proiettilino.coordinate.x <= tronchino[i].coordinate.x + LARGHEZZA_TRONCHI && 
                    proiettilino.coordinate.y == tronchino[i].coordinate.y + 2 && nemico[i] == true)
                {
                    nemico[i] = false;
                    kill(proiettilino.pid, SIGKILL);
                    fuorischermo = true;
                    proiettilino.coordinate.x = -CINQUE;
                    statistiche.punteggio += 50;
                }
                if (nemico[i])
                {
                    stampaNemico(tronchino[i].coordinate);

                    time(&fine_proiettile[i]);
                    if ((diff_proiettile[i] = difftime(fine_proiettile[i], inizio_proiettile[i])) >= CINQUE)
                    {

                        time(&inizio_proiettile[i]);
                        funzProiettileNemico(tronchino[i].coordinate, p, i);
                    }
                }
                else
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
                if (tronchino[i].coordinate.x <= ranocchio.coordinate.x && 
                    (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= ranocchio.coordinate.x && 
                    ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    if (nemico[i])
                    {
                        vite--;
                        ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    }
                    else
                    {
                        // appena salita la rana mi calcolo la differenza tra l'inizio del tronco e la posizione della rana
                        // così facendo la rana salirà sul tronco nel punto esatto
                        if (!sulTronco[i])
                        {
                            differenza[i] = ranocchio.coordinate.x - tronchino[i].coordinate.x;
                            sulTronco[i] = true;
                        }
                        ranocchio.coordinate.x = tronchino[i].coordinate.x + differenza[i];
                        coloreTroncoRana = true;

                        // comunico la posizione al processo rana
                        write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    }
                }

                // in questo casa la rana è sull'altezza del tronco però una x diversa dal range di esso
                // pertanto non è riuscita a salire sul tronco successivo quindi comunico le coordinate di partenza
                // e tolgo una vita
                else if (ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    sulTronco[i] = false;
                }

                if (controlloCollisioneOggetti(camioncino[i], ranocchio.coordinate, LARGHEZZA_CAMION))
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                }
            }

            stampaMacchina(macchinina[i]);
            pidMacchine[i] = macchinina[i].pid;

            // controllo se la rana è dentro il range dello sprite della macchina
            // allora la porto alla alla posizione di partenza e tolgo una vita
            if (controlloCollisioneOggetti(macchinina[i], ranocchio.coordinate, LARGHEZZA_MACCHINA))
            {
                vite--;
                ranocchio = posizioneInizialeRana(pRana, ranocchio);
            }
        }
        pidSchermo = statistiche.pid;
        stampaRana(ranocchio.coordinate, coloreTroncoRana);

        stampaVite(vite);
        mvwprintw(stdscr, proiettilino.coordinate.y, proiettilino.coordinate.x, "%c", spriteProiettile);
        for (i = ZERO; i < TRE; i++)
            mvprintw(proiettileNemico[i].coordinate.y, proiettileNemico[i].coordinate.x, "%c", spriteProiettile);

        mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", statistiche.punteggio);
        mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %-20d", statistiche.tempo);
        refresh();

        if (ranocchio.id == q || vite == ZERO || statistiche.tempo <= ZERO)
        {
            if (vite == ZERO)
                gameOver();
            endwin();
            for (i = ZERO; i < CINQUE; i++)
            {
                if (i < TRE)
                {
                    kill(pidCamion[i], SIGKILL);
                    kill(pidTronchi[i], SIGKILL);
                }

                kill(pidMacchine[i], SIGKILL);
            }
            kill(pidRana, SIGKILL);
            kill(pidSchermo, SIGKILL);

            return EXIT_SUCCESS;
        }
    }
}
