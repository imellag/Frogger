#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#define ALTEZZA_RANA 3
#define LARGHEZZA_RANA 5
#define ALTEZZA_SCHERMO 30
#define LARGHEZZA_SCHERMO 135

#define COLORE_MARCIAPIEDE 10

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    int id;
    Coordinate coordinate;
} Oggetto;
