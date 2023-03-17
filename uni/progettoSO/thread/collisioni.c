#include "lib.h"
#include "collisioni.h"
#include "funzioniGenerali.h"

bool controlloCollisioniRanaProiettile(Coordinate proiettileNemico, Coordinate ranocchio)
{

    if (proiettileNemico.x >= ranocchio.x &&
        proiettileNemico.x <= ranocchio.x + LARGHEZZA_RANA &&
        proiettileNemico.y >= ranocchio.y &&
        proiettileNemico.y <= ranocchio.y + ALTEZZA_RANA)
        return true;

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

bool controlloCollisioniProiettiliAuto(Coordinate proiettile, Oggetto veicolo, int larghezza)
{
    if (veicolo.velocita < ZERO)
    {
        if (proiettile.x <= veicolo.coordinate.x &&
            proiettile.x >= veicolo.coordinate.x - larghezza &&
            proiettile.y >= veicolo.coordinate.y &&
            proiettile.y <= veicolo.coordinate.y + ALTEZZA_VEICOLI)
            return true;
    }
    else
    {
        if (proiettile.x >= veicolo.coordinate.x &&
            proiettile.x <= veicolo.coordinate.x + larghezza &&
            proiettile.y >= veicolo.coordinate.y &&
            proiettile.y <= veicolo.coordinate.y + ALTEZZA_VEICOLI)
            return true;
    }

    return false;
}

void controlloCollisioniMacchine(Oggetto macchinina[], Oggetto ranocchio, int *vite, Oggetto camioncino[], int gameDifficulty)
{
    int i, invincibilita = CINQUE;
    for (i = 0; i < CINQUE; i++)
    {
        if (invincibilita-- <= 0 && macchinina[i].coordinate.x < ranocchio.coordinate.x && (macchinina[i].coordinate.x + LARGHEZZA_MACCHINA) > ranocchio.coordinate.x && macchinina[i].coordinate.y == ranocchio.coordinate.y)
        {
            invincibilita = CINQUE;
            (*vite)--;
         //   posizioneInizialeRana(pRana, ranocchio, gameDifficulty);
            clear();
        }
    }
    for (i = ZERO; i < TRE; i++)
    {
        if (invincibilita-- <= ZERO && camioncino[i].coordinate.x < ranocchio.coordinate.x && (camioncino[i].coordinate.x + LARGHEZZA_CAMION) > ranocchio.coordinate.x && camioncino[i].coordinate.y == ranocchio.coordinate.y)
        {
            invincibilita = CINQUE;
            (*vite)--;
           // posizioneInizialeRana(pRana, ranocchio, gameDifficulty);
            clear();
        }
    }
}

bool proiettiliVeicoli(Oggetto proiettile, Oggetto proiettileNemico[], Oggetto veicolo, int larghezza, bool hitProiettile[])
{
    int j;

    // DA RIVEDERE
    for (j = ZERO; j < MAX_TRONCHI; j++)
    {
        if (controlloCollisioniProiettiliAuto(proiettileNemico[j].coordinate, veicolo, larghezza))
            hitProiettile[j] = true;
    }
    if (controlloCollisioniProiettiliAuto(proiettile.coordinate, veicolo, larghezza))
        return true;

    return false;
}

bool controlloCollisioneNemicoProiettile(Oggetto proiettile, Oggetto tronco, bool nemico)
{
    bool flag = false;

    if (proiettile.coordinate.x >= tronco.coordinate.x &&
        proiettile.coordinate.x <= tronco.coordinate.x + LARGHEZZA_TRONCHI &&
        proiettile.coordinate.y == tronco.coordinate.y + DUE && nemico)

        flag = true;

    return flag;
}