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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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

    int corsiaRandom;

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
    pthread_t threadRana, threadProiettile[NUMERO_PROIETTILI],
        threadTronchi[MAX_TRONCHI], threadMacchine[MAX_MACCHINE], threadCamion[MAX_CAMION], threadTempo;

    // variabili utilizzate per far spawnare i nemici e farli sparare al momento giusto
    time_t inizio_nemico, fine_nemico, inizio_proiettile[MAX_TRONCHI], fine_proiettile;

    Oggetto proiettileNemico[MAX_TRONCHI];
    Oggetto rana;
    Oggetto vecchiaRana, vecchiaRana2;

    Oggetto tempo;

    int i, j;
    Oggetto pacchetto;
    Oggetto proiettilino[NUMERO_PROIETTILI];
    Coordinate nuoveCoordinate;
    int macchineInCoda = 0;

    Colore bufferColori;
    int timer;
    int punteggio;
    bool arrayTane[NUMERO_TANE] = {false};

    bool nemico[MAX_TRONCHI] = {false};

    parametriVeicolo macchina[MAX_MACCHINE];
    parametriVeicolo camion[MAX_CAMION];
    Oggetto tronco[MAX_TRONCHI];
    WINDOW *finestraGioco;

    bool hitProiettile[MAX_TRONCHI];

    // inizializzo le variabili
    offset = 0;
    riniziaPartita = false;
    timer = TEMPO_INIZIALE;
    punteggio = PUNTEGGIO_INIZIALE;

    // musica di sottofondo
    if (info.audio)
        system("ffplay -nodisp ../file_audio/frogger.mp3 2> /dev/null & ");

    rana.coordinate.x = ZERO;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + info.difficolta * NUMERO_CORSIE * 2;
    rana.difficolta = info.difficolta;

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
    stampaRana(finestraGioco, rana.coordinate, coloreTroncoRana, info.difficolta);

    // pthread_create(threadTempo,NULL,&movimentoRana,&rana);
    // funzRana(info.difficolta);
    // funzAuto(info.difficolta);
    // funzTronchi(info.difficolta);
    // funzTempo();

    // prima di iniziare porto tutti gli oggetti fuori dallo schermo
    /* for (i = ZERO; i < MAX_TRONCHI; i++)
     {
         tronco[i].coordinate.x = FUORI_MAPPA;
         tronco[i].coordinate.y = FUORI_MAPPA;

         proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
         proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
     }

     for (i = ZERO; i < MAX_CAMION; i++)
     {

         camion[i].coordinate.x = FUORI_MAPPA;
         camion[i].coordinate.y = FUORI_MAPPA;
     }

     for (i = 0; i < MAX_MACCHINE; i++)
     {
         macchina[i].coordinate.x = FUORI_MAPPA;
         macchina[i].coordinate.y = FUORI_MAPPA;
     }
     for (i = 0; i < NUMERO_PROIETTILI; i++)
     {
         proiettilino[i].coordinate.x = FUORI_MAPPA - 1;
         proiettilino[i].coordinate.y = FUORI_MAPPA - 1;
     }

     */
    // creo tutti i processi della rana, dei veicoli, dei tronchi e del tempo
    int velocita[MAX_CORSIE];
    int spostamento;
    for (i = ZERO; i < NUMERO_TRONCHI + info.difficolta; i++)
        velocita[i] = UNO;

    spostamento = rand() % DUE;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    pthread_create(&threadRana, NULL, &movimentoRana, &rana);
    for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
    {
        tronco[i].velocita = velocita[i] * spostamento * -1;
        tronco[i].id = i;
        tronco[i].difficolta = info.difficolta;
        pthread_create(&threadTronchi[i], NULL, &movimentoTronco, &tronco[i]);
    }

    // randomizzo anche la direzione
    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;

    int velocitaCorsie[MAX_CORSIE];
    int direzioneCorsie[MAX_CORSIE];
    Coordinate inizioVeicoli[MAX_CAMION + MAX_MACCHINE];
    direzione = spostamento;
    j = 0;
    for (i = 0; i < NUMERO_MACCHINE + NUMERO_CAMION + (info.difficolta * 2); i++)
    {
        do
        {
            inizioVeicoli[i].x = rand() % LARGHEZZA_SCHERMO;
            inizioVeicoli[i].y = rand() % (NUMERO_CORSIE + info.difficolta);
        } while (controlloInizioCoordinateCorsie(inizioVeicoli, i));

        if (i < (NUMERO_MACCHINE + info.difficolta))
        {
            macchina[i].veicolo.coordinate.x = inizioVeicoli[i].x;
            macchina[i].veicolo.coordinate.y = INIZIO_AUTOSTRADA + inizioVeicoli[i].y * 3 + info.difficolta * 3;
        }
        else
        {
            camion[j].veicolo.coordinate.x = inizioVeicoli[i].x;
            camion[j].veicolo.coordinate.y = INIZIO_AUTOSTRADA + inizioVeicoli[i].y * 3 + info.difficolta * 3;
            j++;
        }
    }
    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
    {
        direzioneCorsie[i] = direzione;
        direzione = direzione * -1;
        ;
    }

    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI)) - 2500 * info.difficolta;
    for (i = 0; i < NUMERO_MACCHINE + info.difficolta; i++)
    {
        macchina[i].direzioneCorsia = direzioneCorsie[i % (NUMERO_CORSIE + info.difficolta)];
        macchina[i].velocitaCorsia = velocitaCorsie[i % (NUMERO_CORSIE + info.difficolta)];
        macchina[i].veicolo.difficolta = info.difficolta;
        macchina[i].veicolo.id = i;
        pthread_create(&threadMacchine[i], NULL, &movimentoVeicolo, &macchina[i]);
    }

    pthread_create(&threadTempo, NULL, &orologio, &tempo);

    time(&inizio_nemico);

    partitaInCorso = true;

    while (partitaInCorso)
    {
        partitaFinita = false;
        // ricevo partitaFinita
        if (!partitaFinita)
        {
            risultato = controlloLimitiRana(rana.coordinate, info.difficolta);

            // se la rana è entrata in una tana viene portata alla posizione iniziale viene aggiornato il punteggio
            if (risultato < SEI && risultato >= ZERO)
            {
                if (!(arrayTane[risultato] == true))
                {
                    arrayTane[risultato] = true; // viene chiusa la tana
                    pthread_mutex_lock(&mutex);
                    rana = posizioneInizialeRana(rana, info.difficolta);
                    pthread_mutex_unlock(&mutex);

                    punteggio += PUNTEGGIO_TANA;
                }
            }

            funzMarciapiede(finestraGioco, info.difficolta);
            funzAutostrada(finestraGioco, info.difficolta);
            funzPrato(finestraGioco, info.difficolta);
            funzFiume(finestraGioco, info.difficolta);

            funzTane(finestraGioco, arrayTane);

            coloreTroncoRana = false;
            time(&fine_nemico);

            stampaTempo(finestraGioco, tempo.velocita);
            stampaPunteggio(finestraGioco, punteggio);

            // ciclo tronchi
            for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
            {

                /* controllo la collisione del proiettile con i tronchi e se è presente un nemico lo uccido
                           e distruggo il proiettile*/

                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (controlloCollisioneNemicoProiettile(proiettilino[j], tronco[i], nemico[i]))
                    {
                        nemico[i] = false;
                        pthread_mutex_lock(&mutex);
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                        pthread_mutex_unlock(&mutex);
                        punteggio += PUNTEGGIO_UCCISIONE;
                    }
                }

                if (vecchiaRana.coordinate.x != vecchiaRana2.coordinate.x || vecchiaRana.coordinate.y != vecchiaRana2.coordinate.y)
                {
                    vecchiaRana2 = vecchiaRana;
                    sulTronco = false;
                }

                // controllo se la rana è salita sul tronco
                if (tronco[i].coordinate.x <= rana.coordinate.x &&
                    (tronco[i].coordinate.x + LARGHEZZA_TRONCHI) >= rana.coordinate.x &&
                    rana.coordinate.y == tronco[i].coordinate.y)
                {

                    // se è presente un nemico sul tronco allora la rana muore
                    if (nemico[i])
                    {
                        pthread_mutex_lock(&mutex);
                        rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                        pthread_mutex_unlock(&mutex);
                    }

                    else
                    {
                        // appena salita la rana mi calcolo la differenza tra l'inizio del tronco e la posizione della rana
                        // così facendo la rana salirà sul tronco nel punto esatto
                        if (!sulTronco)
                        {
                            differenza = rana.coordinate.x - tronco[i].coordinate.x;
                            sulTronco = true;
                        }
                        pthread_mutex_lock(&mutex);
                        rana.coordinate.x = tronco[i].coordinate.x + differenza;
                        pthread_mutex_unlock(&mutex);
                        coloreTroncoRana = true;
                    }
                }

                // in questo casa la rana è sull'altezza del tronco però una x diversa dal range di esso
                // pertanto non è riuscita a salire sul tronco successivo quindi comunico le coordinate di partenza
                // e tolgo una vita
                else if (rana.coordinate.y == tronco[i].coordinate.y)
                {
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_in_acqua.mp3 2> /dev/null & ");
                    sulTronco = false;
                    pthread_mutex_lock(&mutex);
                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                    pthread_mutex_unlock(&mutex);
                }

                /* se il nemico non è morto lo stampo sul tronco e se è pronto a sparare il proiettile
                ne creo il processo*/
                if (nemico[i])
                {
                    stampaNemico(finestraGioco, tronco[i].coordinate);

                    time(&fine_proiettile);
                    if ((difftime(fine_proiettile, inizio_proiettile[i])) >= DUE)
                    {
                        time(&inizio_proiettile[i]);
                        // funzProiettileNemico(tronco[i].coordinate, i, info.difficolta);
                    }
                }
                // altrimenti, se non è presente un nemico stampo solamente il tronco
                else
                    stampaTronco(finestraGioco, tronco[i].coordinate);

                // mi prendo i pid dei tronchi per poi utilizzare questo array per
                // killare correttamente i vari processi tronchi
            }

            punteggio = PUNTEGGIO_INIZIALE + (tempo.velocita * 10) - TEMPO_INIZIALE * 10;

            // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
            for (i = 0; i < NUMERO_MACCHINE + info.difficolta; i++)
            {
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (proiettiliVeicoli(proiettilino[j], proiettileNemico, macchina[i].veicolo, LARGHEZZA_MACCHINA, hitProiettile))
                        proiettilino[j] = uccidiProiettile(proiettilino[j]);
                }
                stampaMacchina(finestraGioco, macchina[i].veicolo, i);

                // controllo se la rana è dentro il range dello sprite della macchina
                // allora la porto alla alla posizione di partenza e tolgo una vita
                if (controlloCollisioneOggetti(macchina[i].veicolo, rana.coordinate, LARGHEZZA_MACCHINA))
                {
                    pthread_mutex_lock(&mutex);
                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                    pthread_mutex_unlock(&mutex);
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
            }
            // stampo il punteggio  e il tempo rimanente nella parte alta dello schermo
            stampaPunteggio(finestraGioco, punteggio);
            stampaTempo(finestraGioco, timer);

            // controllo se c'è almeno una tana che non è stata chiusa
            // buffer = controlloTaneChiuse(arrayTane);

            if (tempo.velocita <= 0)
            {
                pthread_mutex_lock(&mutex);
                rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                pthread_mutex_unlock(&mutex);
            }

            stampaRana(finestraGioco, rana.coordinate, coloreTroncoRana, info.difficolta);

            wrefresh(finestraGioco);
        }
        buffer = false;
    }
    /* Leggo i dati della pipe p dove vengono scritte le posizioni di tutti gli oggetti.
    Quello che viene letto dalla pipe viene salvato in una variabile provvisoria e,
    dopo essere stato riconosciuto tramite il suo id, viene salvato nella variabile dell'oggetto
    corrispondente. Viene anche letto il tempo aggiornato in secondi.
    while (partitaInCorso)
    {
        partitaFinita = false;

        refresh();

        if (!partitaFinita)
        {



            // stampa dei colori di background
            funzMarciapiede(finestraGioco, info.difficolta);
            funzAutostrada(finestraGioco, info.difficolta);
            funzPrato(finestraGioco, info.difficolta);
            funzFiume(finestraGioco, info.difficolta);

            // stampa la tane e ne tiene controllo

            coloreTroncoRana = false;
            time(&fine_nemico);

            /* controllo se it tempo di attesa dello spawn del nemico è finito e se è
            così viene stampato in un tronco che non è occupato dalla rana o da un altro nemico
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
                    } while (tronco[troncoNemico].coordinate.y == rana.coordinate.y ||
                             nemico[troncoNemico] == true);

                    nemico[troncoNemico] = true;
                    time(&inizio_nemico);
                    time(&inizio_proiettile[troncoNemico]);
                }
            }

            // ciclo macchine

*/
    /* ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
     con le varie macchine o se la rana è presente sul tronco */

    /*
                // ciclo camion
                for (i = 0; i < NUMERO_CAMION + info.difficolta; i++)
                {
                    // controllo se i proiettili collidono con un camion e nel caso li distruggo
                    for (j = 0; j < NUMERO_PROIETTILI; j++)
                    {
                        if (proiettiliVeicoli(proiettilino[j], proiettileNemico, camion[i], LARGHEZZA_CAMION, hitProiettile))
                            proiettilino[j] = uccidiProiettile(proiettilino[j]);
                    }
                    // stampo i camion
                    stampaCamion(finestraGioco, camion[i], i);

                    // controllo la collisione della rana con l'array dei camion e se è dentro la sprite di un camion
                    // tolgo una vita e resetto la sua posizione
                    if (controlloCollisioneOggetti(camion[i], rana.coordinate, LARGHEZZA_CAMION))
                    {
                        rana = morteRana(finestraGioco, &vite, pRana, rana, info.difficolta, &timer);
                        if (info.audio)
                            system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                    }
                }

                // ciclo proiettili nemici
                for (i = 0; i < MAX_PROIETTILI_NEMICI; i++)
                {

                    /* per ognuno dei proiettili nemici controllo che non collida con il proiettile della rana,
                    e nel caso li distruggo entrambi
                    for (j = 0; j < NUMERO_PROIETTILI; j++)
                    {
                        if (controlloCollisioniProiettili(proiettilino[j].coordinate, proiettileNemico[i].coordinate))
                        {
                            proiettilino[j] = uccidiProiettile(proiettilino[j]);
                            hitProiettile[i] = true;
                        }
                    }

                    // controllo se un proiettile nemico collide con la rana e nel caso lo distruggo e tolgo una vita
                    if (controlloCollisioniRanaProiettile(proiettileNemico[i].coordinate, rana.coordinate) &&
                        hitProiettile[i] == false)
                    {
                        rana = morteRana(finestraGioco, &vite, pRana, rana, info.difficolta, &timer);
                        kill(proiettileNemico[i].pid, SIGKILL);
                        hitProiettile[i] = true;
                    }
                }


                // la rana  prova a entrare in una tana già chiusa
                if (rana.coordinate.y == INIZIO_TANE)
                    rana = morteRana(finestraGioco, &vite, pRana, rana, info.difficolta, &timer);

                stampaRana(finestraGioco, rana.coordinate, coloreTroncoRana, info.difficolta);

                stampaVite(finestraGioco, vite);

                // controllo se i proiettili nemici sono in movimento e nel caso li stampo
                for (i = 0; i < NUMERO_PROIETTILI; i++)
                    stampaProiettili(finestraGioco, tronco, nemico, proiettilino[i].coordinate, info.difficolta);

                for (i = ZERO; i < NUMERO_NEMICI + info.difficolta; i++)
                {
                    if (!hitProiettile[i])
                        stampaProiettili(finestraGioco, tronco, nemico, proiettileNemico[i].coordinate, info.difficolta);
                }

                // stampo il punteggio  e il tempo rimanente nella parte alta dello schermo
                stampaPunteggio(finestraGioco, punteggio);
                stampaTempo(finestraGioco, timer);


                wrefresh(finestraGioco);
                tempo.velocita = 0;

                // controllo se c'è almeno una tana che non è stata chiusa
                buffer = controlloTaneChiuse;

                if (timer <= 0)
                    rana = morteRana(finestraGioco, &vite, pRana, rana, info.difficolta, &timer);
            }
            /* uscita dal gioco nel caso in cui viene premuta la q, finiscono le vite, finisce il tempo
            oppure vengono chiuse tutte le tane. Nel caso di vittoria o sconfitta
            viene stampata una schermata finale diversa
            riniziaPartita = finePartita(finestraGioco, rana, vite, buffer, punteggio, info.difficolta, tempo, macchina, camion, tronco, &partitaInCorso, partitaFinita);
            buffer = false;
        }
        return riniziaPartita;
        */
}

bool CorsiaOccupata(Oggetto macchina[], Oggetto camion[], int corsia, int difficolta)
{
    bool flag = false;
    int i;

    for (i = 0; i < NUMERO_MACCHINE + difficolta; i++)
    {
        if (macchina[i].velocita < ZERO)
        {
            if (macchina[i].coordinate.x >= LARGHEZZA_SCHERMO && macchina[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
            {
                flag = true;
                break;
            }
        }
        else
        {
            if (macchina[i].coordinate.x <= 0 && macchina[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
            {
                flag = true;
                break;
            }
        }
    }
    if (!flag)
    {
        for (i = 0; i < NUMERO_CAMION + difficolta; i++)
        {
            if (camion[i].velocita < ZERO)
            {
                if (camion[i].coordinate.x >= LARGHEZZA_SCHERMO && camion[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
                {
                    flag = true;
                    break;
                }
            }
            else
            {
                if (camion[i].coordinate.x <= 0 && camion[i].coordinate.y == (INIZIO_AUTOSTRADA + corsia * 3 + difficolta * 3))
                {
                    flag = true;
                    break;
                }
            }
        }
    }

    return flag;
}