// stampa lo sfondo dell'autostrada
void funzAutostrada();

// crea per ogni macchina unn processo
void funzAuto(int p[]);

void funzCamion(int p[2], int velocitaCorsie[],int spostamento);

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
