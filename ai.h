#ifndef AI_H
#define AI_H

#include <giocatore.h>
#include <tavolo.h>

class AI : public Giocatore
{
public:
    AI();
    int calcolaMossa(Tavolo tavolo, int turno) const;
};

#endif // AI_H
