#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA] = {" o.o ", "+-|-+", "\\-|-/"};

void funzRana(int p[], int pRana[], int gameDifficulty)
{

    pid_t pidRana;

    pidRana = fork();
    if (pidRana < 0)
    {
        printw("Error");
        exit(-1);
    }
    else if (pidRana == 0)
    {
        movimentoRana(p, pRana, gameDifficulty);
        exit(0);
    }
}

void movimentoRana(int p[], int pRana[], int gameDifficulty)
{
    int proiettile_sparato = 0;
    int inputMovimento;
    int lettura;

    double diff;
    _Bool move;
    pid_t pidProiettile;
    Oggetto rana;
    Oggetto pacchetto;
    Oggetto proiettile;
    close(p[READ]);
    close(pRana[WRITE]);

    rana.coordinate.x = 0;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + (gameDifficulty)*6;
    rana.pid = getpid();
    rana.id = RANA;
    write(p[WRITE], &rana, sizeof(Oggetto));

    while (true)
    {
        rana.id = RANA;
        timeout(1);
        inputMovimento = getch();
        if (read(pRana[READ], &pacchetto, sizeof(Oggetto)) > 0)
            rana = pacchetto;

        move = true;
        switch (inputMovimento)
        {
        case w:
        case W:
        case KEY_UP:

            rana.coordinate.y -= ALTEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == LIMITE_RANA)
                rana.coordinate.y += ALTEZZA_RANA;
            break;
        case s:
        case S:
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == LIMITE_RANA)
                rana.coordinate.y -= ALTEZZA_RANA;
            break;
        case d:
        case D:
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == LIMITE_RANA)
                rana.coordinate.x -= LARGHEZZA_RANA;
            break;
        case a:
        case A:
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == LIMITE_RANA)
                rana.coordinate.x += LARGHEZZA_RANA;
            break;
        case Q:
        case q:
            rana.id = q;
            move = false;
            write(p[WRITE], &rana, sizeof(Oggetto));
            break;

        case SPACEBAR:
            move = false;
            rana.id = SPAWN_PROIETTILE;
            write(p[WRITE], &rana, sizeof(Oggetto));

            break;

        case 'p':
        case 'P':
            rana.id = PAUSA;
            move = false;
            write(p[WRITE], &rana, sizeof(Oggetto));
            break;

        default:
            move = false;
            break;
        }

        if (move)
            write(p[WRITE], &rana, sizeof(Oggetto));
    }
}

void funzProiettile(Oggetto rana, int p[], int numeroProiettile)
{
    Oggetto proiettile;
    proiettile.id = PROIETTILE0 + numeroProiettile;
    proiettile.coordinate.x = rana.coordinate.x + 2;
    proiettile.coordinate.y = rana.coordinate.y - 1;
    proiettile.pid = getpid();
    while (true)
    {
        if (controlloLimitiProiettile(proiettile.coordinate))
        {
            proiettile.id = PROIETTILE0_OUT + numeroProiettile;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y--;
    }
    return;
}

void stampaRana(WINDOW *finestraGioco, Coordinate rana, _Bool coloreRanaTronco, int gameDifficulty)
{
    int i, j, colorePosizione;
    colorePosizione = controlloPosizione(rana, coloreRanaTronco, gameDifficulty);

    init_pair(COLORE_RANA_SFONDO, COLORE_RANA, colorePosizione);
    wattron(finestraGioco, COLOR_PAIR(COLORE_RANA_SFONDO) | A_BOLD);

    for (i = 0; i < ALTEZZA_RANA; i++)
    {
        for (j = 0; j < LARGHEZZA_RANA; j++)
        {
            mvwprintw(finestraGioco, rana.y + i, rana.x + j, "%c", spriteRana[i][j]);
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_RANA_SFONDO) | A_BOLD);
}