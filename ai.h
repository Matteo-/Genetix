#ifndef AI_H
#define AI_H

#include "player.h"
#include "table.h"
#include "brain.h"

class AI : public Player
{
public:
    AI(Brain *b, int id = 0);
    ~AI();
    int calcolaMossa(const Table &table, int turno) const;
    void addScore(float s);
    void win(float s);
    void lose(float s);
    void parity(float s);
    stat statistics() const;
    float getScore() const;
    int getID() const;
    void resetScore();
    bool operator<(const Player &) const;
    friend AI* operator+(const AI&, const AI&);
    friend bool operator<(const AI &a, const AI &b);
    friend bool operator<=(const AI &a, const AI &b);
    friend bool operator>(const AI &a, const AI &b);
    friend bool operator>=(const AI &a, const AI &b);
    static int maxValueOf(const QVector<float> v);

private:
    float alphabeta(Table table, int mossa, int turno, int player, float a, float b, int depth) const;
    static int max_depth;
    stat statistics_;
    Brain *brain;
    static int max_iter_backprop;
    int ID;
    static int istanze;                    //debug memory leak
};

#endif // AI_H
