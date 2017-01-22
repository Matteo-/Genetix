#ifndef AI_H
#define AI_H

#include "player.h"
#include "table.h"
#include "brain.h"

class AI : public Player
{
public:
    AI(Brain *b);
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(int s);
    float getScore() const;
    void resetScore();
    bool operator<(const Player &) const;
    friend AI* operator+(const AI&, const AI&);
    friend bool operator<(const AI &a, const AI &b);
    friend bool operator<=(const AI &a, const AI &b);
    friend bool operator>(const AI &a, const AI &b);
    friend bool operator>=(const AI &a, const AI &b);

private:
    int score;
    Brain *brain;
};

#endif // AI_H
