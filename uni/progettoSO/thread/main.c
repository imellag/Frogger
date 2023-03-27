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

    // creo la variabile delle impostazioni dell'utente e inizializzo la difficoltà a facile
    Avvio info;
    info.difficolta = EASY;

    int maxx, maxy;

    bool riniziaPartita = false;
    bool partitaFinita;

    setlocale(LC_ALL, "");

    // configuro l'ambiente per poter utilizzare ncurses
    initscr();
    noecho();
    curs_set(false);
    cbreak();
    start_color();
    keypad(stdscr, true);

    getmaxyx(stdscr, maxy, maxx);
    dimensioneFinestra(maxx, maxy);
    do
    {
        riniziaPartita = false;
        refresh();
        info.colore.g = 890;
        info.audio = false;
         info = menuIniziale();
        colori(info);
        riniziaPartita = areaGioco(info);

    } while (riniziaPartita);

    endwin();

    return EXIT_SUCCESS;
}