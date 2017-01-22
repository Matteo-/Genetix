#ifndef GIOCATORE_H
#define GIOCATORE_H

#include "table.h"

/**
 * @brief classe astratta che desscrive l'interfaccia pubblica di un giocatore
 */
class Player
{
public:
    //da determinare i parametri (mie pedine, pedine avversarie)
    virtual int calcolaMossa(const Table &table, int turno) const = 0;
    virtual void addScore(int s) = 0;
    virtual bool operator<(const Player &) const = 0;
    virtual float getScore() const = 0;
    virtual void resetScore() = 0;
    virtual ~Player() {}
private:
};

#endif // GIOCATORE_H
