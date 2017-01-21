#ifndef AI_H
#define AI_H

#include "player.h"
#include "table.h"
#include "brain.h"

class AI : public Player
{
public:
    friend AI* operator+(const AI&, const AI&);
    AI(Brain *b);
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(int s);

private:
    int score;
    Brain *brain;
};

#endif // AI_H
