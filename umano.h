#ifndef UMANO_H
#define UMANO_H

#include "player.h"
#include "table.h"

class Umano : public Player
{
public:
    Umano();
    ~Umano();
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(float s);
    float getScore() const;
    void resetScore();
    bool operator<(const Player &) const;

    void win(float s) { addScore(s); statistics_.win++; }
    void lose(float s) { addScore(s); statistics_.lose++; }
    void parity(float s) { addScore(s); statistics_.parity++; }
    stat statistics() { return statistics_; }
private:
    stat statistics_;
    static int istanze;                    //debug memory leak
};

#endif // UMANO_H
