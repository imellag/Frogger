#include "lib.h"
#include "tane.h"

char spriteTana[TRE][LARGHEZZA_TANE] = {"     _____     ", "    /     \\    ", "___/       \\___"};

void funzTane() {
    int i, j;

    attron(COLOR_PAIR(SETTE));

    for(i = ZERO; i < ALTEZZA_TANE; i++){
        for(j = ZERO; j < LARGHEZZA_SCHERMO; j++){
            mvprintw(INIZIO_TANE + i, ZERO + j, " ");
        }
    }

    stampaTane();

    attroff(COLOR_PAIR(SETTE));
}

void stampaTane() {
    int i, j, k;
    Coordinate pos;
    pos.x = ZERO;
    pos.y = INIZIO_TANE;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < ALTEZZA_TANE; j++) {
            for (k = 0; k < LARGHEZZA_TANE; k++) {
                mvaddch(pos.y + j, pos.x + k, spriteTana[j][k]);
            }
        }
        
        pos.x += LARGHEZZA_TANE;

        if (i != 4) {
            for (k = 0; k < 15; k++) {
                mvaddch(INIZIO_TANE+ALTEZZA_TANE-UNO, pos.x + k, '_');
            }
        }
        pos.x += LARGHEZZA_TANE;
    }
}
