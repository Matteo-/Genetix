#ifndef GIOCATORE_H
#define GIOCATORE_H

#include "table.h"

/**
 * @brief The Giocatore class
 * classe astratta che desscrive l'interfaccia pubblica di un giocatore
 */
class Player
{
public:
    //da determinare i parametri (mie pedine, pedine avversarie)
    virtual int calcolaMossa(const Table &table, int turno) const = 0;
    virtual void addScore(int s) = 0;
    virtual ~Player() {}
};

#endif // GIOCATORE_H
