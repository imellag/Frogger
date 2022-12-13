#include "lib.h"
#include "rana.h"
#include "funzioniGenerali.h"

void funzRana(Oggetto rana, int p[]) {
    int inputMovimento;
    
    while (true) {
        inputMovimento = getch();
        switch (inputMovimento)
        {
        case KEY_UP:
            rana.coordinate.y -= ALTEZZA_RANA;
            if (controlloLimiti) {
                rana.coordinate.y += ALTEZZA_RANA;
            }
            break; 
        case KEY_DOWN:
            rana.coordinate.y += ALTEZZA_RANA;
            if (controlloLimiti) {
                rana.coordinate.y -= ALTEZZA_RANA;
            }            
            break;
        case KEY_RIGHT:
            rana.coordinate.x += LARGHEZZA_RANA;
            if (controlloLimiti) {
                rana.coordinate.y -= LARGHEZZA_RANA;
            }
            break;
        case KEY_LEFT:
            rana.coordinate.x -= LARGHEZZA_RANA;
            if (controlloLimiti) {
                rana.coordinate.x += LARGHEZZA_RANA;
            }
            break;
        default:
            break;
        }

        close(p[0]);
        write(p[1], &rana, sizeof(Oggetto));
    }
}
