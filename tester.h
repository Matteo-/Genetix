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
    void addScore(float s);
    bool operator<(const Player &) const;
    float getScore() const;
    void resetScore();

    void win(float s) { addScore(s); statistics_.win++; }
    void lose(float s) { addScore(s); statistics_.lose++; }
    void parity(float s) { addScore(s); statistics_.parity++; }
    stat statistics() { return statistics_; }
private:
    stat statistics_;
    static int istanze;                    //debug memory leak
};

#endif // TESTER_H
