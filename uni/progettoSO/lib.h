#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

// enumerazione utilizzata per gli id dei vari oggetti presenti sullo schermo
enum oggetti {RANA, PROIETTILE, PROIETTILE_OUT, TRONCO0, TRONCO1, TRONCO2, MACCHINA0, 
MACCHINA1, MACCHINA2, MACCHINA3, MACCHINA4, CAMION0, CAMION1,CAMION2};

// serve per togliere vari magic numbers
enum numeri {ZERO, UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, DIECI};

// difficoltà di gioco disponibili
enum gameDifficulty {EASY, MEDIUM, HARD};

// input da tastiera
#define q 113
#define SPACEBAR ' '

// dimensioni rana
#define ALTEZZA_RANA 3
#define LARGHEZZA_RANA 5

// dimensioni totali del gioco
#define ALTEZZA_SCHERMO 35
#define LARGHEZZA_SCHERMO 140

// dimensioni tronco
#define ALTEZZA_TRONCHI 3
#define LARGHEZZA_TRONCHI 15

// dimensioni macchina
#define LARGHEZZA_MACCHINA 6

// dimensioni camion
#define LARGHEZZA_CAMION 13

// dimensioni autostrada
#define ALTEZZA_AUTOSTRADA 9
#define INIZIO_AUTOSTRADA 20

// dimensioni prato
#define ALTEZZA_PRATO 3
#define INIZIO_PRATO 17

// dimensioni fiume
#define ALTEZZA_FIUME 9
#define INIZIO_FIUME 8

// dimensioni marciapiede
#define INIZIO_MARCIAPIEDE 29
#define ALTEZZA_MARCIAPIEDE 3

// dimensioni tane
#define ALTEZZA_TANE 3
#define LARGHEZZA_TANE 15
#define INIZIO_TANE 5
#define NUMERO_TANE 5

// dimensioni nemici
#define ALTEZZA_NEMICO 3
#define LARGHEZZA_NEMICO 4

// definisco un numero per ciascuno colore che viene utilizzato
#define COLORE_RANA 10
#define COLORE_MARCIAPIEDE 20
#define COLORE_AUTOSTRADA 30
#define COLORE_TRONCHI 40
#define COLORE_TANA 50

// lettura/scrittura della pipe
#define READ 0
#define WRITE 1

// struttura per le coordinate
typedef struct {
    int x;
    int y;
} Coordinate;

// struttura degli oggetti presenti nello schermo
typedef struct {
    int id;
    Coordinate coordinate;
    int velocita;
    pid_t pid;
} Oggetto;
