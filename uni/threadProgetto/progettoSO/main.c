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
    pthread_t pthreadRana, pthreadMacchine[CINQUE], pthreadTronchi[TRE],
        pthreadNemici, pthreadProiettile, pthreadCamion[TRE], pthreadSchermo;

    int maxx, maxy;
    int differenza[3];
    int maxx_precedente, maxy_precedente;
    int vite = TRE;
    int risultato;
    int direzione;
    int gameDifficulty;
    int spawnNemico;
    int troncoNemico;

    double diff_nemico, diff_proiettile[3];

    bool arrayTane[NUMERO_TANE] = {false, false, false, false, false};

    bool nemico[3] = {false, false, false};

    bool coloreTroncoRana = false;
    bool sulTronco[3] = {false, false, false};

    time_t inizio_nemico, fine_nemico, inizio_proiettile[3], fine_proiettile[3];

    Oggetto proiettileNemico[3];
    Oggetto rana;

    Schermo statistiche;

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

    statistiche.tempo = 50;
    statistiche.punteggio = 0;
    rana.coordinate.x = ZERO;
    rana.coordinate.y = ALTEZZA_SCHERMO - SEI;

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

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    stampaVite(vite);

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(rana.coordinate, coloreTroncoRana);

    refresh();

    pthread_create(&pthreadRana, NULL, &movimentoRana, &rana);
    // funzAuto(p);
    // funzTronchi(p, pRana);
    // funzTempo(pOrologio);

    for (i = 0; i < 5; i++)
    {
        if (i < 3)
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

    time(&inizio_nemico);
    for (i = 0; i < 3; i++)
        time(&inizio_proiettile[i]);

    // se la dimensione della finestra cambiata allora pulisce lo schermo per evitare problemi
    maxx_precedente = maxx;
    maxy_precedente = maxy;
    getmaxyx(stdscr, maxy, maxx);
    if (maxx != maxx_precedente || maxy != maxy_precedente)
        clear();

    risultato = controlloLimiti(rana.coordinate, RANA);

    // controllo se la rana è entrata nelle tane allora la porto alla posizione iniziale e aggiorno il punteggio
    if (risultato < SEI && risultato >= UNO)
    {
        if (!(arrayTane[risultato - 1] == true))
        {
            arrayTane[risultato - 1] = true;
            //   rana = posizioneInizialeRana(pRana, rana);
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

    if ((diff_nemico = difftime(fine_nemico, inizio_nemico)) >= 5)
    {
        do
        {
            troncoNemico = rand() % 3;

        } while (tronchino[troncoNemico].coordinate.y == rana.coordinate.y);
        nemico[troncoNemico] = true;
        time(&inizio_nemico);
    }

    // ciclo per assegnare i pthread agli oggetti e successivamente controllo le collisioni
    // con le varia macchine o se la rana è presente sul tronco
    /*   for (i = 0; i < CINQUE; i++)
       {
           if (i < 3)
           {
               time(&fine_proiettile[i]);

               if (proiettilino.coordinate.x >= tronchino[i].coordinate.x &&
                   proiettilino.coordinate.x <= tronchino[i].coordinate.x + LARGHEZZA_TRONCHI && proiettilino.coordinate.y == tronchino[i].coordinate.y + 2 && nemico[i] == true)
               {

                   nemico[i] = false;
                   kill(proiettilino.pthread, SIGKILL);
                   proiettilino.coordinate.x = -CINQUE;
               }
               if (nemico[i])
               {
                   stampaNemico(tronchino[i].coordinate);

                   time(&fine_proiettile[i]);
                   if ((diff_proiettile[i] = difftime(fine_proiettile[i], inizio_proiettile[i])) >= 1)
                   {
                       time(&inizio_proiettile[i]);
                       funzProiettileNemico(tronchino[i].coordinate, p, i);
                   }
               }
               else
                   // stampo i 3 tronchi
                   stampaTronco(tronchino[i].coordinate);
               // mi prendo i pthread dei tronchi per poi utilizzare questo array per
               // killare correttamente i vari processi tronchi
               pthreadTronchi[i] = tronchino[i].pthread;

               // stampo i camion
               stampaCamion(camioncino[i]);

               // mi prendo i pthread dei camion per poi utilizzare questo array per
               // killare correttamente i vari processi camion
               pthreadCamion[i] = camioncino[i].pthread;

               // controllo se la rana è salita sul tronco
               if (tronchino[i].coordinate.x <= rana.coordinate.x && (tronchino[i].coordinate.x + LARGHEZZA_TRONCHI) >= rana.coordinate.x && rana.coordinate.y == tronchino[i].coordinate.y)
               {
                   if (nemico[i])
                   {
                       vite--;
                       rana = posizioneInizialeRana(pRana, rana);
                   }
                   else
                   {
                       // appena salita la rana mi calcolo la differenza tra l'inizio del tronco e la posizione della rana
                       // così facendo la rana salirà sul tronco nel punto esatto
                       if (!sulTronco[i])
                       {
                           differenza[i] = rana.coordinate.x - tronchino[i].coordinate.x;
                           sulTronco[i] = true;
                       }
                       rana.coordinate.x = tronchino[i].coordinate.x + differenza[i];
                       coloreTroncoRana = true;

                       // comunico la posizione al processo rana
                       write(pRana[WRITE], &rana, sizeof(Oggetto));
                   }
               }

               // in questo casa la rana è sull'altezza del tronco però una x diversa dal range di esso
               // pertanto non è riuscita a salire sul tronco successivo quindi comunico le coordinate di partenza
               // e tolgo una vita
               else if (rana.coordinate.y == tronchino[i].coordinate.y)
               {
                   vite--;
                   rana = posizioneInizialeRana(pRana, rana);
                   sulTronco[i] = false;
               }

               if (controlloCollisioneOggetti(camioncino[i], rana.coordinate, LARGHEZZA_CAMION))
               {
                   vite--;
                   rana = posizioneInizialeRana(pRana, rana);
               }
           }

           stampaMacchina(macchinina[i]);
           pthreadMacchine[i] = macchinina[i].pthread;

           // controllo se la rana è dentro il range dello sprite della macchina
           // allora la porto alla alla posizione di partenza e tolgo una vita
           if (controlloCollisioneOggetti(macchinina[i], rana.coordinate, LARGHEZZA_MACCHINA))
           {
               vite--;
               rana = posizioneInizialeRana(pRana, rana);
           }
       }
       */
    // pthreadSchermo = statistiche.pthread;
   
    stampaRana(rana.coordinate, coloreTroncoRana);
   
    stampaVite(vite);
    mvwprintw(stdscr, proiettilino.coordinate.y, proiettilino.coordinate.x, "%c", spriteProiettile);
    for (i = 0; i < 3; i++)
        mvwprintw(stdscr, proiettileNemico[i].coordinate.y, proiettileNemico[i].coordinate.x, "%c", spriteProiettile);

    mvwprintw(stdscr, UNO, LARGHEZZA_SCHERMO / DUE - QUATTRO, "Score: %d", statistiche.punteggio);
    mvwprintw(stdscr, ALTEZZA_SCHERMO - DUE, LARGHEZZA_SCHERMO / DUE - NOVE, "Tempo rimanente: %-20d", statistiche.tempo);
    refresh();

    if (rana.id == q || vite == ZERO || statistiche.tempo <= 0)
    {
        if (vite == ZERO)
            gameOver();
        endwin();
        for (i = 0; i < 5; i++)
        {
            if (i < 3)
            {
                kill(pthreadCamion[i], SIGKILL);
                kill(pthreadTronchi[i], SIGKILL);
            }

            kill(pthreadMacchine[i], SIGKILL);
        }
        kill(pthreadRana, SIGKILL);
        kill(pthreadSchermo, SIGKILL);

        return EXIT_SUCCESS;
    }
}

