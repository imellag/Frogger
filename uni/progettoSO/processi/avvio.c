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

wchar_t *effettiSonori[COLORE_NERO_VERDE] = {
    L"█▀▀ █▀▀ █▀▀ █▀▀ ▀█▀ ▀█▀ █   █▀ █▀█ █▄░█ █▀█ █▀█ █",
    L"██▄ █▀░ █▀░ ██▄ ░█░ ░█░ █   ▄█ █▄█ █░▀█ █▄█ █▀▄ █",
};

wchar_t *coloreRana[COLORE_NERO_VERDE] = {
    L"█▀▀ █▀█ █░░ █▀█ █▀█ █▀▀   █▀▄ █▀▀ █░░ █░░ ▄▀█   █▀█ ▄▀█ █▄░█ ▄▀█",
    L"█▄▄ █▄█ █▄▄ █▄█ █▀▄ ██▄   █▄▀ ██▄ █▄▄ █▄▄ █▀█   █▀▄ █▀█ █░▀█ █▀█",
};

wchar_t *indietro[COLORE_NERO_VERDE] = {
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

    init_pair(COLORE_VERDE_NERO, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLORE_NERO_VERDE, COLOR_BLACK, COLOR_GREEN);

    wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

    // stampa nome con scritta di caricamento
    stampaFrogger(LARGHEZZA_SCHERMO / 2 - 30, 9, finestraIniziale);

    mvwprintw(finestraIniziale, 25, 63, "Caricamento");

    for (i = ZERO; i < TRE; i++)
    {
        mvwprintw(finestraIniziale, 25, 74 + i, ".");
        wrefresh(finestraIniziale);
        usleep(500000);
    }

    wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

    wclear(finestraIniziale);
    wrefresh(finestraIniziale);
    stampaIniziale(finestraIniziale);

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
                        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));

                        for (i = ZERO; i < TRE; i++)
                            stampaRettangolo(finestraIniziale, 52, 15 + i * 7); // tra l'inizio di un rettangolo e l'inizio dell'altro ci sono 7 spazi

                        mvwprintw(finestraIniziale, 17, 67, "Facile");
                        mvwprintw(finestraIniziale, 24, 68, "Media");
                        mvwprintw(finestraIniziale, 31, 65, "Difficile");

                        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
                        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

                        stampaFrogger(LARGHEZZA_SCHERMO / 2 - 30, 5, finestraIniziale);

                        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
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
                        stampaIniziale(finestraIniziale);
                        wrefresh(finestraIniziale);
                        continue;
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

    while (true)
    { // fare dei define per tutti questi magic numbers

        stampaSprite(finestraIniziale, info);

        input = getch();


        if (input == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK)
            {
                if (event.x < 30 + INIZIO_LARGHEZZA_FINESTRA && event.y > 30 + INIZIO_ALTEZZA_FINESTRA) // tasto "indietro"
                {
                    wclear(finestraIniziale);
                    return info;
                }
                // audio on / off
                else if (event.x >= 31 + INIZIO_LARGHEZZA_FINESTRA && event.x < 67 + INIZIO_LARGHEZZA_FINESTRA &&
                         event.y >= 14 + INIZIO_ALTEZZA_FINESTRA && event.y <= 19 + INIZIO_ALTEZZA_FINESTRA) // audio on
                {
                    info.audio = true;
                }
                else if (event.x >= 67 + INIZIO_LARGHEZZA_FINESTRA && event.x <= 104 + INIZIO_LARGHEZZA_FINESTRA &&
                         event.y >= 14 + INIZIO_ALTEZZA_FINESTRA && event.y <= 19 + INIZIO_ALTEZZA_FINESTRA) // audio off
                {
                    info.audio = false;
                }
                else if (event.y >= 23 + INIZIO_ALTEZZA_FINESTRA && event.y <= 28 + INIZIO_ALTEZZA_FINESTRA &&
                         event.x >= 16 + INIZIO_LARGHEZZA_FINESTRA && event.x < 52 + INIZIO_LARGHEZZA_FINESTRA) // colore rosso
                {
                    info.colore.r = 999,
                    info.colore.g = 1;
                    info.colore.b = 1;
                }
                else if (event.y >= 23 + INIZIO_ALTEZZA_FINESTRA && event.y <= 28 + INIZIO_ALTEZZA_FINESTRA &&
                         event.x >= 52 + INIZIO_LARGHEZZA_FINESTRA && event.x < 88 + INIZIO_LARGHEZZA_FINESTRA) // colore verde
                {
                    info.colore.r = 75,
                    info.colore.g = 890;
                    info.colore.b = 20;
                }
                else if (event.y >= 23 + INIZIO_ALTEZZA_FINESTRA && event.y <= 28 + INIZIO_ALTEZZA_FINESTRA &&
                         event.x >= 88 + INIZIO_LARGHEZZA_FINESTRA && event.x < 124 + INIZIO_LARGHEZZA_FINESTRA) // colore blu
                {
                    info.colore.r = 1,
                    info.colore.g = 1;
                    info.colore.b = 999;
                }
                else
                { // debug
                    mvwprintw(finestraIniziale, 0, 0, "Input generico");
                    wrefresh(finestraIniziale);
                }
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

void stampaIniziale(WINDOW *finestraIniziale)
{
    int i;

    wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));

    for (i = 0; i < 3; i++)
        stampaRettangolo(finestraIniziale, 52, 15 + i * 7); // tra l'inizio di un rettangolo e l'inizio dell'altro ci sono 7 spazi

    mvwprintw(finestraIniziale, 17, 58, "Inizia una nuova partita");
    mvwprintw(finestraIniziale, 24, 64, "Impostazioni");
    mvwprintw(finestraIniziale, 31, 62, "Esci dal gioco");
    wrefresh(finestraIniziale);

    wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
    wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

    stampaFrogger(LARGHEZZA_SCHERMO / 2 - 30, 5, finestraIniziale);
}

