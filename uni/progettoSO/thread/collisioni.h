void controlloCollisioniMacchine( Oggetto macchinina[],Oggetto ranocchio,int *vite,Oggetto camioncino[],int gameDifficulty);
// funzione per la collisione del proiettile nemico con la rana
bool controlloCollisioniRanaProiettile(Coordinate proiettileNemico, Coordinate ranocchio);
// funzione per le collisioni dei proiettili, sia nemici che della rana, con le macchine
bool controlloCollisioniProiettiliAuto(Coordinate proiettile, Oggetto veicolo, int larghezza);
// collisione tra proiettile alleato e proiettile nemico
bool controlloCollisioniProiettili(Coordinate proiettile, Coordinate proiettileNemico);
// funzione generale che controlla tutte le collisioni di tutti i proiettili con tutti i veicoli

// controllo collisione proiettile della rana con i nemici
bool controlloCollisioneNemicoProiettile(Oggetto proiettile, Oggetto tronco, bool nemico);