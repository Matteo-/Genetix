#ifndef TAVOLO_H
#define TAVOLO_H

#include <QVector>
using namespace std;

/**
 * @brief The Tavolo class
 * classe che implementa e gestisce il campo da gioco
 */
class Tavolo
{
public:
    Tavolo();
    void stampa() const;
    QVector< QVector<int> > get() const;
    bool fineGioco() const;
    int calcolaVincitore() const;
    QVector<int> mosseValide(int turno) const;
    int eseguiMossa(int turno, int mossa);
    void inizializza();
private:
    int numMosse(int turno) const;

    QVector< QVector<int> > buca;
    const int fagioli_inizio;
};

#endif // TAVOLO_H
