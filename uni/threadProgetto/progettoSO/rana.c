#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA] = {" o.o ", "+-|-+", "\\-|-/"};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* movimentoRana(void* _rana)
{

    Oggetto* rana=(Oggetto*)_rana;
    time_t inizio, fine;
    double diff;
    
    Oggetto proiettile;
    (*rana).id = RANA;
    rana->coordinate.x = ZERO;
    rana->coordinate.y = ALTEZZA_SCHERMO - SEI;
    pthread_t pthreadProiettile;
    int proiettile_sparato = ZERO;
    int inputMovimento;
    int lettura;
  
    _Bool move;
    time(&inizio);
    while (true)
    {

        timeout(1);
        inputMovimento = getch();
      pthread_mutex_lock(&mutex);
        move = true;
        switch (inputMovimento)
        {
        case KEY_UP:
            rana->coordinate.y -= ALTEZZA_RANA;
            if (controlloLimiti(rana->coordinate, RANA) == SEI)
                rana->coordinate.y += ALTEZZA_RANA;
            break;
        case KEY_DOWN:
            rana->coordinate.y += ALTEZZA_RANA;
            if (controlloLimiti(rana->coordinate, RANA) == SEI)
                rana->coordinate.y -= ALTEZZA_RANA;
            break;
        case KEY_RIGHT:
            rana->coordinate.x += LARGHEZZA_RANA;
            if (controlloLimiti(rana->coordinate, RANA) == SEI)
                rana->coordinate.x -= LARGHEZZA_RANA;
            break;
        case KEY_LEFT:
            rana->coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimiti(rana->coordinate, RANA) == SEI)
                rana->coordinate.x += LARGHEZZA_RANA;
            break;

        case q:
            rana->id = q;
            break;

        case SPACEBAR:
            move = false;

            time(&fine);
            if (difftime(fine, inizio) > 1 )
            {
                //pthread_create(&pthreadProiettile,NULL,&funzProiettile)
            }
            break;

        default:
            move = false;
            break;
        }
         pthread_mutex_unlock(&mutex);

        // read(pRana[READ],&rana,sizeof(Oggetto));
        //if (move)
         //   write(p[WRITE], &rana, sizeof(Oggetto));
    }
   // usleep(10000);
}

/*void* funzProiettile(void* _proiettile)
{
    Oggetto* proiettile=_proiettile;
    proiettile->id = UNO;
    proiettile->coordinate.x = rana->coordinate.x + DUE;
    proiettile->coordinate.y = rana->coordinate.y - UNO;
    proiettile->pthread = getpthread();
    while (true)
    {
        if (controlloLimiti(proiettile->coordinate, PROIETTILE))
        {
           pthread_cancel(proiettile->pthread);
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile->coordinate->y--;
    }
    return;
}
*/

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

bool controlloCollisioniProiettile(Coordinate proiettile, Coordinate entita, int LARGHEZZA_ENTITA, int ALTEZZA_ENTITA)
{

    bool flag = false;

    //        if()
}