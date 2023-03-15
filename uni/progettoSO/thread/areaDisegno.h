// stampa il prato
void funzPrato(WINDOW *finestraGioco, int gameDifficulty);

// stampa lo sfondo del marciapiede
void funzMarciapiede(WINDOW *finestraGioco, int gameDifficulty);

// stampa lo sfondo dell'autostrada
void funzAutostrada(WINDOW *finestraGioco, int gameDifficulty);

// stampo lo sfondo del fiume
void funzFiume(WINDOW *finestraGioco, int gameDifficulty);

// inizializza lo sfondo delle tane
void funzTane(WINDOW*finestraGioco,bool arrayTane[]);
// stampa le tane effettive, facendo differenza tra quelle aperte e quelle chiuse
void stampaTane(WINDOW*finestraGioco,bool arrayTane[]);

// stampa il punteggio in alto
void stampaPunteggio(WINDOW *finestraGioco,int punteggio);