
#include "lib.h"
#include "funzioniGenerali.h"
#include "rana.h"
#include "autostrada.h"
#include "fiume.h"
#include "avvio.h"
#include "collisioni.h"
#include "funzioniMain.h"
#include "areaDisegno.h"
#include "areaGioco.h"

bool areaGioco(Avvio info)
{
    srand(time(NULL));

    int maxx, maxy;
    int differenza;
    int risultato;
    int direzione;
    int contaNemici;

    int troncoNemico;
    int offset;
    int vite;

    bool buffer = false;
    bool riniziaPartita = false;
    bool pausa = false;
    bool partitaFinita;

    char nomeUtente[MAX_NOMI];
    bool coloreTroncoRana = false;
    bool coloreTroncoProiettile = false;
    bool coloreNemicoProiettile = false;
    bool sulTronco = false;
    bool partitaInCorso;

    // variabili utilizzate per far spawnare i nemici e farli sparare al momento giusto
    time_t inizio_nemico, fine_nemico, inizio_proiettile[MAX_TRONCHI], fine_proiettile;

    Oggetto proiettileNemico[MAX_TRONCHI];
    Oggetto ranocchio;
    Oggetto vecchiaRana, vecchiaRana2;

    Oggetto tempo;

    int i, j;
    Oggetto pacchetto;
    Oggetto proiettilino[NUMERO_PROIETTILI];
    Coordinate nuoveCoordinate;

    Colore bufferColori;
    int timer;
    int punteggio;
    bool arrayTane[NUMERO_TANE] = {false};

    bool nemico[MAX_TRONCHI] = {false};

    Oggetto macchinina[MAX_MACCHINE];
    Oggetto camioncino[MAX_CAMION];
    Oggetto tronchino[MAX_TRONCHI];
    WINDOW *finestraGioco;

    bool hitProiettile[MAX_TRONCHI];

    // inizializzo le variabili
    offset = 0;
    riniziaPartita = false;
    timer = TEMPO_INIZIALE;
    punteggio = PUNTEGGIO_INIZIALE;

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

    // musica di sottofondo
    if (info.audio)
        system("ffplay -nodisp ../file_audio/frogger.mp3 2> /dev/null & ");

    ranocchio.coordinate.x = ZERO;
    ranocchio.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + info.difficolta * NUMERO_CORSIE * 2;

    for (i = 0; i < MAX_TRONCHI; i++)
        hitProiettile[i] = false;

    // creo la finestra in base alla difficoltà scelta dall'utente
    // quindi se verrà impostata la difficoltà facile si avranno 3 corsie di fiumi e di strada
    // difficoltà media 4 corsie di fiumi e di strada ecc...
    finestraGioco = newwin(ALTEZZA_SCHERMO + (info.difficolta * NUMERO_CORSIE * 2),
                           LARGHEZZA_SCHERMO - 1, INIZIO_ALTEZZA_FINESTRA, INIZIO_LARGHEZZA_FINESTRA);

    vite = MAX_VITE - info.difficolta;

    clear();

    for (i = ZERO; i < NUMERO_MACCHINE + 3 * info.difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_MACCHINA0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }
    for (i = ZERO; i < NUMERO_CAMION + 3 * info.difficolta; i++)
    {
        bufferColori = coloreVeicolo();
        init_color(COLORE_CAMION0 + i, bufferColori.r, bufferColori.g, bufferColori.b);
    }

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(finestraGioco, vite);

    // stampo le varie sezioni dello schermo
    funzMarciapiede(finestraGioco, info.difficolta);
    funzAutostrada(finestraGioco, info.difficolta);
    funzPrato(finestraGioco, info.difficolta);
    funzFiume(finestraGioco, info.difficolta);
    stampaRana(finestraGioco, ranocchio.coordinate, coloreTroncoRana, info.difficolta);

    // creo tutti i processi della rana, dei veicoli, dei tronchi e del tempo
    funzRana(p, pRana, info.difficolta);
    funzAuto(p, info.difficolta);
    funzTronchi(p, info.difficolta);
    funzTempo(p);

    // prima di iniziare porto tutti gli oggetti fuori dallo schermo
    for (i = ZERO; i < MAX_TRONCHI; i++)
    {

        tronchino[i].coordinate.x = FUORI_MAPPA;
        tronchino[i].coordinate.y = FUORI_MAPPA;

        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
    }

    for (i = ZERO; i < MAX_CAMION; i++)
    {

        camioncino[i].coordinate.x = FUORI_MAPPA;
        camioncino[i].coordinate.y = FUORI_MAPPA;
    }

    for (i = 0; i < MAX_MACCHINE; i++)
    {
        macchinina[i].coordinate.x = FUORI_MAPPA;
        macchinina[i].coordinate.y = FUORI_MAPPA;
    }
    for (i = 0; i < NUMERO_PROIETTILI; i++)
    {
        proiettilino[i].coordinate.x = FUORI_MAPPA - 1;
        proiettilino[i].coordinate.y = FUORI_MAPPA - 1;
    }

    time(&inizio_nemico);

    close(pRana[READ]);
    partitaInCorso = true;
    /* Leggo i dati della pipe p dove vengono scritte le posizioni di tutti gli oggetti.
    Quello che viene letto dalla pipe viene salvato in una variabile provvisoria e,
    dopo essere stato riconosciuto tramite il suo id, viene salvato nella variabile dell'oggetto
    corrispondente. Viene anche letto il tempo aggiornato in secondi. */
    while (partitaInCorso)
    {
        partitaFinita = false;

        refresh();
        read(p[READ], &pacchetto, sizeof(Oggetto));

        if (pacchetto.id == RANA)
        {
            ranocchio = pacchetto;
            vecchiaRana = pacchetto;
        }

        else if (pacchetto.id >= PROIETTILE0 && pacchetto.id <= PROIETTILE29)
            proiettilino[pacchetto.id - PROIETTILE0] = pacchetto;

        else if (pacchetto.id >= PROIETTILE0_OUT && pacchetto.id <= PROIETTILE29_OUT)
        {
            proiettilino[pacchetto.id - PROIETTILE0_OUT].coordinate.x = FUORI_MAPPA;
            proiettilino[pacchetto.id - PROIETTILE0_OUT].coordinate.y = FUORI_MAPPA;
        }

        else if (pacchetto.id >= PROIETTILE_NEMICO0 && pacchetto.id <= PROIETTILE_NEMICO2)
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0] = pacchetto;

        else if (pacchetto.id >= PROIETTILE_NEMICO0_OUT && pacchetto.id <= PROIETTILE_NEMICO2_OUT)
        {
            hitProiettile[pacchetto.id - PROIETTILE_NEMICO0_OUT] = false;
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0_OUT].coordinate.x = FUORI_MAPPA;
            proiettileNemico[pacchetto.id - PROIETTILE_NEMICO0_OUT].coordinate.y = FUORI_MAPPA;
        }

        else if (pacchetto.id >= TRONCO0 && pacchetto.id <= TRONCO4)
            tronchino[pacchetto.id - TRONCO0] = pacchetto;

        else if (pacchetto.id >= MACCHINA0 && pacchetto.id <= MACCHINA10)
            macchinina[pacchetto.id - MACCHINA0] = pacchetto;

        else if (pacchetto.id >= CAMION0 && pacchetto.id <= CAMION8)
            camioncino[pacchetto.id - CAMION0] = pacchetto;

        else if (pacchetto.id == q)
            ranocchio = pacchetto;

        else if (pacchetto.id == SPAWN_PROIETTILE)
            creaProiettile(p, ranocchio, &offset);

        else if (pacchetto.id == PAUSA)
            partitaFinita = funzPausa(finestraGioco, info.difficolta, camioncino, tronchino, macchinina, tempo.pid, ranocchio.pid);
        else if (pacchetto.id == TEMPO)
            tempo = pacchetto;

        if (!partitaFinita)
        {

            // controllo se la rana è entrata in una tana
            risultato = controlloLimitiRana(ranocchio.coordinate, info.difficolta);

            // se la rana è entrata in una tana viene portata alla posizione iniziale viene aggiornato il punteggio
            if (risultato < SEI && risultato >= ZERO)
            {
                if (!(arrayTane[risultato] == true))
                {
                    arrayTane[risultato] = true; // viene chiusa la tana
                    ranocchio = posizioneInizialeRana(pRana, ranocchio, info.difficolta);
                    punteggio += PUNTEGGIO_TANA;
                }
            }

            // stampa dei colori di background
            funzMarciapiede(finestraGioco, info.difficolta);
            funzAutostrada(finestraGioco, info.difficolta);
            funzPrato(finestraGioco, info.difficolta);
            funzFiume(finestraGioco, info.difficolta);

            // stampa la tane e ne tiene controllo
            funzTane(finestraGioco, arrayTane);

            coloreTroncoRana = false;
            time(&fine_nemico);

            /* controllo se it tempo di attesa dello spawn del nemico è finito e se è
            così viene stampato in un tronco che non è occupato dalla rana o da un altro nemico */
            if ((fine_nemico - inizio_nemico) >= TEMPO_SPAWN_NEMICO - info.difficolta)
            {
                contaNemici = 0;
                for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
                {
                    if (nemico[i])
                        contaNemici++;
                }
                if (!(contaNemici == (NUMERO_NEMICI + info.difficolta) || contaNemici == (NUMERO_NEMICI + info.difficolta - 1) && sulTronco))
                {
                    do
                    {
                        troncoNemico = rand() % (NUMERO_TRONCHI + info.difficolta);
                    } while (tronchino[troncoNemico].coordinate.y == ranocchio.coordinate.y ||
                             nemico[troncoNemico] == true);

                    nemico[troncoNemico] = true;
                    time(&inizio_nemico);
                    time(&inizio_proiettile[troncoNemico]);
                }
            }

            // ciclo macchine

            /* ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
             con le varie macchine o se la rana è presente sul tronco */
            for (i = ZERO; i < NUMERO_MACCHINE + info.difficolta; i++)
            {
                // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana

                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (proiettiliVeicoli(proiettilino[j], proiettileNemico, macchinina[i], LARGHEZZA_MACCHINA, hitProiettile))
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                }
                stampaMacchina(finestraGioco, macchinina[i], i);

                // controllo se la rana è dentro il range dello sprite della macchina
                // allora la porto alla alla posizione di partenza e tolgo una vita
                if (controlloCollisioneOggetti(macchinina[i], ranocchio.coordinate, LARGHEZZA_MACCHINA))
                {
                    ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
            }

            // ciclo camion
            for (i = 0; i < NUMERO_CAMION + info.difficolta; i++)
            {
                // controllo se i proiettili collidono con un camion e nel caso li distruggo
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (proiettiliVeicoli(proiettilino[j], proiettileNemico, camioncino[i], LARGHEZZA_CAMION, hitProiettile))
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                }
                // stampo i camion
                stampaCamion(finestraGioco, camioncino[i], i);

                // controllo la collisione della rana con l'array dei camion e se è dentro la sprite di un camion
                // tolgo una vita e resetto la sua posizione
                if (controlloCollisioneOggetti(camioncino[i], ranocchio.coordinate, LARGHEZZA_CAMION))
                {
                    ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
            }

            // ciclo proiettili nemici
            for (i = 0; i < MAX_PROIETTILI_NEMICI; i++)
            {

                /* per ognuno dei proiettili nemici controllo che non collida con il proiettile della rana,
                e nel caso li distruggo entrambi */
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (controlloCollisioniProiettili(proiettilino[j].coordinate, proiettileNemico[i].coordinate))
                    {
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                        hitProiettile[i] = true;
                    }
                }

                // controllo se un proiettile nemico collide con la rana e nel caso lo distruggo e tolgo una vita
                if (controlloCollisioniRanaProiettile(proiettileNemico[i].coordinate, ranocchio.coordinate) &&
                    hitProiettile[i] == false)
                {
                    ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);
                    kill(proiettileNemico[i].pid, SIGKILL);
                    hitProiettile[i] = true;
                }
            }

            // ciclo tronchi
            for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
            {

                /* controllo la collisione del proiettile con i tronchi e se è presente un nemico lo uccido
                           e distruggo il proiettile */

                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (controlloCollisioneNemicoProiettile(proiettilino[j], tronchino[i], nemico[i]))
                    {
                        nemico[i] = false;
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                        punteggio += PUNTEGGIO_UCCISIONE;
                    }
                }

                if (vecchiaRana.coordinate.x != vecchiaRana2.coordinate.x || vecchiaRana.coordinate.y != vecchiaRana2.coordinate.y)
                {
                    vecchiaRana2 = vecchiaRana;
                    sulTronco = false;
                }

                // controllo se la rana è salita sul tronco
                if (tronchino[i].coordinate.x <= ranocchio.coordinate.x &&
                    (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= ranocchio.coordinate.x &&
                    ranocchio.coordinate.y == tronchino[i].coordinate.y)
                {

                    // se è presente un nemico sul tronco allora la rana muore
                    if (nemico[i])
                        ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);

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
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_in_acqua.mp3 2> /dev/null & ");
                    sulTronco = false;
                    ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);
                }

                /* se il nemico non è morto lo stampo sul tronco e se è pronto a sparare il proiettile
                ne creo il processo */
                if (nemico[i])
                {
                    stampaNemico(finestraGioco, tronchino[i].coordinate);

                    time(&fine_proiettile);
                    if ((difftime(fine_proiettile, inizio_proiettile[i])) >= DUE)
                    {
                        time(&inizio_proiettile[i]);
                        funzProiettileNemico(tronchino[i].coordinate, p, i, info.difficolta);
                    }
                }
                // altrimenti, se non è presente un nemico stampo solamente il tronco
                else
                    stampaTronco(finestraGioco, tronchino[i].coordinate);

                // mi prendo i pid dei tronchi per poi utilizzare questo array per
                // killare correttamente i vari processi tronchi
            }
            // la rana  prova a entrare in una tana già chiusa
            if (ranocchio.coordinate.y == INIZIO_TANE)
                ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);

            stampaRana(finestraGioco, ranocchio.coordinate, coloreTroncoRana, info.difficolta);

            stampaVite(finestraGioco, vite);

            // controllo se i proiettili nemici sono in movimento e nel caso li stampo
            for (i = 0; i < NUMERO_PROIETTILI; i++)
                stampaProiettili(finestraGioco, tronchino, nemico, proiettilino[i].coordinate, info.difficolta);

            for (i = ZERO; i < NUMERO_NEMICI + info.difficolta; i++)
            {
                if (!hitProiettile[i])
                    stampaProiettili(finestraGioco, tronchino, nemico, proiettileNemico[i].coordinate, info.difficolta);
            }

            // stampo il punteggio  e il tempo rimanente nella parte alta dello schermo
            stampaPunteggio(finestraGioco, punteggio);
            stampaTempo(finestraGioco, timer);

            if (tempo.velocita)
            {
                timer--;
                punteggio -= DIECI;
            }

            wrefresh(finestraGioco);
            tempo.velocita = 0;

            // controllo se c'è almeno una tana che non è stata chiusa
            buffer = controlloTaneChiuse;

            if (timer <= 0)
                ranocchio = morteRana(finestraGioco, &vite, pRana, ranocchio, info.difficolta, &timer);
        }
        /* uscita dal gioco nel caso in cui viene premuta la q, finiscono le vite, finisce il tempo
        oppure vengono chiuse tutte le tane. Nel caso di vittoria o sconfitta
        viene stampata una schermata finale diversa */
        riniziaPartita = finePartita(finestraGioco, ranocchio, vite, buffer, punteggio, info.difficolta, tempo, macchinina, camioncino, tronchino, &partitaInCorso, partitaFinita);
        buffer = false;

       
    }
     return riniziaPartita;
}