#ifndef AI_H
#define AI_H

#include "player.h"
#include "table.h"
#include "brain.h"

class AI : public Player
{
public:
    AI(Brain *b);
    ~AI();
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
    static int maxValueOf(const QVector<float> v);

private:
    int score;
    Brain *brain;
    static int max_iter_backprop;
    static int istanze;                    //debug memory leak
};

#endif // AI_H
