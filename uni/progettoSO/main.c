#include "lib.h"
#include "rana.h"
#include "marciapiede.h"
#include "autostrada.h"
#include "prato.h"
#include "fiume.h"
#include "tane.h"

void colori();
void dimensioneFinestra(int maxx, int maxy);
void stampaRana(Coordinate rana);
void stampaVite(int vite);

char spriteProiettile = '^';
char spriteRana[ALTEZZA_RANA][LARGHEZZA_RANA + 1] = {" o.o ", "+-|-+", "\\-|-/"};
char spriteTronchi[ALTEZZA_RANA][LARGHEZZA_TRONCHI + 1] = {"<~~~~~~~~>", "<~~~~~~~~>", "<~~~~~~~~>"};
char spriteMacchine[ALTEZZA_RANA][LARGHEZZA_MACCHINA]= {" /^\\_","| __ |","o   o"};
void stampaTronco(Coordinate tronco);
void stampaMacchina(Coordinate macchina);
int main()
{
    srand(time(NULL));

    int maxx, maxy;
    int maxx_precedente, maxy_precedente;
    int tempo = 30, punteggio = 0, vite = 3;
    Oggetto arrayTronchi[3];
    int velocitaTronchi[3];
    Coordinate rana;
    Coordinate tronco;
    tronco.x = -1;
    tronco.y = -1;
    rana.x = 0;
    rana.y = ALTEZZA_SCHERMO - 6;

    initscr();
    noecho();
    curs_set(false);
    cbreak();
    colori();
    keypad(stdscr, true);
    getmaxyx(stdscr, maxy, maxx);
    // WINDOW *finestraMarciapiede = newwin(3, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 6, 0);
    /* -6 perché lascio tre righe in basso per le info (tempo ecc) e il marciapiede è alto 3 */
    // WINDOW *finestraAutostrada = newwin(9, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 15, 0);
    /* -14 perché è alto 9 e c'era prima il marciapiede di 5 */
    // WINDOW *finestraPrato = newwin(3, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 18, 0);
    // alto 3
    //  WINDOW *finestraFiume = newwin(9, LARGHEZZA_SCHERMO, ALTEZZA_SCHERMO - 27, 0);
    // alto 9
    int p[2];
    if (pipe(p) == -1)
    {
        perror("Error\n");
        exit(-1);
    }

    clear();
    refresh();

    dimensioneFinestra(maxx, maxy);

    /* Lascio due righe vuote in basso per scrivere il tempo/punteggio ecc. dopo.
    Quindi dato che l'altezza è il numero totale di "pixel" ma effettivamente poi
    le coordinate partono da 0 tolgo 3 alla rana rispetto all'altezza dello schermo
    per centrarla con il marciapiede */

    for (int i = 0; i < 3; i++)
    {
        velocitaTronchi[i] = 2 + rand() % (4 - 2 + 1);
    }

    stampaVite(vite);

    funzMarciapiede();
    funzAutostrada();
    funzPrato();
    funzFiume();
    stampaRana(rana);
    mvwprintw(stdscr, 1, LARGHEZZA_SCHERMO / 2 - 4, "Score: %d", punteggio);
    mvwprintw(stdscr, ALTEZZA_SCHERMO - 2, LARGHEZZA_SCHERMO / 2 - 9, "Tempo rimanente: %d", tempo);
    refresh();

   

    pid_t pidRana, pidMacchine, pidTronchi, pidNemici, pidProiettile;
    pidRana = fork();

    if (pidRana < 0)
    {
        printw("Error");
        exit(-1);
    }
    else if (pidRana == 0)
    {
        funzRana(p);
    }
    else
    {
        funzAuto(p);
        funzTronchi(p);
        erase();
        
        int i;
        Oggetto pacchetto;
        Oggetto proiettilino;
        Oggetto tronchino[3];
        Oggetto macchinina[3];
        proiettilino.coordinate.x = -1;
        proiettilino.coordinate.y = -1;
        Oggetto ranocchio;
        _Bool fuorischermo = false;

        close(p[1]);

        while (true)
        {

            read(p[0], &pacchetto, sizeof(Oggetto));
            switch (pacchetto.id)
            {
            case RANA:
                ranocchio = pacchetto;
                break;

            case PROIETTILE:
                proiettilino = pacchetto;
                fuorischermo = false;
                break;
            case PROIETTILE_OUT:
                fuorischermo = true;
                break;
            case TRONCO0:
                tronchino[0] = pacchetto;
                break;
            case TRONCO1:
                tronchino[1] = pacchetto;
                break;
            case TRONCO2:
                tronchino[2] = pacchetto;
                break;

            case MACCHINA0:
                macchinina[0] = pacchetto;
                break;

            case MACCHINA1:
                macchinina[1] = pacchetto;
                break;

            case MACCHINA2:
                macchinina[2] = pacchetto;
                break;

            case q:
                ranocchio = pacchetto;
                break;

            default:
                break;
            }
            maxx_precedente = maxx;
            maxy_precedente = maxy;
            getmaxyx(stdscr, maxy, maxx);
            if (maxx != maxx_precedente || maxy != maxy_precedente)
                clear();

            stampaVite(vite);

            read(pproiettile[0],&proiettilino, sizeof(Oggetto));
            
            stampaVite(spriteRana, vite);
             
            funzMarciapiede(finestraMarciapiede);
            funzAutostrada(finestraAutostrada);
            funzPrato(finestraPrato);
            funzFiume(finestraFiume,arrayTronchi);

            stampaRana(ranocchio.coordinate, spriteRana);
            mvwprintw(stdscr, proiettilino.coordinate.y,proiettilino.coordinate.x,"%c",spriteProiettile);
            mvwprintw(stdscr, 1, LARGHEZZA_SCHERMO / 2 - 4, "Score: %d", punteggio);
            mvwprintw(stdscr, ALTEZZA_SCHERMO - 2, LARGHEZZA_SCHERMO / 2 - 9, "Tempo rimanente: %d", tempo);
            refresh();

            if (ranocchio.id == q)
            {
                endwin();
                kill(pidRana, SIGKILL);
                kill(pidMacchine, SIGKILL);
                kill(pidTronchi, SIGKILL);

                return 0;
            }

            // per ora
        }
    }
}

