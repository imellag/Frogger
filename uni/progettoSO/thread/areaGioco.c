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

    int troncoNemico, vite, corsiaRandom, corsia;

    bool buffer = false;
    bool partitaFinita;
    bool coloreTroncoRana = false;
    bool sulTronco = false;
    bool partitaInCorso = true;
    pthread_t threadRana, threadProiettile[NUMERO_PROIETTILI],
        threadTronchi[MAX_TRONCHI], threadMacchine[NUMERO_MACCHINE], threadCamion[NUMERO_CAMION],
        threadTempo, threadProiettileNemico[MAX_TRONCHI], threadCambioCorsia;

    // variabili utilizzate per far spawnare i nemici e farli sparare al momento giusto
    time_t inizio_nemico, fine_nemico, inizio_proiettile[MAX_TRONCHI], fine_proiettile;
    time_t inizioSparo, fineSparo;

    Oggetto proiettileNemico[MAX_TRONCHI], proiettilinoNemico[MAX_TRONCHI];

    Oggetto rana, ranocchio, vecchiaRana, tronchino, tempo;

    Oggetto pacchetto;
    Proiettile proiettilino[NUMERO_PROIETTILI], proiettile[NUMERO_PROIETTILI];
    Coordinate nuoveCoordinate;
    int macchineInCoda = 0;

    Colore bufferColori;
    bool arrayTane[NUMERO_TANE] = {false};

    bool nemico[MAX_TRONCHI] = {false};

    int delayProiettileNemico;

    parametriVeicolo macchina[NUMERO_MACCHINE], camion[NUMERO_CAMION], macchinina, camioncino;

    Oggetto tronco[MAX_TRONCHI];
    WINDOW *finestraGioco;

    bool hitProiettile[MAX_TRONCHI];

    // inizializzo le variabili
    int offset = 0;
    bool riniziaPartita = false;
    int timer = TEMPO_INIZIALE;
    int punteggio = PUNTEGGIO_INIZIALE;

    bool flagCambioCorsia = false;

    // musica di sottofondo
    if (info.audio)
        system("ffplay -nodisp ../file_audio/frogger.mp3 2> /dev/null & ");

    // creo la finestra in base alla difficoltà scelta dall'utente
    // quindi se verrà impostata la difficoltà facile si avranno 3 corsie di fiumi e di strada
    // difficoltà media 4 corsie di fiumi e di strada ecc...
    finestraGioco = newwin(ALTEZZA_SCHERMO + (info.difficolta * NUMERO_CORSIE * 2),
                           LARGHEZZA_SCHERMO - 1, INIZIO_ALTEZZA_FINESTRA, INIZIO_LARGHEZZA_FINESTRA);

    rana.coordinate.x = 0;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + info.difficolta * NUMERO_CORSIE * 2;
    rana.difficolta = info.difficolta;

    for (i = 0; i < MAX_TRONCHI; i++)
        hitProiettile[i] = false;

    vite = MAX_VITE - info.difficolta;

    clear();
    refresh();

    // per ogni veicolo genero un colore casuale
    creaColoriRandom(info.difficolta);

    /* Lascio 2 righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
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
    inizializzaArray(tronco, proiettileNemico, proiettilinoNemico, camion, macchina, proiettilino, proiettile);

    // velocità di ogni tronco
    int velocita[MAX_CORSIE];
    int spostamento;
    for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
        velocita[i] = 1;

    // creo casualmente la direzione della prima corsia dei tronchi, e inverto la direzione per ogni altro tronco
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    pthread_create(&threadRana, NULL, &movimentoRana, &rana);
    pthread_create(&threadCambioCorsia, NULL, &delayCambioCorsia, &flagCambioCorsia);

    for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
    {
        tronco[i].velocita = velocita[i] * spostamento;
        tronco[i].id = i;
        tronco[i].difficolta = info.difficolta;
        pthread_create(&threadTronchi[i], NULL, &movimentoTronco, &tronco[i]);
        spostamento *= -1;
    }

    // randomizzo anche la direzione della prima corsia della macchine e la inverto per le corsie successive
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    int velocitaCorsie[MAX_CORSIE];
    int direzioneCorsie[MAX_CORSIE];
    j = 0;

    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
    {
        direzioneCorsie[i] = spostamento;
        spostamento *= -1;
    }

    // creo casualmente la velocità per ogni corsia che varia anche in base alla difficoltà
    for (i = 0; i < NUMERO_CORSIE + info.difficolta; i++)
        velocitaCorsie[i] = (MIN_VELOCITA_VEICOLI + rand() % (MAX_VELOCITA_VEICOLI - MIN_VELOCITA_VEICOLI)) - 7500 * info.difficolta;

    // inizializzo le coordinate dei veicoli
    for (i = 0; i < NUMERO_MACCHINE; i++)
    {
        inizializzaVeicolo(macchina[i], info.difficolta, direzioneCorsie, velocitaCorsie);
        macchina[i].veicolo.id = MACCHINA0;
        pthread_create(&threadMacchine[i], NULL, &movimentoVeicolo, &macchina[i]);
    }
    for (i = 0; i < NUMERO_CAMION; i++)
    {
        inizializzaVeicolo(camion[i], info.difficolta, direzioneCorsie, velocitaCorsie);
        camion[i].veicolo.id = CAMION0;
        pthread_create(&threadCamion[i], NULL, &movimentoVeicolo, &camion[i]);
    }

    pthread_create(&threadTempo, NULL, &orologio, &tempo);

    time(&inizio_nemico);

    partitaInCorso = true;

    time(&inizioSparo);

    for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
        time(&inizio_proiettile[i]);
    partitaFinita = false;
    while (partitaInCorso)
    {
        time(&fineSparo);

        // ricevo partitaFinita
        if (!partitaFinita)
        {

            pthread_mutex_lock(&mutex);
            // controllo se la rana è entrata in una tana
            risultato = controlloLimitiRana(rana.coordinate, info.difficolta);

            // se la rana è entrata in una tana viene portata alla posizione iniziale viene aggiornato il punteggio
            // i numeri da 0 a 5 sono i numeri di ogni rispettiva tana
            if (risultato < 6 && risultato >= 0)
            {
                if (!(arrayTane[risultato] == true))
                {
                    arrayTane[risultato] = true; // viene chiusa la tana

                    rana = posizioneInizialeRana(rana, info.difficolta);

                    punteggio += PUNTEGGIO_TANA;
                    timer = TEMPO_INIZIALE;
                }
            }

            // se nel thread rana viene premuta la barra spaziatrice
            // allora viene modificato l'id e genero un proiettile
            if (rana.id == SPAWN_PROIETTILE)
            {

                int delaySparo = fineSparo - inizioSparo;

                // inserisco un delay per non dare la possibilità all'utente di sparare molteplici volte
                if (delaySparo > 1)
                {

                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");

                    // passo le coordinate della rana per inizializzare il proiettile
                    proiettilino[offset].rana = rana;
                    pthread_create(&threadProiettile[offset], NULL, &funzProiettile, &proiettilino[offset]);

                    // cambio l'id precedentemente modificato dal thread rana
                    rana.id = RANA;

                    // mi sposto nell'array crato per i proiettili
                    offset++;
                    if (offset == NUMERO_PROIETTILI)
                        offset = 0;

                    time(&inizioSparo);
                }
            }
            pthread_mutex_unlock(&mutex);
            // se ricevo dal thread della rana l'id pausa allora faccio partite la funzione pausa
            // che blocca tutti gli altri thread con un mutex
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

            // stampo rispettivamente il tempo e il punetggio
            stampaTempo(finestraGioco, timer);
            stampaPunteggio(finestraGioco, punteggio);

            /* controllo se it tempo di attesa dello spawn del nemico è finito e se è
             così viene stampato in un tronco che non è occupato dalla rana o da un altro nemico*/
            if ((fine_nemico - inizio_nemico) >= TEMPO_SPAWN_NEMICO - info.difficolta)
            {

                // coontrollo quanti nemici ci sono
                contaNemici = 0;
                for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
                {
                    if (nemico[i])
                        contaNemici++;
                }
                // se il numero dei nemici spawnati corrisponde al numero di tronchi o se
                // la rana è presente sul tronco e i nemici sono un numero in meno rispetto ai tronchi
                // (quindi comunque tutti i tronchi occupati) allora non faccio spawnare nessun nuovo nemico
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
                pthread_mutex_lock(&mutex);
                tronchino = tronco[i];
                pthread_mutex_unlock(&mutex);

                /* controllo la collisione del proiettile con i tronchi e se è presente un nemico lo uccido
                           e distruggo il proiettile*/

                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    proiettile[j] = proiettilino[j];
                    pthread_mutex_unlock(&mutex);

                    if (controlloCollisioneNemicoProiettile(proiettile[j].proiettile, tronchino, nemico[i]))
                    {
                        pthread_mutex_lock(&mutex);
                        nemico[i] = false;
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                        punteggio += PUNTEGGIO_UCCISIONE;
                        pthread_mutex_unlock(&mutex);
                    }
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
                if (rana.coordinate.x <= tronco[i].coordinate.x + LARGHEZZA_TRONCHI &&
                    rana.coordinate.x + LARGHEZZA_RANA >= tronco[i].coordinate.x &&
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
                else if (rana.coordinate.y == tronchino.coordinate.y)
                {
                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_in_acqua.mp3 2> /dev/null & ");
                    sulTronco = false;

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                }
                pthread_mutex_unlock(&mutex);

                /* se il nemico non è morto lo stampo sul tronco e se è pronto a sparare il proiettile
                ne creo il thread */
                if (nemico[i])
                {
                    stampaNemico(finestraGioco, tronchino.coordinate);
                    time(&fine_proiettile);
                    delayProiettileNemico = fine_proiettile - inizio_proiettile[i];
                    if (proiettileNemico[i].coordinate.x == (FUORI_MAPPA - 2) && delayProiettileNemico > 2)
                    {
                        time(&inizio_proiettile[i]);
                        system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");

                        pthread_mutex_lock(&mutex);
                        proiettileNemico[i].coordinate.x = tronco[i].coordinate.x + LARGHEZZA_TRONCHI / 2;
                        proiettileNemico[i].coordinate.y = tronco[i].coordinate.y + ALTEZZA_CORSIE;
                        proiettileNemico[i].difficolta = info.difficolta;
                        pthread_mutex_unlock(&mutex);
                        pthread_create(&threadProiettileNemico[i], NULL, &movimentoProiettileNemico, &proiettileNemico[i]);
                    }
                }
                // altrimenti, se non è presente un nemico stampo solamente il tronco
                else
                    stampaTronco(finestraGioco, tronchino.coordinate);
            }

            // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
            for (i = 0; i < NUMERO_MACCHINE; i++)
            {
                pthread_mutex_lock(&mutex);
                macchinina = macchina[i];
                pthread_mutex_unlock(&mutex);

                for (j = 0; j < NUMERO_NEMICI + info.difficolta; j++)
                {
                    pthread_mutex_lock(&mutex);
                    if (controlloCollisioniProiettiliAuto(proiettileNemico[j].coordinate, macchina[i].veicolo, LARGHEZZA_MACCHINA))
                    {
                        proiettileNemico[j].coordinate.x = FUORI_MAPPA - 2;
                        proiettileNemico[j].coordinate.y = FUORI_MAPPA - 2;
                    }
                    pthread_mutex_unlock(&mutex);
                }
                // controllo la collisione dei proiettili con le auto, sia quelli nemici che quello della rana
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    proiettile[j] = proiettilino[j];
                    pthread_mutex_unlock(&mutex);

                    if (controlloCollisioniProiettiliAuto(proiettile[j].proiettile.coordinate, macchinina.veicolo, LARGHEZZA_MACCHINA))
                    {
                        pthread_mutex_lock(&mutex);
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                        pthread_mutex_unlock(&mutex);
                    }
                }

                stampaMacchina(finestraGioco, macchinina.veicolo, i);

                /* modifico nel thread l'id del veicolo e se viene modificato
                 vuol dire che il veicolo è uscito dalla corsia quindi
                 gli viene assegnata una nuova corsia con un spazio libero */

                if (macchinina.veicolo.id == MACCHINA0_OUT)
                {

                    pthread_mutex_lock(&mutex);
                    bool spawnCorsia = flagCambioCorsia;
                    pthread_mutex_unlock(&mutex);
                    // se la flag è stata cambaita dal thread a true allora posso spawnare la macchina
                    if (spawnCorsia)
                    {

                        pthread_mutex_lock(&mutex);
                        /* ogni volta che un veicolo richiede una corsia gliela assegno partendo da 0
                         e successivamente passo alla successiva (la corsia della macchina è casuale
                         poichè le macchine che richiederanno la stessa corsia sarà casuale) */
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
                        flagCambioCorsia = false;
                        pthread_mutex_unlock(&mutex);
                    }
                }

                // controllo se la rana è dentro il range dello sprite della macchina
                // allora la porto alla alla posizione di partenza e tolgo una vita
                pthread_mutex_lock(&mutex);
                if (controlloCollisioneOggetti(macchinina.veicolo, rana.coordinate, LARGHEZZA_MACCHINA))
                {

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);

                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
                pthread_mutex_unlock(&mutex);
            }

            // inizio i cicli di stampa e di controllo delle collisioni
            // ciclo camion
            for (i = 0; i < NUMERO_CAMION; i++)
            {
                pthread_mutex_lock(&mutex);
                camioncino = camion[i];
                pthread_mutex_unlock(&mutex);

                for (j = 0; j < NUMERO_NEMICI + info.difficolta; j++)
                {
                    pthread_mutex_lock(&mutex);
                    if (controlloCollisioniProiettiliAuto(proiettileNemico[j].coordinate, camion[i].veicolo, LARGHEZZA_CAMION))
                    {
                        proiettileNemico[j].coordinate.x = FUORI_MAPPA - 2;
                        proiettileNemico[j].coordinate.y = FUORI_MAPPA - 2;
                    }
                    pthread_mutex_unlock(&mutex);
                }
                // controllo se i proiettili collidono con un camion e nel caso li distruggo
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    proiettile[j] = proiettilino[j];
                    pthread_mutex_unlock(&mutex);

                    if (controlloCollisioniProiettiliAuto(proiettile[j].proiettile.coordinate, camioncino.veicolo,
                                                          LARGHEZZA_CAMION))
                    {
                        pthread_mutex_lock(&mutex);
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);
                        pthread_mutex_unlock(&mutex);
                    }
                }
                // stampo i camion

                stampaCamion(finestraGioco, camioncino.veicolo, i);

                // controllo la collisione della rana con l'array dei camion e se è dentro la sprite di un camion
                // tolgo una vita e resetto la sua posizione
                pthread_mutex_lock(&mutex);
                if (controlloCollisioneOggetti(camioncino.veicolo, rana.coordinate, LARGHEZZA_CAMION))
                {

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);

                    if (info.audio)
                        system("ffplay -nodisp ../file_audio/rana_investita.mp3 2> /dev/null & ");
                }
                pthread_mutex_unlock(&mutex);

                /* modifico nel thread l'id del veicolo e se viene modificato
                 vuol dire che il veicolo è uscito dalla corsia quindi
                 gli viene assegnata una nuova corsia con un spazio libero */

                if (camioncino.veicolo.id == MACCHINA0_OUT)
                {
                    pthread_mutex_lock(&mutex);
                    bool spawnCorsia = flagCambioCorsia;
                    pthread_mutex_unlock(&mutex);
                    if (spawnCorsia)
                    {
                        pthread_mutex_lock(&mutex);
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

                        flagCambioCorsia = false;
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }

            // ciclo proiettili nemici
            for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
            {
                pthread_mutex_lock(&mutex);
                proiettilinoNemico[i] = proiettileNemico[i];
                pthread_mutex_unlock(&mutex);
                /* per ogn1 dei proiettili nemici controllo che non collida con il proiettile della rana,
                e nel caso li distruggo entrambi*/
                for (j = 0; j < NUMERO_PROIETTILI; j++)
                {
                    pthread_mutex_lock(&mutex);
                    proiettile[j] = proiettilino[j];
                    pthread_mutex_unlock(&mutex);

                    if (controlloCollisioniProiettili(proiettile[j].proiettile.coordinate, proiettilinoNemico[i].coordinate))
                    {
                        pthread_mutex_lock(&mutex);
                        proiettilino[j].proiettile = uccidiProiettile(proiettilino[j].proiettile, threadProiettile[j]);

                        proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
                        proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
                        pthread_mutex_unlock(&mutex);
                    }
                }
                pthread_mutex_lock(&mutex);
                // controllo se un proiettile nemico collide con la rana e nel caso lo distruggo e tolgo una vita
                if (controlloCollisioniRanaProiettile(proiettilinoNemico[i].coordinate, rana.coordinate))
                {

                    rana = morteRana(finestraGioco, &vite, rana, info.difficolta, &timer);
                    proiettileNemico[i].coordinate.x = FUORI_MAPPA - 2;
                    proiettileNemico[i].coordinate.y = FUORI_MAPPA - 2;
                }
                pthread_mutex_unlock(&mutex);
            }
            // stampo i proiettili nemici
            for (i = 0; i < NUMERO_NEMICI + info.difficolta; i++)
                stampaProiettili(finestraGioco, tronco, nemico, proiettilinoNemico[i].coordinate, info.difficolta);

            // controllo se i proiettili nemici sono in movimento e nel caso li stampo
            for (i = 0; i < NUMERO_PROIETTILI; i++)
                stampaProiettili(finestraGioco, tronco, nemico, proiettile[i].proiettile.coordinate, info.difficolta);

            // stampo il punteggio  e il tempo rimanente nella parte alta dello schermo
            stampaPunteggio(finestraGioco, punteggio);
            stampaTempo(finestraGioco, timer);
            stampaVite(finestraGioco, vite);

            pthread_mutex_lock(&mutex);
            /* se nel thread viene modificata la variabile a 1 di tempo allora vuol dire che il
              tempo è sceso di un secondo quindi diminuisco il timer di 1 e di conseguenza anche il punteggio
              successivamente rimodifico la variabile velocita a 0 , aspettando un altro input
            (utilizzo il campo velocità per passare una struct oggetto in linea con tutti gli oggetti del codice) */

            if (tempo.velocita)
            {
                timer--;
                // calcolo il punteggio che scende con il passare del tempo
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
            viene stampata una schermata finale diversa */
            partitaInCorso = finePartita(finestraGioco, rana, vite, buffer, punteggio, info.difficolta, partitaFinita);
             buffer = false;
        }

       
    }

    riniziaPartita = pausaeNuovaPartita(finestraGioco, 2);

    if (!riniziaPartita)
    {
        werase(finestraGioco);
        wrefresh(finestraGioco);
        schermataFinale(finestraGioco);
    }

    system("killall ffplay");

   for (i = 0; i < NUMERO_TRONCHI + info.difficolta; i++)
        pthread_cancel(threadTronchi[i]);

    for (i = 0; i < NUMERO_CAMION; i++)
        pthread_cancel(threadCamion[i]);

    for (i = 0; i < NUMERO_MACCHINE; i++)
        pthread_cancel(threadMacchine[i]);

    hread_cancel(threadTempo);
    pthread_cancel(threadRana);
    pthread_cancel(threadCambioCorsia);

    pthread_mutex_destroy(&mutex);
    delwin(finestraGioco);

    return riniziaPartita;
}
