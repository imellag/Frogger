#include "lib.h"

int main()
{
    srand(time(NULL));
    int maxx, maxy;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    start_color();
    init_color(COLORE_MARCIAPIEDE, 259, 259, 259); // grigio (per ora), sarebbe 66/66/66 in rgb, convertito
    init_pair(1, COLOR_BLACK, COLORE_MARCIAPIEDE);
    keypad(stdscr, true);
    getmaxyx(stdscr, maxy, maxx);

    while (maxy < 30 || maxx < 135)
    {
        erase();
        mvwprintw(stdscr, maxy / 2, maxx / 2 -17, "Ingrandisci lo schermo per giocare!"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    refresh();

    WINDOW *marciapiede = newwin(3, 135, 26, 0);
    wattron(marciapiede, COLOR_PAIR(1));    
    wbkgd(marciapiede, COLOR_PAIR(1));   

    wrefresh(marciapiede);

    sleep(5);

    endwin();
}
