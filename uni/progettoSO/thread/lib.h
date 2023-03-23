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
    MACCHINA10 = 79,
    CAMION0,
    CAMION8 = 88,
    TRONCONEMICO0,
    TRONCONEMICO4 = 93,
    PROIETTILE_NEMICO0,
    PROIETTILE_NEMICO2 = 96,
    PROIETTILE_NEMICO0_OUT,
    PROIETTILE_NEMICO2_OUT = 99,
    MACCHINA0_OUT,
    MACCHINA10_OUT = 110,
};

// serve per togliere vari magic numbers
enum numeri
{
    ZERO,
    UNO,
    DUE,
    TRE,
    QUATTRO,
    CINQUE,
    SEI,
    SETTE,
    OTTO,
    NOVE,
    DIECI
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

// vale sia per i camion che per le macchine
#define ALTEZZA_VEICOLI 3

// dimensioni macchina e numero di macchine
#define LARGHEZZA_MACCHINA 6
#define NUMERO_MACCHINE 5
#define MAX_MACCHINE 11

// dimensioni camion e numero di camion
#define LARGHEZZA_CAMION 10
#define NUMERO_CAMION 3
#define MAX_CAMION 9

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
#define NUMERO_CORSIE_FIUME 5

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
#define TEMPO_INIZIALE 40
#define TEMPO_TANA 5
#define PUNTEGGIO_INIZIALE 2000
#define PUNTEGGIO_UCCISIONE 50
#define PUNTEGGIO_TANA 150
#define TEMPO_SPAWN_NEMICO 7
// numero massimo di proiettili che la rana può avere contemporaneamente a schermo
#define NUMERO_PROIETTILI 30

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
    int difficolta;
} Oggetto;

typedef struct
{
    Oggetto proiettile;
    Oggetto rana;
    int numeroProiettile;
} Proiettile;

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

typedef struct
{
    Oggetto veicolo;
    int velocitaCorsia;
    int direzioneCorsia;

} parametriVeicolo;

extern pthread_mutex_t mutex;