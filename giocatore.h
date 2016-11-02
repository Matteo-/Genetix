#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <tavolo.h>

/**
 * @brief The Giocatore class
 * classe astratta che desscrive l'interfaccia pubblica di un giocatore
 */
class Giocatore
{
public:
    //da determinare i parametri (mie pedine, pedine avversarie)
    virtual int calcolaMossa(Tavolo tavolo, int turno) const = 0;
    virtual ~Giocatore() {}

};

#endif // GIOCATORE_H
