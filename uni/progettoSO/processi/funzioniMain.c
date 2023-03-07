#include "funzioniMain.h"

char spriteCuore[] = {"<3"};
char spriteVittoria[CINQUE][52] = {
    " _   _    _    ___  __     _____ _   _ _____ ___  _ ",
    "| | | |  / \\  |_ _| \\ \\   / /_ _| \\ | |_   _/ _ \\| |",
    "| |_| | / _ \\  | |   \\ \\ / / | ||  \\| | | || | | | |",
    "|  _  |/ ___ \\ | |    \\ V /  | || |\\  | | || |_| |_|",
    "|_| |_/_/   \\_\\___|    \\_/  |___|_| \\_| |_| \\___/(_)",
};

void dimensioneFinestra(int maxx, int maxy)
{
    clear();
    while (maxy < ALTEZZA_SCHERMO || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / DUE, maxx / DUE - 17, "Ingrandisci lo schermo per giocare!"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / DUE, LARGHEZZA_SCHERMO / DUE - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / DUE + UNO, LARGHEZZA_SCHERMO / DUE - SETTE, "Buona fortuna!");
    refresh();
    clear();
    refresh();
}

void stampaVite(WINDOW *finestraGioco, int vite)
{
    Coordinate vita;
    vita.x = LARGHEZZA_SCHERMO - TRE;
    vita.y = DUE;
    /* inizio a stamparle da in basso a destra, poi mi sposto
    verso sinistra */
    init_pair(OTTO, COLOR_RED, COLOR_BLACK);
    wattron(finestraGioco, COLOR_PAIR(OTTO));
    for (int i = ZERO; i < vite; i++)
    {
        for (int j = ZERO; j < DUE; j++)
            mvwprintw(finestraGioco, vita.y, vita.x + j, "%c", spriteCuore[j]);

        vita.x -= TRE; // lascio 1 di spazio tra le varie vite
    }
    wattroff(finestraGioco, COLOR_PAIR(OTTO));
}

void gameOver(WINDOW *finestraGioco)
{
    mvwprintw(finestraGioco, ALTEZZA_SCHERMO / DUE, LARGHEZZA_SCHERMO / DUE, "Hai perso!");
    wrefresh(finestraGioco);
    sleep(2);

    getch();
}

void vittoria(WINDOW *finestraGioco, int punteggio)
{
    stampaScrittaFinale(finestraGioco, LARGHEZZA_SCHERMO / DUE - LARGHEZZA_SPRITE_FINE / DUE, CINQUE);
    mvwprintw(finestraGioco, 13, LARGHEZZA_SCHERMO / DUE - DIECI, "Punteggio finale: %d!", punteggio);

    wrefresh(finestraGioco);
    sleep(2);

    getch();
}

void stampaScrittaFinale(WINDOW *finestraGioco, int iniziox, int inizioy)
{
    int i, j;

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
    {
        for (j = ZERO; j < LARGHEZZA_SPRITE_FINE; j++)
        {
            mvwaddch(finestraGioco, inizioy + i, iniziox + j, spriteVittoria[i][j]);
        }
    }
}