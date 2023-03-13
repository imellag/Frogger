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

wchar_t *effettiSonori[DUE] = {
L"█▀▀ █▀▀ █▀▀ █▀▀ ▀█▀ ▀█▀ █   █▀ █▀█ █▄░█ █▀█ █▀█ █",
L"██▄ █▀░ █▀░ ██▄ ░█░ ░█░ █   ▄█ █▄█ █░▀█ █▄█ █▀▄ █",
};

wchar_t *coloreRana[DUE] = {
L"█▀▀ █▀█ █░░ █▀█ █▀█ █▀▀   █▀▄ █▀▀ █░░ █░░ ▄▀█   █▀█ ▄▀█ █▄░█ ▄▀█",
L"█▄▄ █▄█ █▄▄ █▄█ █▀▄ ██▄   █▄▀ ██▄ █▄▄ █▄▄ █▀█   █▀▄ █▀█ █░▀█ █▀█",
};

wchar_t *indietro[DUE] = {
L"█ █▄░█ █▀▄ █ █▀▀ ▀█▀ █▀█ █▀█",
L"█ █░▀█ █▄▀ █ ██▄ ░█░ █▀▄ █▄█",
};

Avvio menuIniziale()
{
    Avvio info;

    info.colore.r = 75;
    info.colore.g = 890;
    info.colore.b = 20;
    info.audio = true;

    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);

    int i, j;
    int input;

    WINDOW *finestraIniziale;

    finestraIniziale = newwin(ALTEZZA_SCHERMO + 2, LARGHEZZA_SCHERMO - 1, INIZIO_ALTEZZA_FINESTRA, INIZIO_LARGHEZZA_FINESTRA);

    init_pair(UNO, COLOR_GREEN, COLOR_BLACK);
    init_pair(DUE, COLOR_BLACK, COLOR_GREEN);

    wattron(finestraIniziale, COLOR_PAIR(UNO));

    // stampa nome con scritta di caricamento
    stampaFrogger(LARGHEZZA_SCHERMO / DUE - 30, 9, finestraIniziale);

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
                        wclear(finestraIniziale);
                        info = impostazioni(finestraIniziale, info);
                    }
                }
            }
        }
    }
    wclear(finestraIniziale);

    delwin(finestraIniziale);
}

Avvio impostazioni(WINDOW *finestraIniziale, Avvio info)
{
    int input, i;

    wattron(finestraIniziale, COLOR_PAIR(UNO));
    stampaFrogger(LARGHEZZA_SCHERMO / DUE - 30, 3, finestraIniziale);

    for (i = 0; i < 2; i++) {
        mvwprintw(finestraIniziale, 11 + i, LARGHEZZA_SCHERMO / 2 - 24, "%ls", effettiSonori[i]);
    }
    for (i = 0; i < 2; i++) {
        mvwprintw(finestraIniziale, 20 + i, LARGHEZZA_SCHERMO / 2 - 32, "%ls", coloreRana[i]);
    }
    for (i = 0; i < 2; i++) {
        mvwprintw(finestraIniziale, 31 + i, 2, "%ls", indietro[i]);
    }
    wattroff(finestraIniziale, COLOR_PAIR(UNO));
    wrefresh(finestraIniziale);

    while (true)
    {
        if (info.audio) {
            wattron(finestraIniziale, COLOR_PAIR(DUE));
            stampaRettangolo(finestraIniziale, 31, 14);
            mvwprintw(finestraIniziale, 16, 46, "Acceso");
            wattroff(finestraIniziale, COLOR_PAIR(DUE));

            wattron(finestraIniziale, COLOR_PAIR(UNO));
            mvwprintw(finestraIniziale, 16, 84, "Spento");
            wattroff(finestraIniziale, COLOR_PAIR(UNO));
        }
        else {
            wattron(finestraIniziale, COLOR_PAIR(DUE));
            mvwprintw(finestraIniziale, 16, 46, "Acceso");
            wattroff(finestraIniziale, COLOR_PAIR(DUE));

            wattron(finestraIniziale, COLOR_PAIR(UNO));
            stampaRettangolo(finestraIniziale, 67, 14);
            mvwprintw(finestraIniziale, 16, 84, "Spento");
            wattroff(finestraIniziale, COLOR_PAIR(UNO));
        }
        if (info.colore.g == 890) { // colore verde 
            // il primo rettangolo dei colori inizia alla x = 16
            wattron(finestraIniziale, COLOR_PAIR(UNO));
            mvwprintw(finestraIniziale, 25, 32, "Rosso");
            wattroff(finestraIniziale, COLOR_PAIR(UNO));

            wattron(finestraIniziale, COLOR_PAIR(DUE));
            stampaRettangolo(finestraIniziale, 52, 23); // 52 = 16 + 36
            mvwprintw(finestraIniziale, 25, 68, "Verde"); // 66 = 52 + 18 (metà rettangolo) - 2 (metà parola) stesso ragionamento per centrare tutte le scritte nei rettangoli
            wattroff(finestraIniziale, COLOR_PAIR(DUE));

            wattron(finestraIniziale, COLOR_PAIR(UNO));
            mvwprintw(finestraIniziale, 25, 104, "Rosso");
            wattroff(finestraIniziale, COLOR_PAIR(UNO));
        }
        else if (info.colore.b == 999) { // colore blu

        }
        else if (info.colore.r == 999) { //colore rosso

        }
        
        wrefresh(finestraIniziale);

        input = getch();

        if (input == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {

            }
        }
    }

    return info;
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
