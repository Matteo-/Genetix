#ifndef UMANO_H
#define UMANO_H

#include "player.h"
#include "table.h"

class Umano : public Player
{
public:
    Umano();
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(int s);
private:
    int score;
};

#endif // UMANO_H
