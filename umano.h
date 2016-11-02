#ifndef UMANO_H
#define UMANO_H

#include <giocatore.h>
#include <tavolo.h>

class Umano : public Giocatore
{
public:
    int calcolaMossa(Tavolo tavolo, int turno) const;

};

#endif // UMANO_H
