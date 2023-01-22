#define MIN_TEMPO_MACCHINA 20000
#define MAX_TEMPO_MACCHINA 40000
#define MIN_TEMPO_CAMION 20000
#define MAX_TEMPO_CAMION 40000

// stampa lo sfondo dell'autostrada
void funzAutostrada();

// crea per ogni macchina unn processo
void funzAuto(int p[]);

void funzCamion(int p[], int velocitaCorsie[],int spostamento);

// gestisce il movimento della macchina che successivamente viene inviato al main
void movimentoMacchina(int p[], int numeroMacchina, int velocita[]);

void movimentoCamion(int p[DUE], int numeroCamion, int velocita[]);

// dopo che la macchina arriva a uno dei due bordi 
//viene rigenerata in una corsia random
Oggetto cambioCorsia();

// funziona per stampare la macchina
void stampaMacchina(Oggetto macchina);

void stampaCamion(Oggetto camion);

_Bool postoOccupato(Coordinate veicolo, int corsia, int velocita[], int veicol);
