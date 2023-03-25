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
    if (veicolo.velocita < 0)
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

bool proiettiliVeicoli(Oggetto proiettile, Oggetto proiettileNemico[], Oggetto veicolo, int larghezza, bool hitProiettile[],int difficolta)
{
    int j;

    for (j = 0; j < NUMERO_NEMICI+difficolta; j++)
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
        proiettile.coordinate.y == tronco.coordinate.y + 2 && nemico)

        flag = true;

    return flag;
}