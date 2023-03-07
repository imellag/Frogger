#include "lib.h"

/* funzione generale utilizzata per diversi oggetti nel gioco, riconosciuti tramite la variabile tipo,
e in base al movimento di questo controlla che si comporti correttamente una volta arrivato al bordo dello schermo */
int controlloLimiti(Coordinate entita, int tipo);

// controlla se la rana è sul tronco
int controlloRanaTronco(Coordinate rana, Oggetto tronco[]);

// inzializza i colori
void colori();

// in base alla posizione della rana o del proiettile stampa i colori sottostanti
int controlloPosizione(Coordinate oggetto, bool coloriFiume);

_Bool checkCoordinate(int posizione, int coordinata_da_checkare, int intorni);

void funzTempo(int pOrologio[]);

void orologio(int pOrologio[]);

// riporta la rana alla posizione di partenza dopo aver perso una vita
Oggetto posizioneInizialeRana(int pRana[], Oggetto rana);

// controllo la collisione della rana con diversi tipi di oggetto, e per farlo passo anche la dimensione di questo
bool controlloCollisioneOggetti(Oggetto entita, Coordinate rana, int LARGHEZZA_ENTITA);

/* porta il proiettile fuori dallo schermo dopo che ha è stato distrutto
o ha colpito la rana o il nemico */
Oggetto posizioneInizialeProiettile(int p[], Oggetto proiettile, int i);

// stampa i proiettili controllando prima il colore di sfondo
void stampaProiettile(Coordinate proiettile, Oggetto arrayTronchi[], bool nemico[]);