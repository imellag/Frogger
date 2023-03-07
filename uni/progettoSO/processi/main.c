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

int main()
{
    srand(time(NULL));
    pid_t pidRana, pidMacchine[NUMERO_MACCHINE], pidTronchi[NUMERO_TRONCHI],
        pidNemici, pidProiettile, pidCamion[NUMERO_CAMION], pidSchermo;

    int maxx, maxy;
    int differenza;
    int maxx_precedente, maxy_precedente;
    int vite = MAX_VITE;
    int risultato;
    int direzione;
    int gameDifficulty;
    int spawnNemico;
    int troncoNemico;
    bool buffer = false;

    bool arrayTane[NUMERO_TANE] = {false, false, false, false, false};

    bool nemico[NUMERO_TRONCHI] = {false, false, false};

    bool coloreTroncoRana = false;
    bool coloreTroncoProiettile = false;
    bool coloreNemicoProiettile = false;
    bool sulTronco = false;

    bool hitProiettile[3] = {false};

    // variabili utilizzate per far spawnare i nemici e farli sparare al momento giusto
    time_t inizio_nemico, fine_nemico, inizio_proiettile[3], fine_proiettile;

    Oggetto proiettileNemico[NUMERO_NEMICI];
    Oggetto ranocchio;
    Oggetto vecchiaRana, vecchiaRana2;

    Tempo secondo;
    secondo.tempo = false;

    int i, j;
    Oggetto pacchetto;
    Oggetto proiettilino;
    Coordinate nuoveCoordinate;

    Oggetto tronchino[NUMERO_TRONCHI];
    Oggetto macchinina[NUMERO_MACCHINE];
    Oggetto camioncino[NUMERO_CAMION];
    Colore bufferColori;

    int tempo = TEMPO_INIZIALE;
    int punteggio = PUNTEGGIO_INIZIALE;
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

    for (i = ZERO; i < NUMERO_MACCHINE; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_MACCHINA0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
    for (i = ZERO; i < NUMERO_CAMION; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_CAMION0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }

    // pipe principale che viene utilizzata per inviare le posizioni dei vari oggetti
    // e stamparli successivamente nel main
    int p[DUE];
    if (pipe(p) == -UNO)
    {
        perror("Error\n");
        exit(EXIT_FAILURE);
    }

    // pipe non bloccante che mi serve per comunicare con il processo rana
    // in caso di collisioni o spostamenti(es: rana sul tronco)
    int pRana[DUE];
    if (pipe(pRana) == -UNO)
    {
        perror("Error\n");
        exit(EXIT_FAILURE);
    }

    // funzione per rendere non bloccante la pipe
    fcntl(pRana[0], F_SETFL, fcntl(pRana[0], F_GETFL) | O_NONBLOCK);

    // pipe non bloccante che mi serve per comunicare con il processo rana
    // in caso di collisioni o spostamenti(es: rana sul tronco)
    int pOrologio[DUE];
    if (pipe(pOrologio) == -UNO)
    {
        perror("Error\n");
        exit(EXIT_FAILURE);
    }

    // funzione per rendere non bloccante la pipe
    fcntl(pOrologio[0], F_SETFL, fcntl(pOrologio[0], F_GETFL) | O_NONBLOCK);

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(vite);

    // stampo le varie sezioni dello schermo
    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(ranocchio.coordinate, coloreTroncoRana);

    refresh();

    // creo tutti i processi della rana, dei veicoli, dei tronchi e del tempo
    funzRana(p, pRana);
    funzAuto(p);
    funzTronchi(p);
    funzTempo(pOrologio);

    // prima di iniziare porto tutti gli oggetti fuori dallo schermo
    for (i = ZERO; i < CINQUE; i++)
    {
        if (i < TRE)
        {
            tronchino[i].coordinate.x = FUORI_MAPPA;
            tronchino[i].coordinate.y = FUORI_MAPPA;

            camioncino[i].coordinate.x = FUORI_MAPPA;
            camioncino[i].coordinate.y = FUORI_MAPPA;
        }

        macchinina[i].coordinate.x = FUORI_MAPPA;
        macchinina[i].coordinate.y = FUORI_MAPPA;
    }

    proiettilino.coordinate.x = FUORI_MAPPA;
    proiettilino.coordinate.y = FUORI_MAPPA;

    time(&inizio_nemico);

    close(pRana[READ]);
    close(pOrologio[WRITE]);
    /* Leggo i dati della pipe p dove vengono scritte le posizioni di tutti gli oggetti.
    Quello che viene letto dalla pipe viene salvato in una variabile provvisoria e,
    dopo essere stato riconosciuto tramite il suo id, viene salvato nella variabile dell'oggetto
    corrispondente. Viene anche letto il tempo aggiornato in secondi. */
    while (true)
    {
        read(p[READ], &pacchetto, sizeof(Oggetto));
        read(pOrologio[READ], &secondo, sizeof(Tempo));
        switch (pacchetto.id)
        {
        case RANA:
            ranocchio = pacchetto;
            vecchiaRana = pacchetto;
            break;

        case PROIETTILE:
            proiettilino = pacchetto;
            break;
        case PROIETTILE_OUT:
            proiettilino.coordinate.x = FUORI_MAPPA;
            proiettilino.coordinate.y = FUORI_MAPPA;
            break;

        case PROIETTILE_NEMICO0:
        case PROIETTILE_NEMICO1:
        case PROIETTILE_NEMICO2:
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0] = pacchetto;
            break;

        case PROIETTILE_NEMICO0_OUT:
        case PROIETTILE_NEMICO1_OUT:
        case PROIETTILE_NEMICO2_OUT:
            hitProiettile[pacchetto.id - PROIETTILE_NEMICO0_OUT] = false;
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0_OUT].coordinate.x = FUORI_MAPPA;
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0_OUT].coordinate.y = FUORI_MAPPA;
            break;

        case TRONCO0:
        case TRONCO1:
        case TRONCO2:
            tronchino[pacchetto.id - TRONCO0] = pacchetto;
            break;

        case MACCHINA0:
        case MACCHINA1:
        case MACCHINA2:
        case MACCHINA3:
        case MACCHINA4:
            macchinina[pacchetto.id - MACCHINA0] = pacchetto;
            break;

        case CAMION0:
        case CAMION1:
        case CAMION2:
            camioncino[pacchetto.id - CAMION0] = pacchetto;
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

        // controllo se la rana è entrata in una tana
        risultato = controlloLimiti(ranocchio.coordinate, RANA);

        // se la rana è entrata in una tana viene portata alla posizione iniziale viene aggiornato il punteggio
        if (risultato < SEI && risultato >= UNO)
        {
            if (!(arrayTane[risultato - UNO] == true))
            {
                arrayTane[risultato - UNO] = true; // viene chiusa la tana
                ranocchio = posizioneInizialeRana(pRana, ranocchio);
                punteggio += PUNTEGGIO_TANA;
                tempo += TEMPO_TANA;
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

        /* controllo se it tempo di attesa dello spawn del nemico è finito e se è
        così viene stampato in un tronco che non è occupato dalla rana o da un altro nemico */
        if ((fine_nemico - inizio_nemico) >= SPAWN_NEMICO)
        {
            do
            {
                troncoNemico = rand() % NUMERO_TRONCHI;
            } while (tronchino[troncoNemico].coordinate.y == ranocchio.coordinate.y &&
                     nemico[troncoNemico] == true);

            nemico[troncoNemico] = true;
            time(&inizio_nemico);
            time(&inizio_proiettile[troncoNemico]);
        }

        /* ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
         con le varie macchine o se la rana è presente sul tronco */
        for (i = ZERO; i < NUMERO_MACCHINE; i++)
        {
            if (i < NUMERO_CAMION)
            {
                /* per ognuno dei proiettili nemici controllo che non collida con il proiettile della rana,
                e nel caso li distruggo entrambi */
                if (controlloCollisioniProiettili(proiettilino.coordinate, proiettileNemico[i].coordinate))
                {
                    kill(proiettilino.pid, SIGKILL);
                    proiettilino.coordinate.x = FUORI_MAPPA;
                    proiettilino.coordinate.y = FUORI_MAPPA;

                    hitProiettile[i] = true;
                }
                // controllo se i proiettili collidono con un camion e nel caso li distruggo
                if (proiettiliVeicoli(proiettilino, proiettileNemico, camioncino[i].coordinate, LARGHEZZA_CAMION, hitProiettile))
                {
                    kill(proiettilino.pid, SIGKILL);
                    proiettilino.coordinate.x = FUORI_MAPPA;
                    proiettilino.coordinate.y = FUORI_MAPPA;
                }
                // controllo se un proiettile nemico collide con la rana e nel caso lo distruggo e tolgo una vita
                if (controlloCollisioniRanaProiettile(proiettileNemico[i].coordinate, ranocchio.coordinate) &&
                    hitProiettile[i] == false)
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    kill(proiettileNemico[i].pid, SIGKILL);
                    hitProiettile[i] = true;

                    // beep();
                }

                /* controllo la collisione del proiettile con i tronchi e se è presente un nemico lo uccido
                e distruggo il proiettile */
                if (proiettilino.coordinate.x >= tronchino[i].coordinate.x &&
                    proiettilino.coordinate.x <= tronchino[i].coordinate.x + LARGHEZZA_TRONCHI &&
                    proiettilino.coordinate.y == tronchino[i].coordinate.y + DUE && nemico[i] == true)
                {
                    nemico[i] = false;
                    kill(proiettilino.pid, SIGKILL);
                    proiettilino.coordinate.x = FUORI_MAPPA;
                    proiettilino.coordinate.y = FUORI_MAPPA;
                    punteggio += PUNTEGGIO_UCCISIONE;
                }

                /* se il nemico non è morto lo stampo sul tronco e se è pronto a sparare il proiettile
                ne creo il processo */
                if (nemico[i])
                {
                    stampaNemico(tronchino[i].coordinate);

                    time(&fine_proiettile);
                    if ((fine_proiettile - inizio_proiettile[i]) >= DUE)
                    {
                        refresh();
                        time(&inizio_proiettile[i]);
                        funzProiettileNemico(tronchino[i].coordinate, p, i);
                    }
                }
                // altrimenti, se non è presente un nemico stampo solamente il tronco
                else
                    stampaTronco(tronchino[i].coordinate);

                // mi prendo i pid dei tronchi per poi utilizzare questo array per
                // killare correttamente i vari processi tronchi
                pidTronchi[i] = tronchino[i].pid;

                // stampo i camion
                stampaCamion(camioncino[i], i);

                // mi prendo i pid dei camion per poi utilizzare questo array per
                // killare correttamente i vari processi camion
                pidCamion[i] = camioncino[i].pid;

                if (vecchiaRana.coordinate.x != vecchiaRana2.coordinate.x || vecchiaRana.coordinate.y != vecchiaRana2.coordinate.y)
                {
                    vecchiaRana2 = vecchiaRana;
                    sulTronco = false;
                }
                // controllo se la rana è salita sul tronco
                if (tronchino[i].coordinate.x <= ranocchio.coordinate.x && (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= ranocchio.coordinate.x && ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    if (nemico[i])
                    {
                        vite--;
                        ranocchio = posizioneInizialeRana(pRana, ranocchio);
                        beep();
                    }
                    else
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
                }

                // in questo casa la rana è sull'altezza del tronco però una x diversa dal range di esso
                // pertanto non è riuscita a salire sul tronco successivo quindi comunico le coordinate di partenza
                // e tolgo una vita
                else if (ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    sulTronco = false;
                    // system("ffplay ../file_audio/rana_in_acqua.mp3 2> /dev/null & ");
                }
                // controllo la collisione della rana con l'array dei camion e se è dentro la sprite di un camion
                // tolgo una vita e resetto la sua posizione
                if (controlloCollisioneOggetti(camioncino[i], ranocchio.coordinate, LARGHEZZA_CAMION))
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    // system("ffplay ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
                // la rana  prova a entrare in una tana già chiusa
                if (ranocchio.coordinate.y == INIZIO_TANE)
                {
                    vite--;
                    ranocchio = posizioneInizialeRana(pRana, ranocchio);
                    beep();
                }
            }
            // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
            if (proiettiliVeicoli(proiettilino, proiettileNemico, macchinina[i].coordinate, LARGHEZZA_MACCHINA, hitProiettile))
            {
                kill(proiettilino.pid, SIGKILL);
                proiettilino.coordinate.x = FUORI_MAPPA;
                proiettilino.coordinate.y = FUORI_MAPPA;
            }

            stampaMacchina(macchinina[i], i);
            pidMacchine[i] = macchinina[i].pid;

            // controllo se la rana è dentro il range dello sprite della macchina
            // allora la porto alla alla posizione di partenza e tolgo una vita
            if (controlloCollisioneOggetti(macchinina[i], ranocchio.coordinate, LARGHEZZA_MACCHINA))
            {
                vite--;
                ranocchio = posizioneInizialeRana(pRana, ranocchio);
                // system("ffplay ../file_audio/rana_investita.mp3 2> /dev/null & ");
            }
        }

        pidSchermo = secondo.pid;
        stampaRana(ranocchio.coordinate, coloreTroncoRana);

        stampaVite(vite);

        // controllo se i proiettili nemici sono in movimento e nel caso li stampo
        stampaProiettile(proiettilino.coordinate, tronchino, nemico);
        for (i = ZERO; i < TRE; i++)
        {
            if (!hitProiettile[i])
                stampaProiettile(proiettileNemico[i].coordinate, tronchino, nemico);
        }

        // stampo il puntggio nella parte alta dello schermo e il tempo rimanente nella parte bassa
        mvprintw(UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", punteggio);
        mvprintw(ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %-20d", tempo);

        if (secondo.tempo)
        {
            tempo--;
            punteggio -= DIECI;
        }

        refresh();
        secondo.tempo = false;

        // controllo se c'è almeno una tana che non è stata chiusa
        for (i = ZERO; i < NUMERO_TANE; i++)
        {
            if (arrayTane[i] == false)
            {
                buffer = true;
                break;
            }
        }

        /* uscita dal gioco nel caso in cui viene premuta la q, finiscono le vite, finisce il tempo
        oppure vengono chiuse tutte le tane. Nel caso di vittoria o sconfitta
        viene stampata una schermata finale diversa */
        if (ranocchio.id == q || vite == ZERO || tempo <= ZERO || buffer == false)
        {
            clear();
            if (vite == ZERO)
                gameOver();
            else if (buffer == false)
                vittoria(punteggio);

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
        buffer = false;
    }
}
