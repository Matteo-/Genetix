#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <memory>
#include "table.h"
#include <iostream>

struct stat {
    int ID;
    float score;
    int win;
    int lose;
    int parity;
    QVector<int> topology;
    QVector<float> strategy;

    stat(QVector<int> *top = 0,
         QVector<float> *strat = 0,
         int id = 0,
         float s = 0, int w = 0, int l = 0, int p = 0)
    {
        ID = id;
        score = s;
        win = w;
        lose = l;
        parity = p;

        if(top) { topology = (*top); }
        if(strat) { strategy = (*strat); }
    }

    friend std::ostream& operator<< (std::ostream& out, const stat& s);
};

inline std::ostream& operator<< (std::ostream& out, const stat& s)
{
    out << "************ Player *************\n";
    out << "* ID... " << s.ID << "\n";
    out << "* Score... " << s.score << "\n";
    out << "* Vittorie:... " << s.win << "\n";
    out << "* Sconfitte:..." << s.lose << "\n";
    out << "* Pareggiate:... " << s.parity << "\n";
    out << "* Topologia:... ";
    for(int i = 0; i < s.topology.size(); i++) { out << s.topology[i] << " "; }
    out << "\n";
    out << "* strategia:... ";
    for(int i = 0; i < s.strategy.size(); i++) { out << s.strategy[i] << " "; }
    out << "\n";
    out << "*********************************\n";
    return out;
}

/**
 * @brief classe astratta che desscrive l'interfaccia pubblica di un giocatore
 */
class Player
{
public:
    //da determinare i parametri (mie pedine, pedine avversarie)
    virtual int calcolaMossa(const Table &table, int turno) const = 0;
    virtual void addScore(float s) = 0;
    virtual bool operator<(const Player &) const = 0;
    virtual float getScore() const = 0;
    virtual int getID() const = 0;
    virtual void resetScore() = 0;
    virtual ~Player() {}

    virtual void win(float s) = 0;
    virtual void lose(float s) = 0;
    virtual void parity(float s) = 0;
    virtual stat statistics() const = 0;
protected:
};

typedef std::shared_ptr<Player> PlayerPtr;

#endif // GIOCATORE_H
