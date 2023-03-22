// intervalli min e max per la generazione randomica della velocità
#define MIN_VELOCITA_VEICOLI 20000
#define MAX_VELOCITA_VEICOLI 40000

// intervalli min e max per la generazione randomica dei veicoli
#define MIN_ATTESA 1000000
#define MAX_ATTESA 100000

// vale sia per i camion che per le macchine
#define ALTEZZA_VEICOLI 3

// dimensioni macchina e numero di macchine
#define LARGHEZZA_MACCHINA 6
#define NUMERO_MACCHINE 4
#define MAX_MACCHINE 6

// crea per ogni veicolo un processo
void funzVeicoli(int p[], int gameDifficulty,int pVeicoli[]);

/* gestisce il movimento dei veicoli che viene successivamente inviato al main
 e il cambio delle corsie */
void movimentoVeicolo(int p[], int numeroMacchina, int gameDifficulty,int direzioneCorsie[],int velocitaCorsie[],int pVeicoli[], int tipo);

/* dopo che la macchina arriva a uno dei due bordi
 viene rigenerata in una corsia random */
Oggetto cambioCorsia();

// funzione per stampare la macchina
void stampaMacchina(WINDOW*finestraGioco,Oggetto macchina, int indice);

// funzione per stampare i camion
void stampaCamion(WINDOW*finestraGioco,Oggetto camion, int indice);

// crea casualmente e restituisce un colore per un singolo veicolo
Colore coloreVeicolo();