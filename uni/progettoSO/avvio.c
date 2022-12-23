#include "lib.h"
#include "avvio.h"

char frogger[CINQUE][45] = {
    " _____ ____   ___   ____  ____ _____ ____  _ ",
    "|  ___|  _ \\ / _ \\ / ___|/ ___| ____|  _ \\| |",
    "| |_  | |_) | | | | |  _| |  _|  _| | |_) | |",
    "|  _| |  _ <| |_| | |_| | |_| | |___|  _ <|_|",
    "|_|   |_| \\_\\___/ \\____|\\____|_____|_| \\_(_) "};


void menuIniziale() {
    curs_set(true);
    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    int i, j;

    init_pair(UNO, COLOR_GREEN, COLOR_BLACK);
    init_pair(DUE, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(UNO));

    // stampa nome con scritta di caricamento
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 45; j++) {
            mvaddch(10 + i,LARGHEZZA_SCHERMO / 2 - 23 + j, frogger[i][j]);
        }
    }

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
    12-17   nuova partita
    19-24  impostazioni
    26-31  esci
    */

    /*
    40 larghezza
    */

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 40; j++) {
            mvprintw(12+i, 50+j, " ");
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 40; j++) {
            mvprintw(19+i, 50+j, " ");
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 40; j++) {
            mvprintw(26+i, 50+j, " ");
        }
    }

    mvprintw(14,58,"Inizia una nuova partita");
    mvprintw(21, 64,"Impostazioni");
    mvprintw(28, 62, "Esci dal gioco");

    attroff(COLOR_PAIR(DUE));

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
                    
                    if (event.x > 50 && event.x < 90 && event.y > 26 && event.y < 31){ // USCITA
                        clear();
                        mvprintw(ALTEZZA_SCHERMO/2, LARGHEZZA_SCHERMO/2-7, "Uscita in corso");
                        refresh();
                        // sleep(1);
                        endwin();
                        exit(EXIT_SUCCESS);
                    }
                    if (event.x > 50 && event.x < 90 && event.y > 12 && event.y < 17) { // nuova partita
                        break;
                    }
                    else if (event.x > 50 && event.x < 90 && event.y > 19 && event.y < 24) { // impostazioni
                        continue;
                    }
                }
            }
        }
    }

    // printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

    curs_set(false);
}

