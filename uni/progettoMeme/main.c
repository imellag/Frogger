#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

int main() {
    int maxx, maxy;

    initscr();
    start_color();

    getmaxyx(stdscr, maxy, maxx);
    init_color(COLOR_RED, 372, 8, 31);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    WINDOW *autostrada = newwin(10, maxx, 10, 10);
    wprintw(autostrada, "%d\n%d", maxx, maxy);
    wattron(autostrada, COLOR_PAIR(3));

    wbkgd(autostrada, COLOR_PAIR(3));

    wrefresh(autostrada);

    sleep(5);

    endwin();
}