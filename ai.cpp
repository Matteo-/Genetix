#include "ai.h"
#include <iostream>
#include <time.h>
#include <QVector>
using namespace std;

AI::AI()
{
    srand(time(NULL));
}

int AI::calcolaMossa(Tavolo tavolo, int turno) const
{
    QVector<int> mosse;
    int indice;

    mosse = tavolo.mosseValide(turno);
    indice = rand()%mosse.size();



    return mosse.at(indice);
}

