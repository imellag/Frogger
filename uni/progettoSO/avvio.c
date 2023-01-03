#include "lib.h"
#include "avvio.h"

char frogger[CINQUE][45] = {
    " _____ ____   ___   ____  ____ _____ ____  _ ",
    "|  ___|  _ \\ / _ \\ / ___|/ ___| ____|  _ \\| |",
    "| |_  | |_) | | | | |  _| |  _|  _| | |_) | |",
    "|  _| |  _ <| |_| | |_| | |_| | |___|  _ <|_|",
    "|_|   |_| \\\\_\\___/ \\____|\\____|_____|_| \\_(_)"};


void menuIniziale() {
    curs_set(true);
    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    int i, j;

    init_pair(UNO, COLOR_GREEN, COLOR_BLACK);
    init_pair(DUE, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(UNO));

    // stampa nome con scritta di caricamento
    stampaFrogger(LARGHEZZA_SCHERMO / 2 - 23, 10);

    mvprintw(25, 63, "Caricamento");

    for (i = 0; i < 3; i++) {
        refresh();
        usleep(500000);
        mvprintw(25, 74+i, ".");
    } 

    attroff(COLOR_PAIR(UNO));
    attron(COLOR_PAIR(DUE));

    refresh();
    usleep(500000);
    erase();

    /*
    15-20   nuova partita
    22-27  impostazioni
    29-34  esci
    */

    /*
    36 larghezza
    */

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 36; j++) {
            mvprintw(15+i, 52+j, " ");
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 36; j++) {
            mvprintw(22+i, 52+j, " ");
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 36; j++) {
            mvprintw(29+i, 52+j, " ");
        }
    }

    mvprintw(17,58,"Inizia una nuova partita");
    mvprintw(24, 64,"Impostazioni");
    mvprintw(31, 62, "Esci dal gioco");

    attroff(COLOR_PAIR(DUE));
    attron(COLOR_PAIR(UNO));

    stampaFrogger(LARGHEZZA_SCHERMO / 2 - 23, 5);

    attron(COLOR_PAIR(DUE));

    refresh();

    // printf("\033[?1003h\n"); // Makes the terminal report mouse movement events (per ora non ci serve)

    while (true) {
        int input = getch();

        if (input == KEY_MOUSE)
        {   
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_PRESSED)
                { // click sinistro

                    // mvprintw(0, 0, "Input ricevuto"); debug
                    refresh();
                    
                    if (event.x > 50 && event.x < 90 && event.y > 29 && event.y < 34){ // USCITA
                        clear();
                        mvprintw(ALTEZZA_SCHERMO/2, LARGHEZZA_SCHERMO/2-7, "Uscita in corso");
                        refresh();
                        // sleep(1);
                        endwin();
                        exit(EXIT_SUCCESS);
                    }
                    if (event.x > 50 && event.x < 90 && event.y > 15 && event.y < 20) { // nuova partita
                        break;
                    }
                    else if (event.x > 50 && event.x < 90 && event.y > 22 && event.y < 27) { // impostazioni
                        continue;
                    }
                }
            }
        }
    }

    // printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

    curs_set(false);
}

void stampaFrogger(int iniziox, int inizioy) {
    int i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 45; j++) {
            mvaddch(inizioy + i, iniziox + j, frogger[i][j]);
        }
    }
}
