#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA + UNO] = {" o.o ", "+-|-+", "\\-|-/"};

void funzRana(int p[], int pRana[])
{
    Oggetto rana;
    Oggetto proiettile;
    rana.id = RANA;
    rana.coordinate.x = ZERO;
    rana.coordinate.y = ALTEZZA_SCHERMO - SEI;
    pid_t pidProiettile;
    int proiettile_sparato = ZERO;
    int inputMovimento;
    int lettura;
    close(p[READ]);
    close(pRana[WRITE]);
    _Bool move;

    while (true)
    {

        // if()
        timeout(1);
        inputMovimento = getch();
        read(pRana[READ],&rana,sizeof(Oggetto));
        move = true;
        switch (inputMovimento)
        {
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.y += ALTEZZA_RANA;
            break;
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.y -= ALTEZZA_RANA;
            break;
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.x -= LARGHEZZA_RANA;
            break;
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimiti(rana.coordinate, RANA) == SEI)
                rana.coordinate.x += LARGHEZZA_RANA;
            break;

        case q:
            rana.id = q;
            write(p[WRITE], &rana, sizeof(Oggetto));
            break;

        case SPACEBAR:
            move = false;

            pidProiettile = fork();
            if (pidProiettile < ZERO)
            {
                perror("error");
                exit(1);
            }
            else if (pidProiettile == ZERO)
            {
                proiettile_sparato = funzProiettile(rana, p);
                return;
            }
            break;

        default:
            move = false;
            break;
        }

        // read(pRana[READ],&rana,sizeof(Oggetto));
        if (move)
            write(p[WRITE], &rana, sizeof(Oggetto));

        proiettile_sparato = ZERO;
    }
    usleep(10000);
}

int funzProiettile(Oggetto rana, int p[DUE])
{
    Oggetto proiettile;
    proiettile.id = UNO;
    proiettile.coordinate.x = rana.coordinate.x + DUE;
    proiettile.coordinate.y = rana.coordinate.y - UNO;
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
    return CINQUE;
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
    attroff(COLOR_PAIR(7));
}
