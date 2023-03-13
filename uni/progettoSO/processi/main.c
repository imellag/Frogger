#include "lib.h"
#include "funzioniGenerali.h"
#include "funzioniMain.h"
#include "rana.h"
#include "autostrada.h"
#include "fiume.h"
#include "avvio.h"
#include "collisioni.h"
#include "areaDisegno.h"
#include "areaGioco.h"

int main()
{
    srand(time(NULL));

    Avvio info;
    int maxx, maxy;

    bool riniziaPartita = false;
    bool partitaFinita;

    setlocale(LC_ALL, "");

    do
    {
        // configuro l'ambiente per poter utilizzare ncurses
        initscr();
        noecho();
        curs_set(false);
        cbreak();
        start_color();
        keypad(stdscr, true);
        refresh();
        getmaxyx(stdscr, maxy, maxx);

        dimensioneFinestra(maxx, maxy);
        info = menuIniziale();
        colori(info);
        riniziaPartita = areaGioco(info);

    } while (riniziaPartita);

    return EXIT_SUCCESS;
}
