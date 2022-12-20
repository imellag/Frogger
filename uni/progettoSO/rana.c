#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

void funzRana(int p[])
{
    Oggetto rana;
    Oggetto proiettile;
    rana.id = RANA;
    rana.coordinate.x = 0;
    rana.coordinate.y = ALTEZZA_SCHERMO - 6;
    pid_t pidProiettile;
    int proiettile_sparato = 0;
    int proiettile_sparato = 0;
    int inputMovimento;
    close(p[0]);

    while (true)
    {

    while (true)
    {
        inputMovimento = getch();
        switch (inputMovimento)
        {
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
                 rana.coordinate.y += ALTEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
                 rana.coordinate.y += ALTEZZA_RANA;
            break;
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
               rana.coordinate.y -= ALTEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
               rana.coordinate.y -= ALTEZZA_RANA;
            break;
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
                   rana.coordinate.x -= LARGHEZZA_RANA;
             if (controlloLimiti(rana.coordinate,RANA))
                   rana.coordinate.x -= LARGHEZZA_RANA;
            break;
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
              if (controlloLimiti(rana.coordinate,RANA))
               rana.coordinate.x += LARGHEZZA_RANA;
              if (controlloLimiti(rana.coordinate,RANA))
               rana.coordinate.x += LARGHEZZA_RANA;
            break;

        case q:
            rana.id = q;
            write(p[1], &rana, sizeof(Oggetto));
        case q:
            rana.id = q;
            write(p[1], &rana, sizeof(Oggetto));
            break;

        case SPACEBAR:

            pidProiettile = fork();
            if (pidProiettile < 0)
            {
                perror("error");
                exit(1);

            pidProiettile = fork();
            if (pidProiettile < 0)
            {
                perror("error");
                exit(1);
            }
            else if (pidProiettile == 0)
            {
                proiettile_sparato = funzProiettile(rana, p);
                return;
            else if (pidProiettile == 0)
            {
                proiettile_sparato = funzProiettile(rana, p);
                return;
            }
            break;

            break;

        default:
            break;
        }
        if (proiettile_sparato != 5)
        {
            write(p[1], &rana, sizeof(Oggetto));
            proiettile_sparato = 0;
        }
        if (proiettile_sparato != 5)
        {
            write(p[1], &rana, sizeof(Oggetto));
            proiettile_sparato = 0;
        }
    }
}

int funzProiettile(Oggetto rana, int p[2])
{
int funzProiettile(Oggetto rana, int p[2])
{
    Oggetto proiettile;
    proiettile.id = 1;
    proiettile.coordinate.x = rana.coordinate.x + 2;
    proiettile.coordinate.y = rana.coordinate.y - 1;
    while (true)
    {
        if (controlloLimiti(proiettile.coordinate, PROIETTILE))
        {
            proiettile.id=PROIETTILE_OUT;
            write(p[1], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[1], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y--;
    proiettile.id = 1;
    proiettile.coordinate.x = rana.coordinate.x + 2;
    proiettile.coordinate.y = rana.coordinate.y - 1;
    while (true)
    {
        if (controlloLimiti(proiettile.coordinate, PROIETTILE))
        {
            proiettile.id=PROIETTILE_OUT;
            write(p[1], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[1], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y--;
    }
    return 5;
    return 5;
}