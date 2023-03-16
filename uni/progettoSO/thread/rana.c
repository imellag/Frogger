#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA] = {" o.o ", "+-|-+", "\\-|-/"};
/*
void funzRana(int p[], int pRana[], int gameDifficulty)
{

    pid_t pidRana;

    pidRana = fork();
    if (pidRana < ZERO)
    {
        printw("Error");
        exit(-1);
    }
    else if (pidRana == ZERO)
    {
        movimentoRana(p, pRana, gameDifficulty);
    }
}

void movimentoRana(int p[], int pRana[], int gameDifficulty)
{
    int proiettile_sparato = ZERO;
    int inputMovimento;
    int lettura;
    double diff;
    _Bool move;
    time_t inizio, fine;
    pid_t pidProiettile;
    Oggetto rana;
    Oggetto proiettile;

    rana.coordinate.x = ZERO;
    rana.coordinate.y = POSIZIONE_INIZIALE_RANA_Y + (gameDifficulty)*6;
    rana.pid=getpid();

    close(p[READ]);
    close(pRana[WRITE]);

    time(&inizio);

    while (true)
    {
        rana.id = RANA;
        timeout(UNO);
        inputMovimento = getch();

        read(pRana[READ], &rana, sizeof(Oggetto));
        move = true;
        switch (inputMovimento)
        {
        case w:
        case W:
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == SEI)
                rana.coordinate.y += ALTEZZA_RANA;
            break;
        case s:
        case S:
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == SEI)
                rana.coordinate.y -= ALTEZZA_RANA;
            break;
        case d:
        case D:
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == SEI)
                rana.coordinate.x -= LARGHEZZA_RANA;
            break;
        case a:
        case A:
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimitiRana(rana.coordinate, gameDifficulty) == SEI)
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

        // read(pRana[READ],&rana,sizeof(Oggetto));
        if (move)
            write(p[WRITE], &rana, sizeof(Oggetto));
    }
}

void funzProiettile(Oggetto rana, int p[DUE], int numeroProiettile)
{
    Oggetto proiettile;
    proiettile.id = PROIETTILE0 + numeroProiettile;
    proiettile.coordinate.x = rana.coordinate.x + DUE;
    proiettile.coordinate.y = rana.coordinate.y - UNO;
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

    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_RANA; j++)
        {
            mvwprintw(finestraGioco, rana.y + i, rana.x + j, "%c", spriteRana[i][j]);
        }
    }
    wattroff(finestraGioco, COLOR_PAIR(COLORE_RANA_SFONDO) | A_BOLD);
}