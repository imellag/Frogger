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

#define ALTEZZA_AUTOSTRADA 9
#define INIZIO_AUTOSTRADA 20

#define ALTEZZA_PRATO 3
#define INIZIO_PRATO 17

#define ALTEZZA_FIUME 9
#define INIZIO_FIUME 8

#define INIZIO_MARCIAPIEDE 29
#define ALTEZZA_MARCIAPIEDE 3

#define COLORE_RANA 10
#define COLORE_MARCIAPIEDE 20
#define COLORE_AUTOSTRADA 30
#define COLORE_TRONCHI 40

#define READ 0
#define WRITE 1

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    int id;
    Coordinate coordinate;
    int velocita;
    pid_t pid;
} Oggetto;

enum oggetti {RANA, PROIETTILE, PROIETTILE_OUT, TRONCO0, TRONCO1, TRONCO2, MACCHINA0, MACCHINA1, MACCHINA2};
enum numeri {ZERO, UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, DIECI};

void colori();
void dimensioneFinestra(int maxx, int maxy);
void stampaRana(Coordinate rana);
void stampaVite(int vite);

void stampaTronco(Coordinate tronco);
void stampaMacchina(Coordinate macchina);
int controlloPosizione(Coordinate rana);
