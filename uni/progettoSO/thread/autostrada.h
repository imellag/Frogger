// intervalli min e max per la generazione randomica della velocità
#define MIN_VELOCITA_VEICOLI 20000
#define MAX_VELOCITA_VEICOLI 40000

// intervalli min e max per la generazione randomica dei veicoli
#define MIN_ATTESA 1000000
#define MAX_ATTESA 5000000

// crea per ogni macchina un processo
void funzAuto(int gameDifficulty);

// crea un camion per ogni processo
void funzCamion(int direzioneCorsie[], int spostamento, int gameDifficulty,int velocitaCorsie[],Coordinate inizioVeicoli[],int pVeicoli[]);

/* gestisce il movimento della macchina che viene successivamente inviato al main
 e il cambio delle corsie */
void* movimentoVeicolo( void *_veicolo);

/* gestisce il movimento dei camion che viene successivamente inviato al main
 e il cambio delle corsie */
void movimentoCamion( int numeroCamion, int gameDifficulty,int direzioneCorsie[],int velocitaCorsie[],Coordinate inizioVeicoli[]);

/* dopo che la macchina arriva a uno dei due bordi
 viene rigenerata in una corsia random */
Oggetto cambioCorsia();

// funzione per stampare la macchina
void stampaMacchina(WINDOW*finestraGioco,Oggetto macchina, int indice);

// funzione per stampare i camion
void stampaCamion(WINDOW*finestraGioco,Oggetto camion, int indice);


bool controlloInizioCoordinateCorsie(Coordinate inizioVeicoli[],int i);

// crea casualmente e restituisce un colore per un singolo veicolo
Colore coloreVeicolo();