void stampaSprite(WINDOW *finestraIniziale, Avvio info)
{
    int i;

    werase(finestraIniziale);

    wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
    stampaFrogger(LARGHEZZA_SCHERMO / 2 - 30, 3, finestraIniziale);

    for (i = 0; i < 2; i++)
    {
        mvwprintw(finestraIniziale, 11 + i, LARGHEZZA_SCHERMO / 2 - 24, "%ls", effettiSonori[i]);
    }
    for (i = 0; i < 2; i++)
    {
        mvwprintw(finestraIniziale, 20 + i, LARGHEZZA_SCHERMO / 2 - 32, "%ls", coloreRana[i]);
    }
    for (i = 0; i < 2; i++)
    {
        mvwprintw(finestraIniziale, 31 + i, 2, "%ls", indietro[i]);
    }
    wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

    stampaColoriGiusti(finestraIniziale, info);

    wrefresh(finestraIniziale);
}

void stampaColoriGiusti(WINDOW *finestraIniziale, Avvio info)
{
    if (info.audio)
    {
        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
        stampaRettangolo(finestraIniziale, 31, 14);
        mvwprintw(finestraIniziale, 16, 46, "Acceso");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 16, 84, "Spento");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
    }
    else
    {
        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 16, 46, "Acceso");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
        stampaRettangolo(finestraIniziale, 67, 14);
        mvwprintw(finestraIniziale, 16, 84, "Spento");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
    }
    if (info.colore.g == 890)
    { // colore verde
        // il primo rettangolo dei colori inizia alla x = 16
        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 32, "Rosso");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
        stampaRettangolo(finestraIniziale, 52, 23);   // 52 = 16 + 36
        mvwprintw(finestraIniziale, 25, 68, "Verde"); // 66 = 52 + 18 (metà rettangolo) - 2 (metà parola) stesso ragionamento per centrare tutte le scritte nei rettangoli
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 105, "Blu");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
    }
    else if (info.colore.b == 999)
    { // colore blu
        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 32, "Rosso");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 68, "Verde");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
        stampaRettangolo(finestraIniziale, 88, 23);
        mvwprintw(finestraIniziale, 25, 105, "Blu");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
    }
    else if (info.colore.r == 999)
    { // colore rosso
        wattron(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));
        stampaRettangolo(finestraIniziale, 16, 23);
        mvwprintw(finestraIniziale, 25, 32, "Rosso");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_NERO_VERDE));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 68, "Verde");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));

        wattron(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
        mvwprintw(finestraIniziale, 25, 105, "Blu");
        wattroff(finestraIniziale, COLOR_PAIR(COLORE_VERDE_NERO));
    }

    wrefresh(finestraIniziale);
}
