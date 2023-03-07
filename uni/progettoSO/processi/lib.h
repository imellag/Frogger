#pragma once
#include <ncursesw/curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

// enumerazione utilizzata per gli id dei vari oggetti presenti sullo schermo
enum oggetti
{
    RANA,
    SPAWN_PROIETTILE,
    PROIETTILE0,
    PROIETTILE1,
    PROIETTILE2,
    PROIETTILE3,
    PROIETTILE4,
    PROIETTILE5,
    PROIETTILE6,
    PROIETTILE7,
    PROIETTILE8,
    PROIETTILE9,
    PROIETTILE10,
    PROIETTILE11,
    PROIETTILE12,
    PROIETTILE13,
    PROIETTILE14,
    PROIETTILE15,
    PROIETTILE16,
    PROIETTILE17,
    PROIETTILE18,
    PROIETTILE19,
    PROIETTILE20,
    PROIETTILE21,
    PROIETTILE22,
    PROIETTILE23,
    PROIETTILE24,
    PROIETTILE25,
    PROIETTILE26,
    PROIETTILE27,
    PROIETTILE28,
    PROIETTILE29,
    PROIETTILE0_OUT,
    PROIETTILE1_OUT,
    PROIETTILE2_OUT,
    PROIETTILE3_OUT,
    PROIETTILE4_OUT,
    PROIETTILE5_OUT,
    PROIETTILE6_OUT,
    PROIETTILE7_OUT,
    PROIETTILE8_OUT,
    PROIETTILE9_OUT,
    PROIETTILE10_OUT,
    PROIETTILE11_OUT,
    PROIETTILE12_OUT,
    PROIETTILE13_OUT,
    PROIETTILE14_OUT,
    PROIETTILE15_OUT,
    PROIETTILE16_OUT,
    PROIETTILE17_OUT,
    PROIETTILE18_OUT,
    PROIETTILE19_OUT,
    PROIETTILE20_OUT,
    PROIETTILE21_OUT,
    PROIETTILE22_OUT,
    PROIETTILE23_OUT,
    PROIETTILE24_OUT,
    PROIETTILE25_OUT,
    PROIETTILE26_OUT,
    PROIETTILE27_OUT,
    PROIETTILE28_OUT,
    PROIETTILE29_OUT,
    TRONCO0,
    TRONCO1,
    TRONCO2,
    TRONCO3,
    TRONCO4,
    MACCHINA0,
    MACCHINA1,
    MACCHINA2,
    MACCHINA3,
    MACCHINA4,
    MACCHINA5,
    MACCHINA6,
    MACCHINA7,
    MACCHINA8,
    MACCHINA9,
    MACCHINA10,
    CAMION0,
    CAMION1,
    CAMION2,
    CAMION3,
    CAMION4,
    CAMION5,
    CAMION6,
    CAMION7,
    CAMION8,
    TRONCONEMICO0,
    TRONCONEMICO1,
    TRONCONEMICO2,
    TRONCONEMICO3,
    TRONCONEMICO4,
    PROIETTILE_NEMICO0,
    PROIETTILE_NEMICO1,
    PROIETTILE_NEMICO2,
    PROIETTILE_NEMICO0_OUT,
    PROIETTILE_NEMICO1_OUT,
    PROIETTILE_NEMICO2_OUT
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

// dimensioni delle sprite delle scritte di inizio e fine partita
#define ALTEZZA_SPRITE 5
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
#define LARGHEZZA_CAMION 13
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

// lettura/scrittura della pipe
#define READ 0
#define WRITE 1

// variabili utili per il le statistiche e il controllo della partita
#define MAX_VITE 5
#define TEMPO_INIZIALE 60
#define TEMPO_TANA 5
#define PUNTEGGIO_INIZIALE 2000
#define PUNTEGGIO_UCCISIONE 50
#define PUNTEGGIO_TANA 150
#define SPAWN_NEMICO 6
#define NUMERO_PROIETTILI 30

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

// struttura per il controllo del tempo di gioco
typedef struct
{
    int id;
    bool tempo;
    pid_t pid;
} Tempo;

// struttura usata per restituire un colore da una funzione
typedef struct
{
    int r;
    int g;
    int b;
} Colore;

typedef struct {
    int difficolta;
    bool audio;
    char* nome;
} Avvio;


#define LUNGHEZZA_MAX 25