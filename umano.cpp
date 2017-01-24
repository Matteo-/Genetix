#include "umano.h"
#include <iostream>
using namespace std;

int Umano::istanze = 0;

Umano::Umano(): score(0)
{
    //debug
    istanze++;
    std::cout<<"creating Umano ["<<istanze<<"]....OK\n";
    //debug
}

Umano::~Umano()
{
    //debug
    std::cout<<"deleting Umano ["<<istanze<<"]....";
    istanze--;
    std::cout<<"OK\n";
    //debug
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

float Umano::getScore() const
{
    return score;
}

bool Umano::operator<(const Player &a) const
{
    return score < a.getScore();
}

void Umano::resetScore()
{
    score = 0;
}
