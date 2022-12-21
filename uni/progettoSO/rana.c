#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

void funzRana(int p[],int pRana[])
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
    while (true)
    {
        
       read(pRana[READ],&rana,sizeof(Oggetto));
        inputMovimento = getch();
        switch (inputMovimento)
        {
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;  
            break;
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            break;
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            break;
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            break;

        case q:
            rana.id = q;
            write(p[WRITE], &rana, sizeof(Oggetto));
            break;

        case SPACEBAR:

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
            break;
        }
        if (proiettile_sparato != CINQUE)
        {
           
            write(p[WRITE], &rana, sizeof(Oggetto));
            proiettile_sparato = ZERO;
        }
      
      
        
    }
}

int funzProiettile(Oggetto rana, int p[DUE])
{
    Oggetto proiettile;
    proiettile.id = UNO;
    proiettile.coordinate.x = rana.coordinate.x + DUE;
    proiettile.coordinate.y = rana.coordinate.y -UNO;
    while (true)
    {
        if (controlloLimiti(proiettile.coordinate, PROIETTILE))
        {
            proiettile.id=PROIETTILE_OUT;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y--;
    }
    return 5;
}
