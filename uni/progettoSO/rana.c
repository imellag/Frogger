#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

void funzRana( int p[], int pproiettile[]) {
    Oggetto rana;
    rana.id = RANA;
    rana.coordinate.x = 0;
    rana.coordinate.y = ALTEZZA_SCHERMO - 6;

    pid_t pidProiettile;
    
    int inputMovimento;
    close(p[0]);
    
    while (true) {
        inputMovimento = getch();
        switch (inputMovimento)
        {
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;
            // controllo coordinate
            break;
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            // controllo coordinate
            break;
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            // controllo coordinate
            break;
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            // controllo coordinate
            break;

            case q:
            rana.id=q;
            write(p[1],&rana,sizeof(Oggetto));
            break;

        case SPACEBAR:
            pidProiettile=fork();
            if(pidProiettile<0){
                printw("Error");
                exit(-1);
            }
            else if (pidProiettile==0){
                funzProiettile(pidProiettile,pproiettile, rana.coordinate);
                
            }
            else{
               
                break;
            }
            

        default:
            break;
        }

       
        write(p[1], &rana, sizeof(Oggetto));
    
    }
}

void funzProiettile(pid_t pidProiettile,int pproiettile[], Coordinate rana) {
    Oggetto proiettile;

    proiettile.id=PROIETTILE;
    proiettile.coordinate.x=rana.x+2;
    proiettile.coordinate.y = rana.y-1;

    close(pproiettile[0]);
    while(true) {
       
        if(controlloLimiti,PROIETTILE)
            kill(pidProiettile,SIGKILL);

        write(pproiettile[1],&proiettile,sizeof(Oggetto));
        usleep(40000);


    }
    
    
    

}
