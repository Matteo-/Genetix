#ifndef TESTER_H
#define TESTER_H

#include "player.h"
#include "ai.h"

class Tester : public Player
{
public:
    Tester();
    ~Tester();
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(int s);
    bool operator<(const Player &) const;
    float getScore() const;
    void resetScore();
private:
    float score;
    static int istanze;                    //debug memory leak
};

#endif // TESTER_H
