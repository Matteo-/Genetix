#ifndef PARTITA_H
#define PARTITA_H

#include <giocatore.h>
#include <tavolo.h>
#include <QVector>

class Partita
{
public:
    Partita();
    QVector<Giocatore*> run(Giocatore *g1, Giocatore *g2);
private:
    int turno; // 0 o 1
    const int turno_iniziale;
    Tavolo bantumi;
    QVector<Giocatore*> giocatori;
};

#endif // PARTITA_H
