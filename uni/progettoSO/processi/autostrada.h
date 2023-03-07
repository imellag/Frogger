// intervalli min e max per la generazione randomica della velocità
#define MIN_VELOCITA_VEICOLI 20000
#define MAX_VELOCITA_VEICOLI 50000

// intervalli min e max per la generazione randomica dei veicoli
#define MIN_ATTESA 1000000
#define MAX_ATTESA 5000000

// stampa lo sfondo dell'autostrada
void funzAutostrada();

// crea per ogni macchina un processo
void funzAuto(int p[]);

// crea un camion per ogni processo
void funzCamion(int p[DUE], int velocitaCorsie[], int spostamento);

/* gestisce il movimento della macchina che viene successivamente inviato al main
 e il cambio delle corsie */
void movimentoMacchina(int p[], int numeroMacchina, int velocita);

/* gestisce il movimento dei camion che viene successivamente inviato al main
 e il cambio delle corsie */
void movimentoCamion(int p[DUE], int numeroCamion, int velocita);

/* dopo che la macchina arriva a uno dei due bordi
 viene rigenerata in una corsia random */
Oggetto cambioCorsia();

// funzione per stampare la macchina
void stampaMacchina(Oggetto macchina, int indice);

// funzione per stampare i camion
void stampaCamion(Oggetto camion, int indice);

Colore coloreVeicolo();