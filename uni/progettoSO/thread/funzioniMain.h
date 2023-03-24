#include "lib.h"
#include "funzioniGenerali.h"
#include "rana.h"
#include "avvio.h"

// controlla all'inzio le dimensioni della finestra e
// se è troppo piccola chiede di ingrandire lo schermo
void dimensioneFinestra(int maxx, int maxy);

// stampa le vite rimaste
void stampaVite(WINDOW *finestraGioco, int vite);

// stampa la scritta finale quando il giocatore esaurisce le vite
void gameOver(WINDOW *finestraGioco, int punteggio);

// stampa la barra del tempo e la aggiorna ogni secondo
void stampaTempo(WINDOW *finestraGioco, int tempo);

// stampa la scritta finale quando il giocatore chiude tutte le tane
void vittoria(WINDOW *finestraGioco, int punteggio);

// gestisce la morte della rana. toglie una vita, resetta il tempo e la riporta nella posizione iniziale
Oggetto morteRana(WINDOW *finestraGioco, int *vite, Oggetto ranocchio, int difficolta, int *tempo);

// crea il processo di un proiettile della rana e riproduce il suono dello sparo
void creaProiettile(Oggetto ranocchio, int *offset);

/* stampa l'interfaccia della pausa e della fine della partita. Prende l'input del mouse dell'utente e lo restituisce
alla funzione chiamante che poi lo gestisce */
bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata);

// killa il processo di un proiettile e ne resetta le coordinate in modo che non venga mostrato nello schermo
Oggetto uccidiProiettile(Oggetto proiettile, pthread_t threadProiettile);

// stampa la scritta "grazie per aver giocato"
void schermataFinale(WINDOW *finestraGioco);

// time_t spawnNemico(time_t fine_nemico, time_t inizio_nemico, int difficolta,bool sulTronco, bool nemico[],Oggetto tronchino[],time_t inizio_proiettile[],Oggetto rana);
Oggetto controlloCollisioneVeicoliProiettile(int i, Oggetto proiettilino[], Oggetto proiettileNemico[], Oggetto macchinina[], bool hitProiettile[], pthread_t threadProiettile, pthread_t threadProiettileNemico[]);

// controlla se c'è almeno una tana ancora aperta
bool controlloTaneChiuse(bool arrayTane[]);

// gestisce le casistiche di fine partita
bool finePartita(WINDOW *finestraGioco, Oggetto rana, int vite, bool buffer, int punteggio,
                 int difficolta, bool *partitaInCorso, bool partitaFinita, pthread_t threadRana, pthread_t threadProiettile[],
                 pthread_t threadTronchi[], pthread_t threadMacchine[], pthread_t threadCamion[], pthread_t threadTempo, pthread_t threadProiettileNemico[]);

    // controlla se l'inizio della corsia generata casualmente è occupato da un altro veicolo
    bool CorsiaOccupata(parametriVeicolo macchinina[], parametriVeicolo camioncino[], int corsia, int difficolta);

/* crea un colore random per ogni veicolo e si assicura che questo colore non sia troppo simile a quello dell'autostrada
 per evitare che le macchine siano poco visibili. Viene chiamata una volta all'inizio della partita e ogni veicolo tiene
 il suo colore per tutta la durata della partita */
void creaColoriRandom(int difficolta);
