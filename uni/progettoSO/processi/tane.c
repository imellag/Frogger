#include "lib.h"
#include "tane.h"

char spriteTana[TRE][LARGHEZZA_TANE] = {"     _____     ", "    /     \\    ", "___/       \\___"};

void funzTane(bool arrayTane[]) {
    int i, j;

    attron(COLOR_PAIR(2));

    for(i = ZERO; i < ALTEZZA_TANE; i++){
        for(j = ZERO; j < LARGHEZZA_SCHERMO; j++){
            mvprintw(INIZIO_TANE + i, ZERO + j, " ");
        }
    }

    stampaTane(arrayTane);

    attroff(COLOR_PAIR(2));
}

void stampaTane(bool arrayTane[]) {
    int i, j, k;
    Coordinate pos;
    pos.x = ZERO;
    pos.y = INIZIO_TANE;

    for (i = 0; i < NUMERO_TANE; i++) {
        if (arrayTane[i] == false) { // tana ancora aperta
            for (j = 0; j < ALTEZZA_TANE; j++) {
                for (k = 0; k < LARGHEZZA_TANE; k++) {
                    mvaddch(pos.y + j, pos.x + k, spriteTana[j][k]);
                }
            }
        }
        else {
            for (k = 0; k < LARGHEZZA_TANE; k++) {
                mvaddch(pos.y+ALTEZZA_TANE-UNO, pos.x + k, '_');
            }
        }
        
        pos.x += LARGHEZZA_TANE;

        // ripeto solo per le 4 tane: l'ultima avrà diversa larghezza perché è più vicina al bordo dello schermo
        if (i != NUMERO_TANE-UNO) {
            for (k = 0; k < LARGHEZZA_TANE; k++) {
                mvaddch(INIZIO_TANE+ALTEZZA_TANE-UNO, pos.x + k, '_');
            }
        }
        else {
            for (k = 0; k < 5; k++) {
                mvaddch(INIZIO_TANE+ALTEZZA_TANE-UNO, pos.x + k, '_');
            }
        }
        pos.x += LARGHEZZA_TANE;
    }
}