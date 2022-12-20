#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#define ALTEZZA_RANA 3
#define LARGHEZZA_RANA 5
#define ALTEZZA_SCHERMO 35
#define LARGHEZZA_SCHERMO 140
#define ALTEZZA_TRONCHI 3
#define LARGHEZZA_TRONCHI 15
#define LARGHEZZA_MACCHINA 6

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

enum oggetti {RANA,PROIETTILE,PROIETTILE_OUT,TRONCO0,TRONCO1,TRONCO2,MACCHINA0,MACCHINA1,MACCHINA2};
enum numeri {ZERO, UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, DIECI};

