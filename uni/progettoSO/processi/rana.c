#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA] = {" o.o ", "+-|-+", "\\-|-/"};

void funzRana(int p[], int pRana[])
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
        movimentoRana(p, pRana);
    }
}

void movimentoRana(int p[], int pRana[])
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
    rana.id = RANA;
    rana.coordinate.x = ZERO;
    rana.coordinate.y = ALTEZZA_SCHERMO - SEI;

    close(p[READ]);
    close(pRana[WRITE]);

    time(&inizio);

    while (true)
    {
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
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.y += ALTEZZA_RANA;
            break;
        case s:
        case S:
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.y -= ALTEZZA_RANA;
            break;
        case d:
        case D:
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.x -= LARGHEZZA_RANA;
            break;
        case a:
        case A:
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.x += LARGHEZZA_RANA;
            break;
        case Q:
        case q:
            rana.id = q;
            write(p[WRITE], &rana, sizeof(Oggetto));
            break;

        case SPACEBAR:
            move = false;
            // system("ffplay ../file_audio/sparo.mp3 2> /dev/null &");
            time(&fine);
            if (difftime(fine, inizio) > 1)
            {
                time(&inizio);
                pidProiettile = fork();
                if (pidProiettile < ZERO)
                {
                    perror("error");
                    exit(1);
                }
                else if (pidProiettile == ZERO)
                {
                    funzProiettile(rana, p);
                    exit(0);
                }
            }
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

void funzProiettile(Oggetto rana, int p[DUE])
{
    Oggetto proiettile;
    proiettile.id = UNO;
    proiettile.coordinate.x = rana.coordinate.x + DUE;
    proiettile.coordinate.y = rana.coordinate.y - UNO;
    proiettile.pid = getpid();
    while (true)
    {
        if (controlloLimiti(proiettile.coordinate, PROIETTILE))
        {
            proiettile.id = PROIETTILE_OUT;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y--;
    }
    return;
}

void stampaRana(Coordinate rana, _Bool coloreRanaTronco)
{
    int i, j, colorePosizione;
    colorePosizione = controlloPosizione(rana, coloreRanaTronco);

    init_pair(SETTE, COLORE_RANA, colorePosizione);
    attron(COLOR_PAIR(SETTE));

    for (i = ZERO; i < ALTEZZA_RANA; i++)
    {
        for (j = ZERO; j < LARGHEZZA_RANA; j++)
        {
            mvaddch(rana.y + i, rana.x + j, spriteRana[i][j]);
        }
    }
    attroff(COLOR_PAIR(SETTE));
}