#include "lib.h"
#include "funzioniGenerali.h"
#include "fiume.h"
#include "autostrada.h"

char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI] = {"<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>", "<~~~~~~~~~~~~~>"};
char spriteNemicosulTronco[ALTEZZA_NEMICO][LARGHEZZA_TRONCHI] = {"<~~~~~o\\/o~~~~>", "<~~~~~:||:~~~~>", "<~~~~~./\\.~~~~>"};

int funzTronchi(int p[], int gameDifficulty)
{
    int i;
    pid_t tronco[MAX_TRONCHI];

    int velocita[MAX_TRONCHI];
    int spostamento;

    // array che tiene le direzioni del tronco
    for (i = 0; i < NUMERO_TRONCHI + gameDifficulty; i++)
        velocita[i] = 1;

    // la direzione delle corsie la genero casualmente
    spostamento = rand() % 2;

    if (spostamento == 0)
        spostamento = -1;
    else
        spostamento = 1;

    // genero i processi dei tronchi
    for (i = 0; i < NUMERO_TRONCHI + gameDifficulty; i++)
    {
        tronco[i] = fork();
        if (tronco[i] < 0)
        {
            perror("Error");
        }
        else if (tronco[i] == 0)
        {
            funzTronco(p, i, velocita[i] * spostamento, gameDifficulty);
            exit(0);
        }
    }
}

// funzione che gestisce il movimento del tronco
void funzTronco(int p[], int numeroTronco, int velocita, int gameDifficulty)
{
    Oggetto tronco;

    // uso come seme il il pid per generare velocità differenti
    srand(getpid());

    // gli assegno un usleep randomica che corrisponde alla velocità con cui andrà il tronco
    //(più è alta la difficoltà e più vanno veloci)
    int tempoRandom = TEMPO_TRONCO_MIN + rand() % (TEMPO_TRONCO_MIN + TEMPO_TRONCO_MAX) - (2500 * gameDifficulty);

    // inzializzo le coordinate e gli altri campi
    tronco.coordinate.y = INIZIO_FIUME + numeroTronco * ALTEZZA_TRONCHI;
    tronco.coordinate.x = rand() % (LARGHEZZA_SCHERMO - LARGHEZZA_TRONCHI);
    tronco.id = TRONCO0 + numeroTronco;
    tronco.velocita = velocita;
    tronco.pid = getpid();

    close(p[READ]);

    while (true)
    {
        write(p[WRITE], &tronco, sizeof(Oggetto));

        tronco.coordinate.x += tronco.velocita;
        // se arriva al limite dello shcermo cambia direzione
        if (controlloLimitiTronco(tronco.coordinate))
            tronco.velocita = tronco.velocita * -1;
        usleep(tempoRandom);
    }
}

void stampaTronco(WINDOW *finestraGioco, Coordinate tronco)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));

    for (i = 0; i < ALTEZZA_TRONCHI; i++)
    {
        for (j = 0; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, tronco.y + i, tronco.x + j, "%c", spriteTronchi[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_SFONDO_TRONCHI));
}

void stampaNemico(WINDOW *finestraGioco, Coordinate nemico)
{
    int i, j;

    wattron(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO)); // ROSSO

    for (i = 0; i < ALTEZZA_NEMICO; i++)
    {
        for (j = 0; j < LARGHEZZA_TRONCHI; j++)
            mvwprintw(finestraGioco, nemico.y + i, nemico.x + j, "%c", spriteNemicosulTronco[i][j]);
    }

    wattroff(finestraGioco, COLOR_PAIR(COLORE_NEMICI_TRONCO));
}

// funzione per generare i proiettili nemici
void funzProiettileNemico(Coordinate tronco, int p[], int i, int gameDifficulty, bool audio)
{
    pid_t proiettileNemico;

    if (audio)
        system("ffplay -nodisp ../file_audio/sparo.mp3 2> /dev/null &");
    proiettileNemico = fork();
    if (proiettileNemico < 0)
    {
        printw("Error");
        exit(EXIT_FAILURE);
    }
    else if (proiettileNemico == 0)
    {
        movimentoProiettileNemico(tronco, p, i, gameDifficulty);
        exit(EXIT_FAILURE);
    }
}

// funzione che gestisce il movimento dei proiettili nemici
void movimentoProiettileNemico(Coordinate tronco, int p[], int i, int gameDifficulty)
{
    Oggetto proiettile;
    proiettile.coordinate.x = tronco.x + LARGHEZZA_TRONCHI / 2;
    proiettile.coordinate.y = tronco.y + ALTEZZA_CORSIE;
    proiettile.id = PROIETTILE_NEMICO0 + i;
    proiettile.pid = getpid();
    close(p[READ]);
    while (true)
    {
        if (proiettile.coordinate.y >= ALTEZZA_SCHERMO + (6 * gameDifficulty))
        {
            proiettile.id = PROIETTILE_NEMICO0_OUT + i;
            write(p[WRITE], &proiettile, sizeof(Oggetto));
            break;
        }

        write(p[WRITE], &proiettile, sizeof(Oggetto));
        usleep(50000);
        proiettile.coordinate.y++;
    }
}