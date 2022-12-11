#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#define ALTEZZA_RANA 3
#define LARGHEZZA_RANA 5
#define ALTEZZA_SCHERMO 35
#define LARGHEZZA_SCHERMO 140
#define ALTEZZA_TRONCHI 3
#define LARGHEZZA_TRONCHI 15

#define COLORE_RANA 10
#define COLORE_MARCIAPIEDE 20
#define COLORE_AUTOSTRADA 30
#define COLORE_TRONCHI 40

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    int id;
    Coordinate coordinate;
} Oggetto;
