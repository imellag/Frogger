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
void gameOver(WINDOW *finestraGioco);

// stampa la barra del tempo e la aggiorna ogni secondo
void stampaTempo(WINDOW *finestraGioco, int tempo);

// stampa la scritta finale quando il giocatore chiude tutte le tane
void vittoria(WINDOW *finestraGioco, int punteggio);

// gestisce la morte della rana. toglie una vita, resetta il tempo e la riporta nella posizione iniziale
Oggetto morteRana(WINDOW *finestraGioco, int *vite, int pRana[], Oggetto ranocchio, int difficolta, int *tempo);

// crea il processo di un proiettile della rana e riproduce il suono dello sparo
void creaProiettile(int p[], Oggetto ranocchio, int *offset, bool audio);

/* se l'utente mette in pausa la partita manda un segnale di stop a tutti i processi.
Se poi l'utente decide di riprendere la partita fa ripartire i processi, altrimenti manda un SIGKILL */
bool funzPausa(WINDOW *finestraGioco, int difficolta, Oggetto camion[], Oggetto tronchi[], Oggetto macchine[], pid_t pidTempo, pid_t pidRana);

/* stampa l'interfaccia della pausa e della fine della partita. Prende l'input del mouse dell'utente e lo restituisce
alla funzione chiamante che poi lo gestisce */
bool pausaeNuovaPartita(WINDOW *finestraGioco, int chiamata);

// killa il processo di un proiettile e ne resetta le coordinate in modo che non venga mostrato nello schermo
Oggetto uccidiProiettile(Oggetto proiettile);

// stampa la scritta "grazie per aver giocato"
void schermataFinale(WINDOW *finestraGioco);

// time_t spawnNemico(time_t fine_nemico, time_t inizio_nemico, int difficolta,bool sulTronco, bool nemico[],Oggetto tronchino[],time_t inizio_proiettile[],Oggetto rana);
// Oggetto controlloCollisioneVeicoliProiettile(int i,Oggetto proiettilino[], Oggetto proiettileNemico[], Oggetto macchinina[], bool hitProiettile[]);

bool controlloTaneChiuse(bool arrayTane[]);

bool finePartita(WINDOW *finestraGioco, Oggetto ranocchio, int vite, bool buffer, int punteggio, int difficolta, Oggetto tempo, Oggetto macchina[], Oggetto camion[], Oggetto tronco[], bool *partitaInCorso, bool partitaFinita);