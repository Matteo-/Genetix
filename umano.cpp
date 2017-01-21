#include "umano.h"
#include <iostream>
using namespace std;

Umano::Umano(): score(0)
{

}

int Umano::calcolaMossa(const Table &table, int turno) const
{
    int mossa;
    table.stampa();
    cout << "tocca al giocatore " << turno << " (da 0 a 5)" << endl;
    cin >> mossa;
    return mossa;
}

void Umano::addScore(int s)
{
    score += s;
}