void colori()
{
    start_color();
    init_color(COLORE_RANA, 75, 890, 20);          // 19/227/5
    init_color(COLORE_MARCIAPIEDE, 388, 270, 102); // 99/69/26
    init_color(COLORE_AUTOSTRADA, 259, 259, 259);  // grigio (per ora), sarebbe 66/66/66 in rgb, convertito
    init_color(COLORE_TRONCHI, 459, 298, 102);     // 117/76/26
    init_pair(1, COLORE_RANA, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLORE_MARCIAPIEDE);
    init_pair(3, COLOR_BLACK, COLORE_AUTOSTRADA);
    init_pair(4, COLOR_BLACK, COLOR_GREEN); // colore prato
    init_pair(5, COLOR_BLACK, COLOR_BLUE);  // colore fiume
    init_pair(6, COLORE_TRONCHI, COLOR_BLUE);
    init_pair(7, COLORE_RANA, COLORE_AUTOSTRADA);
}

void dimensioneFinestra(int maxx, int maxy)
{
    clear();
    while (maxy < ALTEZZA_SCHERMO || maxx < LARGHEZZA_SCHERMO)
    {
        erase();
        mvwprintw(stdscr, maxy / 2, maxx / 2 - 17, "Ingrandisci lo schermo per giocare!"); // -17 per centrare la scritta
        getmaxyx(stdscr, maxy, maxx);
        refresh();
    }

    clear();
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2, LARGHEZZA_SCHERMO / 2 - 32, "Per evitare problemi non diminuire la dimensione della finestra!");
    mvwprintw(stdscr, ALTEZZA_SCHERMO / 2 + 1, LARGHEZZA_SCHERMO / 2 - 7, "Buona fortuna!");
    refresh();
    // sleep(2);
    clear();
    refresh();
}

void stampaRana(Coordinate rana)
{
    int i, j;
    attron(COLOR_PAIR(1));
    for (i = 0; i < ALTEZZA_RANA; i++)
    {
        for (j = 0; j < LARGHEZZA_RANA; j++)
        {
            mvaddch(rana.y + i, rana.x + j, spriteRana[i][j]);
        }
    }
    attroff(COLOR_PAIR(1));
}

void stampaVite(int vite)
{
    Coordinate rana;
    rana.x = LARGHEZZA_SCHERMO - LARGHEZZA_RANA;
    rana.y = ALTEZZA_SCHERMO - ALTEZZA_RANA;
    /* inizio a stamparle da in basso a destra, poi mi sposto
    verso sinistra */
    for (int i = 0; i < vite; i++)
    {
        stampaRana(rana);
        rana.x -= LARGHEZZA_RANA + 1; // lascio 1 di spazio tra le varie vite
    }
}

void stampaTronco(Coordinate tronco)
{

    int i, j;
    attron(COLOR_PAIR(6));
    for (i = 0; i < ALTEZZA_RANA; i++)
    {
        for (j = 0; j < LARGHEZZA_TRONCHI; j++)
            mvaddch(tronco.y + i, tronco.x + j, spriteTronchi[i][j]);
    }
    attroff(COLOR_PAIR(6));
}

void stampaMacchina(Coordinate macchina)
{


    int i, j;
    attron(COLOR_PAIR(6));
    for (i = 0; i < ALTEZZA_RANA; i++)
    {
        for (j = 0; j < LARGHEZZA_MACCHINA; j++)
            mvprintw(macchina.y + i, macchina.x + j,"%c",spriteMacchine[i][j]);
    }
    attroff(COLOR_PAIR(6));

}