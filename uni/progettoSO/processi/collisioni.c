#include "lib.h"
#include "collisioni.h"
#include "funzioniGenerali.h"

bool controlloCollisioniRanaProiettile(Coordinate proiettileNemico, Coordinate ranocchio)
{
    if (proiettileNemico.x >= ranocchio.x &&
        proiettileNemico.x <= ranocchio.x + LARGHEZZA_RANA &&
        proiettileNemico.y >= ranocchio.y &&
        proiettileNemico.y <= ranocchio.y + ALTEZZA_RANA)
    {
        return true;
    }

    return false;
}

bool controlloCollisioniProiettili(Coordinate proiettile, Coordinate proiettileNemico)
{
    if (proiettile.x == proiettileNemico.x &&
        proiettile.y == proiettileNemico.y)
    {
        return true;
    }

    return false;
}

bool controlloCollisioniProiettiliAuto(Coordinate proiettile, Coordinate veicolo, int larghezza)
{

    if (proiettile.x >= veicolo.x &&
        proiettile.x <= veicolo.x + larghezza &&
        proiettile.y >= veicolo.y &&
        proiettile.y <= veicolo.y + ALTEZZA_VEICOLI)
    {
        return true;
    }

    return false;
}

void controlloCollisioniMacchine(int pRana[DUE], Oggetto macchinina[], Oggetto ranocchio, int *vite, Oggetto camioncino[],int gameDifficulty)
{
    int i, invincibilita = CINQUE;
    for (i = 0; i < CINQUE; i++)
    {
        if (invincibilita-- <= 0 && macchinina[i].coordinate.x < ranocchio.coordinate.x && (macchinina[i].coordinate.x + LARGHEZZA_MACCHINA) > ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
        {
            invincibilita = CINQUE;
            (*vite)--;
            posizioneInizialeRana(pRana, ranocchio,gameDifficulty);
            write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
            clear();
        }
    }
    for (i = ZERO; i < TRE; i++)
    {
        if (invincibilita-- <= ZERO && camioncino[i].coordinate.x < ranocchio.coordinate.x && (camioncino[i].coordinate.x + LARGHEZZA_CAMION) > ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
        {
            invincibilita = CINQUE;
            (*vite)--;
            posizioneInizialeRana(pRana, ranocchio,gameDifficulty);
            write(pRana[WRITE], &ranocchio, sizeof(Oggetto));
            clear();
        }
    }
}

bool proiettiliVeicoli(Oggetto proiettile, Oggetto proiettileNemico[], Coordinate veicolo, int larghezza, bool hitProiettile[])
{
    int j;

    for (j = ZERO; j < TRE; j++)
    {
        if (controlloCollisioniProiettiliAuto(proiettileNemico[j].coordinate, veicolo, larghezza))
        {
            hitProiettile[j] = true;
        }
    }

    if (controlloCollisioniProiettiliAuto(proiettile.coordinate, veicolo, larghezza))
    {
        return true;
    }

    return false;
}