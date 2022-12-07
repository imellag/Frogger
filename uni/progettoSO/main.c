#include "lib.h"
#include "rana.h"
#include "marciapiede.h"

int main()
{
    srand(time(NULL));
    int maxx, maxy;
    Oggetto rana;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    start_color();
    init_color(COLORE_MARCIAPIEDE, 259, 259, 259); // grigio (per ora), sarebbe 66/66/66 in rgb, convertito
    init_pair(1, COLOR_BLACK, COLORE_MARCIAPIEDE);
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
    refresh();

    rana.id = 1;
    rana.x = (LARGHEZZA_SCHERMO-LARGHEZZA_RANA) / 2;
    // rana.y = da vedere cosa mettere essendo una matrice (penso che dobbiamo indicare le coordinate del punto in alto a sinistra della rana)

    WINDOW *marciapiede = newwin(3, LARGHEZZA_SCHERMO, 26, 0);
    wattron(marciapiede, COLOR_PAIR(1));    
    wbkgd(marciapiede, COLOR_PAIR(1));   
    
    wrefresh(marciapiede);

    pid_t pid1;
    pid1 = fork();

    if (pid1 < 0) {
        printf("Error");
    }
    else if (pid1 == 0) {
        rana(rana);
    }
    else {
        
    }

    sleep(5);

    endwin();
}
