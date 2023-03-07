
#include "lib.h"
#include "collisioni.h"
void controlloCollisioniMacchine(int pRana[DUE], Oggetto macchinina[],Oggetto ranocchio,int *vite,Oggetto camioncino[]){
    int i,invincibilita=5;
   for (i = 0; i < CINQUE; i++)
            {
                if (invincibilita--<=0 && macchinina[i].coordinate.x < ranocchio.coordinate.x  && (macchinina[i].coordinate.x+LARGHEZZA_MACCHINA) >ranocchio.coordinate.x 
                && macchinina[i].coordinate.y == ranocchio.coordinate.y )
                {
                    invincibilita=5;
                    (*vite)--;
                    ranocchio.coordinate.x = ZERO;
                    ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    clear();
                }
            }
            for (i = 0; i < TRE; i++)
            {
                if (invincibilita--<=0 && camioncino[i].coordinate.x < ranocchio.coordinate.x && (camioncino[i].coordinate.x+LARGHEZZA_CAMION) > ranocchio.coordinate.x 
                && camioncino[i].coordinate.y == ranocchio.coordinate.y )
                {
                    invincibilita=5;
                    (*vite)--;
                    ranocchio.coordinate.x = ZERO;
                    ranocchio.coordinate.y = ALTEZZA_SCHERMO - SEI;
                    write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
                    clear();
                }
            }
        
}