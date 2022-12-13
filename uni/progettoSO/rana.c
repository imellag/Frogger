#include "lib.h"
#include "rana.h"

void funzRana(Oggetto rana, int p[]) {
    int inputMovimento;
    
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
        default:
            break;
        }

        close(p[0]);
        write(p[1], &rana, sizeof(Oggetto));
    }
}
