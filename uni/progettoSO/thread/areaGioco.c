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

    int i, j, k = 0;

    int differenza, risultato, direzione, contaNemici;

    int troncoNemico, vite;

    int corsiaRandom, corsia;

    bool buffer = false;
    bool pausa = false;
    bool partitaFinita;
    bool coloreTroncoRana = false;
    bool coloreTroncoProiettile = false;
    bool coloreNemicoProiettile = false;
    bool sulTronco = false;
    bool partitaInCorso;
    pthread_t threadRana, threadProiettile[NUMERO_PROIETTILI],
        threadTronchi[MAX_TRONCHI], threadMacchine[MAX_MACCHINE], threadCamion[MAX_CAMION], threadTempo, threadProiettileNemico[MAX_TRONCHI];

    // variabili utilizzate per far spawnare i nemici e farli sparare al momento giusto
    time_t inizio_nemico, fine_nemico, inizio_proiettile[MAX_TRONCHI], fine_proiettile;

    Oggetto proiettileNemico[MAX_TRONCHI];
    Oggetto rana;
    Oggetto vecchiaRana;

    Oggetto tempo;

    Oggetto pacchetto;
    Proiettile proiettilino[NUMERO_PROIETTILI];
    Coordinate nuoveCoordinate;
    int macchineInCoda = 0;

    Colore bufferColori;
    bool arrayTane[NUMERO_TANE] = {false};

    bool nemico[MAX_TRONCHI] = {false};

    parametriVeicolo macchina[MAX_MACCHINE], camion[MAX_CAMION];
    Oggetto tronco[MAX_TRONCHI];
    WINDOW *finestraGioco;

    bool hitProiettile[MAX_TRONCHI];

    // inizializzo le variabili
    int offset = 0;
    bool riniziaPartita = false;
    int timer = TEMPO_INIZIALE;
    int punteggio = PUNTEGGIO_INIZIALE;

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
    refresh();

    // per ogni veicolo genero un colore casuale
    creaColoriRandom(info.difficolta);

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

    // prima di iniziare porto tutti gli oggetti fuori dallo schermo
    for (i = ZERO; i < MAX_TRONCHI; i++)
    {
        tronco[i].coordinate.x = FUORI_MAPPA;
        tronco[i].coordinate.y = FUORI_MAPPA;

        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 3;
        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 3;
    }

    for (i = ZERO; i < MAX_CAMION; i++)
    {

        camion[i].veicolo.coordinate.x = FUORI_MAPPA;
        camion[i].veicolo.coordinate.y = FUORI_MAPPA;
    }

    for (i = 0; i < MAX_MACCHINE; i++)
    {
        macchina[i].veicolo.coordinate.x = FUORI_MAPPA;
        macchina[i].veicolo.coordinate.y = FUORI_MAPPA;
    }
    for (i = 0; i < NUMERO_PROIETTILI; i++)
    {
        proiettilino[i].proiettile.coordinate.x = FUORI_MAPPA - 1;
        proiettilino[i].proiettile.coordinate.y = FUORI_MAPPA - 1;
    }

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
        pthread_create(&threadProiettileNemico[i], NULL, &movimentoProiettileNemico, &proiettileNemico[i]);
    }

    // randomizzo anche la direzione
    spostamento = rand() % DUE;

    if (spostamento == ZERO)
        spostamento = -UNO;
    else
        spostamento = UNO;

    int velocitaCorsie[MAX_CORSIE];
    int direzioneCorsie[MAX_CORSIE];
    j = 0;

    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
    {
        direzioneCorsie[i] = spostamento;
        spostamento *= -1;
    }

    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI)) - 2500 * info.difficolta;

    for (i = 0; i < NUMERO_MACCHINE + info.difficolta; i++)
    {
        pthread_mutex_lock(&mutex);
        corsia = rand() % (NUMERO_CORSIE + info.difficolta);
        macchina[i].veicolo.coordinate.y = INIZIO_AUTOSTRADA + info.difficolta * 3 + corsia * 3;
        macchina[i].direzioneCorsia = direzioneCorsie[corsia];
        macchina[i].velocitaCorsia = velocitaCorsie[corsia];
        if (direzioneCorsie[corsia] < 0)
            macchina[i].veicolo.coordinate.x = -LARGHEZZA_CAMION;
        else
            macchina[i].veicolo.coordinate.x = LARGHEZZA_CAMION + LARGHEZZA_SCHERMO;
        macchina[i].veicolo.difficolta = info.difficolta;
        macchina[i].veicolo.id = MACCHINA0;
        pthread_mutex_unlock(&mutex);
        pthread_create(&threadMacchine[i], NULL, &movimentoVeicolo, &macchina[i]);
    }
    for (i = 0; i < NUMERO_CAMION + info.difficolta; i++)
    {
        pthread_mutex_lock(&mutex);
        corsia = rand() % (NUMERO_CORSIE + info.difficolta);
        camion[i].veicolo.coordinate.y = INIZIO_AUTOSTRADA + info.difficolta * 3 + corsia * 3;
        camion[i].direzioneCorsia = direzioneCorsie[corsia];
        camion[i].velocitaCorsia = velocitaCorsie[corsia];
        if (direzioneCorsie[corsia] < 0)
            camion[i].veicolo.coordinate.x = -LARGHEZZA_CAMION;
        else
            camion[i].veicolo.coordinate.x = LARGHEZZA_CAMION + LARGHEZZA_SCHERMO;
        camion[i].veicolo.difficolta = info.difficolta;
        camion[i].veicolo.id = CAMION0;
        pthread_mutex_unlock(&mutex);
        pthread_create(&threadCamion[i], NULL, &movimentoVeicolo, &camion[i]);
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
            // controllo se la rana è entrata in una tana
            pthread_mutex_lock(&mutex);
            risultato = controlloLimitiRana(rana.coordinate, info.difficolta);
            pthread_mutex_unlock(&mutex);

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

            if (rana.id == SPAWN_PROIETTILE)
            {
                if (info.audio)
                    system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
                proiettilino[offset].rana = rana;
                pthread_create(&threadProiettile[offset], NULL, &funzProiettile, &proiettilino[offset]);

                pthread_mutex_lock(&mutex);
                rana.id = RANA;
                pthread_mutex_unlock(&mutex);

                offset++;
                if (offset == NUMERO_PROIETTILI)
                    offset = 0;
            }

            if (rana.id == PAUSA)
            {
                partitaFinita = (!pausaeNuovaPartita(finestraGioco, 1));
                pthread_mutex_lock(&mutex);
                rana.id = RANA;
                pthread_mutex_unlock(&mutex);
            }
            // stampa dei colori di background
            funzMarciapiede(finestraGioco, info.difficolta);
            funzAutostrada(finestraGioco, info.difficolta);
            funzPrato(finestraGioco, info.difficolta);
            funzFiume(finestraGioco, info.difficolta);

            funzTane(finestraGioco, arrayTane);

            coloreTroncoRana = false;
            time(&fine_nemico);
            pthread_mutex_lock(&mutex);
            stampaTempo(finestraGioco, timer);
            stampaPunteggio(finestraGioco, punteggio);
            pthread_mutex_unlock(&mutex);

            /* controllo se it tempo di attesa dello spawn del nemico è finito e se è
             così viene stampato in un tronco che non è occupato dalla rana o da un altro nemico*/
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
                    pthread_mutex_lock(&mutex);
                    do
                    {
                        troncoNemico = rand() % (NUMERO_TRONCHI + info.difficolta);
                    } while (tronco[troncoNemico].coordinate.y == rana.coordinate.y ||
                             nemico[troncoNemico] == true);
                    pthread_mutex_unlock(&mutex);
                    nemico[troncoNemico] = true;
                    time(&inizio_nemico);
                    time(&inizio_proiettile[troncoNemico]);
                }
            }

            // ciclo tronchi
            for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
            {

                /* controllo la collisione del proiettile con i tronchi e se è presente un nemico lo uccido
                           e distruggo il proiettile*/

                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    if (controlloCollisioneNemicoProiettile(proiettilino[j].proiettile, tronco[i], nemico[i]))
                    {
                        nemico[i] = false;
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                        punteggio += PUNTEGGIO_UCCISIONE;
                    }
                    pthread_mutex_unlock(&mutex);
                }
                pthread_mutex_lock(&mutex);
                if (vecchiaRana.coordinate.x != rana.coordinate.x || vecchiaRana.coordinate.y != rana.coordinate.y)
                {
                    vecchiaRana = rana;
                    sulTronco = false;
                }
                pthread_mutex_unlock(&mutex);

                // controllo se la rana è salita sul tronco
                pthread_mutex_lock(&mutex);
                if (tronco[i].coordinate.x <= rana.coordinate.x &&
                    (tronco[i].coordinate.x + LARGHEZZA_TRONCHI) >= rana.coordinate.x &&
                    rana.coordinate.y == tronco[i].coordinate.y)
                {

                    // se è presente un nemico sul tronco allora la rana muore
                    if (nemico[i])

                        rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);

                    else
                    {
                        // appena salita la rana mi calcolo la differenza tra l'inizio del tronco e la posizione della rana
                        // così facendo la rana salirà sul tronco nel punto esatto
                        if (!sulTronco)
                        {
                            differenza = rana.coordinate.x - tronco[i].coordinate.x;
                            sulTronco = true;
                        }
                        rana.coordinate.x = tronco[i].coordinate.x + differenza;

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

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                }
                pthread_mutex_unlock(&mutex);

                /* se il nemico non è morto lo stampo sul tronco e se è pronto a sparare il proiettile
                ne creo il processo*/
                if (nemico[i])
                {
                    pthread_mutex_lock(&mutex);
                    stampaNemico(finestraGioco, tronco[i].coordinate);
                    pthread_mutex_unlock(&mutex);
                    time(&fine_proiettile);
                    if (fine_proiettile - inizio_proiettile[i] >= 2)
                    {
                        time(&inizio_proiettile[i]);
                        system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
                        proiettileNemico[i].coordinate.x = tronco[i].coordinate.x + LARGHEZZA_TRONCHI / DUE;
                        proiettileNemico[i].coordinate.y = tronco[i].coordinate.y + ALTEZZA_CORSIE;
                        
                    }
                }
                // altrimenti, se non è presente un nemico stampo solamente il tronco
                else
                {
                    pthread_mutex_lock(&mutex);
                    stampaTronco(finestraGioco, tronco[i].coordinate);
                    pthread_mutex_unlock(&mutex);
                }
            }

            punteggio = PUNTEGGIO_INIZIALE + (timer * 10) - TEMPO_INIZIALE * 10;

            // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
            for (i = 0; i < NUMERO_MACCHINE + info.difficolta; i++)
            {

                // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    if (proiettiliVeicoli(proiettilino[j].proiettile, proiettileNemico, macchina[i].veicolo, LARGHEZZA_MACCHINA, hitProiettile, threadProiettileNemico, info.difficolta))
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                    pthread_mutex_unlock(&mutex);
                }
                pthread_mutex_lock(&mutex);
                stampaMacchina(finestraGioco, macchina[i].veicolo, i);

                // modifico nel thread l'id del veicolo e se viene modificato
                // vuol dire che il veicolo è uscito dalla corsia quindi
                // gli viene assegnata una nuova corsia con un spazio libero
                if (macchina[i].veicolo.id == MACCHINA0_OUT)
                {

                    j = 0;

                    corsia = k % (NUMERO_CORSIE + info.difficolta);
                    k++;
                    // in base alla direzione lo faccio partire da destra o da sinistra
                    if (direzioneCorsie[corsia] < 0)
                        macchina[i].veicolo.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
                    else
                        macchina[i].veicolo.coordinate.x = -LARGHEZZA_CAMION;
                    // gli assegno la direzione , la velocità e l'altezza scelta della corsia scelta
                    macchina[i].veicolo.coordinate.y = INIZIO_AUTOSTRADA + info.difficolta * 3 + corsia * 3;
                    macchina[i].veicolo.velocita = direzioneCorsie[corsia];
                    macchina[i].velocitaCorsia = velocitaCorsie[corsia];
                    macchina[i].veicolo.id = i;
                }

                // controllo se la rana è dentro il range dello sprite della macchina
                // allora la porto alla alla posizione di partenza e tolgo una vita
                if (controlloCollisioneOggetti(macchina[i].veicolo, rana.coordinate, LARGHEZZA_MACCHINA))
                {

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
                pthread_mutex_unlock(&mutex);
            }
            /* ciclo per assegnare i pid agli oggetti e successivamente controllo le collisioni
         con le varie macchine o se la rana è presente sul tronco */

            // ciclo camion
            for (i = 0; i < NUMERO_CAMION + info.difficolta; i++)
            {
                // controllo se i proiettili collidono con un camion e nel caso li distruggo
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    if (proiettiliVeicoli(proiettilino[j].proiettile, proiettileNemico, camion[i].veicolo, LARGHEZZA_CAMION, hitProiettile, threadProiettileNemico, info.difficolta))
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                    pthread_mutex_unlock(&mutex);
                }
                // stampo i camion
                pthread_mutex_lock(&mutex);
                stampaCamion(finestraGioco, camion[i].veicolo, i);

                // controllo la collisione della rana con l'array dei camion e se è dentro la sprite di un camion
                // tolgo una vita e resetto la sua posizione

                if (controlloCollisioneOggetti(camion[i].veicolo, rana.coordinate, LARGHEZZA_CAMION))
                {

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);

                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
                // modifico nel thread l'id del veicolo e se viene modificato
                // vuol dire che il veicolo è uscito dalla corsia quindi
                // gli viene assegnata una nuova corsia con un spazio libero
                if (camion[i].veicolo.id == MACCHINA0_OUT)
                {
                    corsia = k % (NUMERO_CORSIE + info.difficolta);
                    k++;

                    if (direzioneCorsie[corsia] < 0)
                        camion[i].veicolo.coordinate.x = LARGHEZZA_SCHERMO + LARGHEZZA_CAMION;
                    else
                        camion[i].veicolo.coordinate.x = -LARGHEZZA_CAMION;
                    camion[i].veicolo.coordinate.y = INIZIO_AUTOSTRADA + info.difficolta * 3 + corsia * 3;
                    camion[i].veicolo.velocita = direzioneCorsie[corsia];
                    camion[i].velocitaCorsia = velocitaCorsie[corsia];
                    camion[i].veicolo.id = i;
                }
                pthread_mutex_unlock(&mutex);
            }

            // ciclo proiettili nemici
            for (i = 0; i < MAX_PROIETTILI_NEMICI; i++)
            {

                pthread_mutex_lock(&mutex);
                /* per ognuno dei proiettili nemici controllo che non collida con il proiettile della rana,
                e nel caso li distruggo entrambi*/
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    if (controlloCollisioniProiettili(proiettilino[j].proiettile.coordinate, proiettileNemico[i].coordinate))
                    {
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                        hitProiettile[i] = true;
                        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
                        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
                    }
                }

                // controllo se un proiettile nemico collide con la rana e nel caso lo distruggo e tolgo una vita
                if (controlloCollisioniRanaProiettile(proiettileNemico[i].coordinate, rana.coordinate) &&
                    hitProiettile[i] == false)
                {
                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                    proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
                    proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
                    hitProiettile[i] = true;
                }
                pthread_mutex_unlock(&mutex);
            }
            for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
            {
                pthread_mutex_lock(&mutex);
                if (proiettileNemico[i].coordinate.x == FUORI_MAPPA - 2)
                {
                    hitProiettile[i] = false;
                }
                pthread_mutex_unlock(&mutex);
            }
            // stampo i proiettili nemici
            for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
            { // se hanno colpito qualcosa non vengono printati
                pthread_mutex_lock(&mutex);
                if (!hitProiettile[i])
                    stampaProiettili(finestraGioco, tronco, nemico, proiettileNemico[i].coordinate, info.difficolta);
                pthread_mutex_unlock(&mutex);
            }

            // controllo se i proiettili nemici sono in movimento e nel caso li stampo
            for (i = 0; i < NUMERO_PROIETTILI; i++)
                stampaProiettili(finestraGioco, tronco, nemico, proiettilino[i].proiettile.coordinate, info.difficolta);

            // stampo il punteggio  e il tempo rimanente nella parte alta dello schermo
            stampaPunteggio(finestraGioco, punteggio);
            stampaTempo(finestraGioco, timer);
            stampaVite(finestraGioco, vite);

            pthread_mutex_lock(&mutex);
            // se nel thread viene modificata la variabile a 1 di tempo allora vuol dire che il
            //  tempo è sceso di un secondo quindi diminuisco il timer di uno e di conseguenza anche il punteggio
            //  successivamente rimodifico la variabile velocita a 0 , aspettando un altro input
            //(utilizzo il campo velocità per passare una struct oggetto in linea con tutti gli oggetti del codice)

            if (tempo.velocita)
            {
                timer--;
                punteggio -= 10;
                tempo.velocita = 0;
            }
            pthread_mutex_unlock(&mutex);

            // controllo se c'è almeno una tana che non è stata chiusa
            buffer = controlloTaneChiuse(arrayTane);

            if (timer <= 0)
            {
                pthread_mutex_lock(&mutex);
                rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                pthread_mutex_unlock(&mutex);
            }
            pthread_mutex_lock(&mutex);
            stampaRana(finestraGioco, rana.coordinate, coloreTroncoRana, info.difficolta);
            pthread_mutex_unlock(&mutex);
            wrefresh(finestraGioco);

            /* uscita dal gioco nel caso in cui viene premuta la q, finiscono le vite, finisce il tempo
            oppure vengono chiuse tutte le tane. Nel caso di vittoria o sconfitta
            viene stampata una schermata finale diversa*/
            riniziaPartita = finePartita(finestraGioco, rana, vite, buffer, punteggio, info.difficolta, &partitaInCorso, partitaFinita);
        }
        buffer = false;
    }

    return riniziaPartita;
}