#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <locale.h>

// enumerazione utilizzata per gli id dei vari oggetti presenti sullo schermo
enum oggetti
{
    RANA,
    TEMPO,
    SPAWN_PROIETTILE,
    PAUSA,
    PROIETTILE0 = 4,
    PROIETTILE29 = 33,
    PROIETTILE0_OUT,
    PROIETTILE29_OUT = 63,
    TRONCO0,
    TRONCO4 = 68,
    MACCHINA0,
    MACCHINA6 = 72,
    CAMION0,
    CAMION5 = 75,
    TRONCONEMICO0,
    TRONCONEMICO4 = 79,
    PROIETTILE_NEMICO0,
    PROIETTILE_NEMICO2 = 84,
    PROIETTILE_NEMICO0_OUT,
    PROIETTILE_NEMICO2_OUT = 88,
    MACCHINA0_OUT
};

enum ColorPair
{
    COLORE_VERDE_NERO = 1,
    COLORE_NERO_VERDE,
    COLORE_NEMICI_TRONCO,
    COLORE_SFONDO_MARCIAPIEDE,
    COLORE_SFONDO_AUTOSTRDA,
    COLORE_SFONDO_PRATO,
    COLORE_SFONDO_FIUME,
    COLORE_SFONDO_TRONCHI,
    COLORE_NERO,
    COLORE_SCRITTE_INFO,
    COLORE_RANA_SFONDO

};

// difficoltà di gioco disponibili
enum gameDifficulty
{
    EASY,
    MEDIUM,
    HARD
};

// enumerazione per i colori dello sfondo del proiettile
enum coloriProiettile
{
    PROIETTILE_NEMICO,
    PROIETTILE_MARCIAPIEDE,
    PROIETTILE_AUTOSTRADA,
    PROIETTILE_PRATO,
    PROIETTILE_FIUME,
    PROIETTILE_TRONCHI
};

// posizione dove vengono portati gli oggetti come i proiettili quando non devono essere visualizzati a schermo
#define FUORI_MAPPA -5

// input da tastiera
#define ESC 27
#define A 65
#define a 97
#define D 68
#define d 100
#define Q 81
#define q 113
#define S 83
#define s 115
#define W 87
#define w 119
#define SPACEBAR ' '

// altezza generale di tutte le corsie (fiume, autostrada ecc.)
#define ALTEZZA_CORSIE 3

// dimensioni delle sprite delle scritte di inizio e fine partita
#define ALTEZZA_SPRITE 6
#define LARGHEZZA_SPRITE_INIZIO 45
#define LARGHEZZA_SPRITE_FINE 52

// dimensioni rana
#define ALTEZZA_RANA 3
#define LARGHEZZA_RANA 5
#define POSIZIONE_INIZIALE_RANA_Y 29

// dimensioni totali del gioco
#define ALTEZZA_SCHERMO 32
#define LARGHEZZA_SCHERMO 140

// dimensioni tronco
#define ALTEZZA_TRONCHI 3
#define LARGHEZZA_TRONCHI 15
#define NUMERO_TRONCHI 3
#define MAX_TRONCHI 5

// dimensioni camion e numero di camion
#define LARGHEZZA_CAMION 10
#define NUMERO_CAMION 3
#define MAX_CAMION 5

// dimensioni autostrada e coordinate nello schermo
#define ALTEZZA_AUTOSTRADA 9
#define INIZIO_AUTOSTRADA 20
#define NUMERO_CORSIE 3
#define MAX_CORSIE 5

// dimensioni prato e coordinate nello schermo
#define ALTEZZA_PRATO 3
#define INIZIO_PRATO 17

// dimensioni fiume e coordinate nello schermo
#define ALTEZZA_FIUME 9
#define INIZIO_FIUME 8

// dimensioni marciapiede e coordinate nello schermo
#define INIZIO_MARCIAPIEDE 29
#define ALTEZZA_MARCIAPIEDE 3

// dimensioni tane e coordinate nello schermo
#define ALTEZZA_TANE 3
#define LARGHEZZA_TANE 15
#define INIZIO_TANE 5
#define NUMERO_TANE 5

// dimensioni e numero di nemici
#define ALTEZZA_NEMICO 3
#define LARGHEZZA_NEMICO 5
#define NUMERO_NEMICI 3
#define NUMERO_TRONCHI 3
#define MAX_PROIETTILI_NEMICI 5

// definisco un numero per ciascuno colore che viene utilizzato
#define COLORE_RANA 11
#define COLORE_MARCIAPIEDE 12
#define COLORE_AUTOSTRADA 13
#define COLORE_TRONCHI 14
#define COLORE_TANA 15
#define COLORE_NEMICI 16
#define COLORE_FIUME 17
#define COLORE_MACCHINA0 20
#define COLORE_CAMION0 40
#define COLORE_PRATO 18

// lettura/scrittura della pipe
#define READ 0
#define WRITE 1

// variabili utili per il le statistiche e il controllo della partita
#define MAX_VITE 5
#define TEMPO_INIZIALE 50
#define TEMPO_TANA 5
#define PUNTEGGIO_INIZIALE 2000
#define PUNTEGGIO_UCCISIONE 50
#define PUNTEGGIO_TANA 150
#define TEMPO_SPAWN_NEMICO 7

// numero massimo di proiettili che la rana può avere contemporaneamente a schermo
#define NUMERO_PROIETTILI 20

// offset per spostare la finestra dello schermo pù al centro
#define INIZIO_ALTEZZA_FINESTRA 5
#define INIZIO_LARGHEZZA_FINESTRA 20

// struttura per le coordinate
typedef struct
{
    int x;
    int y;
} Coordinate;

// struttura degli oggetti presenti nello schermo
typedef struct
{
    int id;
    Coordinate coordinate;
    int velocita;
    pid_t pid;
} Oggetto;

// struttura usata per restituire un colore da una funzione
typedef struct
{
    int r;
    int g;
    int b;
} Colore;

// struttura per i dati restituiti dalla funzione di avvio della partita
typedef struct
{
    int difficolta;
    bool audio;
    Colore colore;
} Avvio;