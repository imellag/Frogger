#include "lib.h"
#include "rana.h"
#include "marciapiede.h"
#include "autostrada.h"
#include "prato.h"
#include "fiume.h"
#include "tane.h"

void colori();

int main()
{
    srand(time(NULL));
    int maxx, maxy;
    int inputMovimento;
    Oggetto rana;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    colori();
    keypad(stdscr, true);
    getmaxyx(stdscr, maxy, maxx);

    int p[2];
    if (pipe(p) == -1) { 
        printf("Error\n");
    }

    while (maxy < ALTEZZA_SCHERMO || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / 2, maxx / 2 -17, "Ingrandisci lo schermo per giocare!"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2, LARGHEZZA_SCHERMO / 2 - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2 + 1, LARGHEZZA_SCHERMO / 2 - 7, "Buona fortuna!");
    refresh();
    sleep(5);
    clear();
    refresh();

    rana.id = 1;
    rana.coordinate.x = (LARGHEZZA_SCHERMO-LARGHEZZA_RANA) / 2;
    rana.coordinate.y = 26;

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();

    pid_t pid1;
    pid1 = fork();

    if (pid1 < 0) {
        printf("Error");
    }
    else if (pid1 == 0) {
        funzRana(rana, inputMovimento);
    }
    else {
        
    }

    sleep(5);

    endwin();
}

void colori() {
    start_color();
    init_color(COLORE_RANA, 75, 890, 20); // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 259, 259, 259); // grigio (per ora), sarebbe 66/66/66 in rgb, convertito
    init_pair(1, COLOR_BLACK, COLORE_RANA); 
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
}
