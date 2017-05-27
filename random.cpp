#include "random.h"
#include "iostream"
using namespace std;

Random::Random(): statistics_()
{

}

int Random::calcolaMossa(const Table &table, int turno) const
{
    QVector<int> mosse;
    mosse = table.mosseValide(turno);
    int m = rand() % mosse.size();
    //cout << "[n mosse] "<< mosse.size() <<"[mossa random] " << m << "\n"; //debug
    //table.stampa();
    //getchar();
    return mosse[m];
}
