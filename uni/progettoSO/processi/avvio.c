#include "lib.h"
#include "avvio.h"

wchar_t *frogger[ALTEZZA_SPRITE] = {
    L"███████╗██████╗░░█████╗░░██████╗░░██████╗░███████╗██████╗░██╗",
    L"██╔════╝██╔══██╗██╔══██╗██╔════╝░██╔════╝░██╔════╝██╔══██╗██║",
    L"█████╗░░██████╔╝██║░░██║██║░░██╗░██║░░██╗░█████╗░░██████╔╝██║",
    L"██╔══╝░░██╔══██╗██║░░██║██║░░╚██╗██║░░╚██╗██╔══╝░░██╔══██╗╚═╝",
    L"██║░░░░░██║░░██║╚█████╔╝╚██████╔╝╚██████╔╝███████╗██║░░██║██╗",
    L"╚═╝░░░░░╚═╝░░╚═╝░╚════╝░░╚═════╝░░╚═════╝░╚══════╝╚═╝░░╚═╝╚═╝",
};

Avvio menuIniziale(char *Nomeutente)
{
    // int gameDifficulty;
    Avvio info;

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);
     mouseinterval(0); 

    int i, j;
    int input;

    WINDOW *finestraIniziale;

    finestraIniziale = newwin(ALTEZZA_SCHERMO+2, LARGHEZZA_SCHERMO - 1, INIZIO_ALTEZZA_FINESTRA, INIZIO_LARGHEZZA_FINESTRA);

    init_pair(UNO, COLOR_GREEN, COLOR_BLACK);
    init_pair(DUE, COLOR_BLACK, COLOR_GREEN);

    wattron(finestraIniziale, COLOR_PAIR(UNO));

    // stampa nome con scritta di caricamento
    stampaFrogger(LARGHEZZA_SCHERMO / DUE - 30, 9, finestraIniziale);

    // printf("\033[?1003h\n"); // Makes the terminal report mouse movement events

    mvwprintw(finestraIniziale, 25, 63, "Caricamento");

    for (i = ZERO; i < TRE; i++)
    {

        mvwprintw(finestraIniziale, 25, 74 + i, ".");
        wrefresh(finestraIniziale);
        usleep(500000);
    }

    wattroff(finestraIniziale, COLOR_PAIR(UNO));

    wclear(finestraIniziale);
    wrefresh(finestraIniziale);
    wattron(finestraIniziale, COLOR_PAIR(DUE));

    for (i = 0; i < 3; i++)
        stampaRettangolo(finestraIniziale, 52, 15 + i * 7); // tra l'inizio di un rettangolo e l'inizio dell'altro ci sono 7 spazi

    mvwprintw(finestraIniziale, 17, 58, "Inizia una nuova partita");
    mvwprintw(finestraIniziale, 24, 64, "Impostazioni");
    mvwprintw(finestraIniziale, 31, 62, "Esci dal gioco");
    wrefresh(finestraIniziale);

    wattroff(finestraIniziale, COLOR_PAIR(DUE));
    wattron(finestraIniziale, COLOR_PAIR(UNO));

    stampaFrogger(LARGHEZZA_SCHERMO / DUE - 30, 5, finestraIniziale);


    wrefresh(finestraIniziale);

    while (true)
    {

        input = getch();

        if (input == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_PRESSED)
                { // click sinistro
                    if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 29 + INIZIO_ALTEZZA_FINESTRA && event.y < 34 + INIZIO_ALTEZZA_FINESTRA)
                    { // USCITA
                        wclear(finestraIniziale);

                        mvwprintw(finestraIniziale, ALTEZZA_SCHERMO / 2, LARGHEZZA_SCHERMO / 2 - 7, "Uscita in corso");
                        wrefresh(finestraIniziale);
                        endwin();
                        exit(EXIT_SUCCESS);
                    }
                    if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 15 + INIZIO_ALTEZZA_FINESTRA && event.y < 20 + INIZIO_ALTEZZA_FINESTRA)
                    { // nuova partita
                        wclear(finestraIniziale);
                        wattron(finestraIniziale, COLOR_PAIR(DUE));

                        for (i = ZERO; i < TRE; i++)
                            stampaRettangolo(finestraIniziale, 52, 15 + i * 7); // tra l'inizio di un rettangolo e l'inizio dell'altro ci sono 7 spazi

                        mvwprintw(finestraIniziale, 17, 67, "Facile");
                        mvwprintw(finestraIniziale, 24, 68, "Media");
                        mvwprintw(finestraIniziale, 31, 65, "Difficile");

                        wattroff(finestraIniziale, COLOR_PAIR(DUE));
                        wattron(finestraIniziale, COLOR_PAIR(UNO));

                        stampaFrogger(LARGHEZZA_SCHERMO / DUE - 30, 5, finestraIniziale);

                        wattroff(finestraIniziale, COLOR_PAIR(UNO));
                        wrefresh(finestraIniziale);

                        while (true)
                        {
                            input = getch();
                            getmouse(&event);

                            if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 15 + INIZIO_ALTEZZA_FINESTRA && event.y < 20 + INIZIO_ALTEZZA_FINESTRA)
                            {
                                delwin(finestraIniziale);
                                info.difficolta = EASY;
                                return info;
                            }
                            else if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 22 + INIZIO_ALTEZZA_FINESTRA && event.y < 27 + INIZIO_ALTEZZA_FINESTRA)
                            {
                                delwin(finestraIniziale);
                                info.difficolta = MEDIUM;
                                return info;
                            }
                            else if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 29 + INIZIO_ALTEZZA_FINESTRA && event.y < 34 + INIZIO_ALTEZZA_FINESTRA)
                            {
                                delwin(finestraIniziale);
                                info.difficolta = HARD;
                                return info;
                            }
                        }
                    }
                    else if (event.x > INIZIO_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.x < FINE_RETTANGOLO_AVVIO + INIZIO_LARGHEZZA_FINESTRA && event.y > 22 + INIZIO_ALTEZZA_FINESTRA && event.y < 27 + INIZIO_ALTEZZA_FINESTRA)
                    { // impostazioni
                        continue;
                    }
                }
            }
        }
    }
    // printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

    wclear(finestraIniziale);
}

void stampaRettangolo(WINDOW *finestra, int iniziox, int inizioy)
{
    int i, j;
    // ogni rettangolo è alto 5 e largo 36
    for (i = ZERO; i < CINQUE; i++)
    {
        for (j = ZERO; j < 36; j++)
            mvwprintw(finestra, inizioy + i, iniziox + j, " ");
    }
}

void stampaFrogger(int iniziox, int inizioy, WINDOW *finestraIniziale)
{
    int i;

    for (i = ZERO; i < ALTEZZA_SPRITE; i++)
        mvwprintw(finestraIniziale, inizioy + i, iniziox, "%ls", frogger[i]);
}
