#ifndef RANDOM_H
#define RANDOM_H

#include "player.h"
#include "table.h"
#include <stdlib.h>

class Random : public Player
{
public:
    Random();
    ~Random() {}
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(float s) { statistics_.score += s; }
    bool operator<(const Player &b) const {
        if (getScore() < b.getScore()) return true;
        else return false;
    }
    float getScore() const { return statistics_.score;}
    void resetScore() {}

    void win(float s) { addScore(s); statistics_.win++; }
    void lose(float s) { addScore(s); statistics_.lose++; }
    void parity(float s) { addScore(s); statistics_.parity++; }
    stat statistics() { return statistics_; }
private:
    stat statistics_;
};

#endif // RANDOM_H
