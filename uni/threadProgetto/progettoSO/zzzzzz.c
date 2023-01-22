

#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define ALTEZZA_SCHERMO 35
#define LARGHEZZA_SCHERMO 140

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(true);

    start_color();

    // PlaySound("C:\\Snakes and Ladders\\snake.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);

    int i, j;

    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(1));

    /*
    12-17   nuova partita
    19-24  impostazioni
    26-31  esci
    */

    /*
    40 larghezza
    */

    mvprintw(0, 0, "prova");
    refresh();

    sleep(2);
    clear();

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


    attroff(COLOR_PAIR(1));

    refresh();

    // printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

    i = -1;

    while (true) {
        int input = getch();
        i++;

        if (input == KEY_MOUSE)
        {   

            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_PRESSED)
                { // click sinistro

                    mvprintw(i, 0, "Input ricevuto");
                    refresh();
                    
                    if (event.x > 50 && event.x < 90 && event.y > 26 && event.y < 31){ // CASO USCITA
                        mvprintw(0, 30, "Uscita in corso");
                        refresh();
                        sleep(5);
                        endwin();
                        return 0;
                    }
                    if (event.x && event.y)
                    { // controllo le coordinate del click
                        continue;
                    }
                    else if (event.x && event.y){
                        continue;
                    }
                }
            }
        }
        else if (input == 113) {
            endwin();
            return 0;
        }
    }

    // printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

    endwin();

    return 0;
}