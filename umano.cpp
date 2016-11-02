#include "umano.h"
#include <iostream>
using namespace std;

int Umano::calcolaMossa(Tavolo tavolo, int turno) const
{
    int mossa;
    tavolo.stampa();
    cout << "tocca al giocatore " << turno << " (da 0 a 5)" << endl;
    cin >> mossa;
    return mossa;
}
