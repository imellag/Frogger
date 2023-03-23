#include "lib.h"
#include "autostrada.h"

// controlla se la rana è entrata in una tana oppure ha provato a uscire dallo schermo
int controlloLimitiRana(Coordinate entita, int gameDifficulty);

/* funzione generale utilizzata per diversi oggetti nel gioco, riconosciuti tramite la variabile tipo,
e in base al movimento di questo controlla che si comporti correttamente una volta arrivato al bordo dello schermo */
int controlloLimitiMacchina(Coordinate entita);

// funzioni per controllare che gli oggetti non escano dallo schermo
int controlloLimitiProiettile(Coordinate entita);
int controlloLimitiCamion(Coordinate entita);
int controlloLimitiTronco(Coordinate entita);

// controlla se la rana è sul tronco
int controlloRanaTronco(Coordinate rana, Oggetto tronco[]);

// inzializza i colori
void colori(Avvio info);

// in base alla posizione della rana o del proiettile stampa i colori sottostanti
int controlloPosizione(Coordinate oggetto, bool coloriFiume, int gameDifficulty);

/* controlla le coordinate della rana quando entra in una tana e restituisce un booleano che indica
 se la rana è entrata nella tana alla quale è stata aumentata la hitbox di un numero pari agli intorni */
_Bool checkCoordinate(int posizione, int coordinata_da_checkare, int intorni);

// creo il processo del tempo
void funzTempo(int pOrologio[]);

// manda un segnale ogni secondo per gestire il timer
void orologio(int pOrologio[]);

// riporta la rana alla posizione di partenza dopo aver perso una vita
Oggetto posizioneInizialeRana(int pRana[], Oggetto rana, int gameDifficulty);

// controllo la collisione della rana con diversi tipi di oggetto, e per farlo passo anche la dimensione di questo
bool controlloCollisioneOggetti(Oggetto entita, Coordinate rana, int LARGHEZZA_ENTITA);

/* porta il proiettile fuori dallo schermo dopo che ha è stato distrutto
o ha colpito la rana o il nemico */
Oggetto posizioneInizialeProiettile(int p[], Oggetto proiettile, int i);

// stampa i proiettili controllando prima il colore di sfondo
void stampaProiettili(WINDOW *finestraGioco, Oggetto arrayTrochi[], bool nemico[], Coordinate proiettile, int gameDifficulty);

// funzione solo per stampare il proiettile attivando il colore giusto
void soloStampa(WINDOW *finestraGioco, Coordinate proiettile, int colore);