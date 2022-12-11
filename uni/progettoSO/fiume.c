#include "lib.h"
#include "fiume.h"

WINDOW* funzFiume(Oggetto arrayTronchi[]) {
    int corsia = 27;
    WINDOW *finestraFiume = newwin(9, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 27, 0);
    // alto 9

    wattron(finestraFiume, COLOR_PAIR(5));
    wbkgd(finestraFiume, COLOR_PAIR(5));
    wattroff(finestraFiume, COLOR_PAIR(5)); 

    wrefresh(finestraFiume);

    for (int i = 0; i < 3; i++) {
        arrayTronchi[i].id = i+1;
        arrayTronchi[i].coordinate.x = rand()%LARGHEZZA_SCHERMO-LARGHEZZA_TRONCHI-1;
        arrayTronchi[i].coordinate.y = ALTEZZA_SCHERMO-corsia;
        corsia -= 3;
    }
    // a ogni tronco assegno la sua corsia

    return finestraFiume;
}

void funzNemici() {

}

void funzTronchi(int ptronchi[], int velocitaTronchi[3], Oggetto arrayTronchi[]) {

    for (int i = 0; i < 3; i++) {
        if (arrayTronchi[i].coordinate.x + velocitaTronchi[i] + LARGHEZZA_TRONCHI > LARGHEZZA_SCHERMO-1
        || arrayTronchi[i].coordinate.x + velocitaTronchi[i] <= 0) 
        {
            velocitaTronchi[i] *= -1;
        }
        else {
            arrayTronchi[i].coordinate.x += velocitaTronchi[i];
        }

        /* Stavo pensando di aggiungere un controllo del tipo:
        Se il tronco si deve spostare di 3 verso destra ma sono rimasti solo 2 spazi, si sposta 
        Prima di 2 verso destra, poi rimbalza e si sposta di 1 verso sinistra. */
    }   
    close(ptronchi[0]);
    for (int i = 0; i < 3; i++) {
        write(ptronchi[1], &arrayTronchi[i], sizeof (Oggetto));
    }
}