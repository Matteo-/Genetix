#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <memory>
#include "table.h"
#include <iostream>

struct stat {
    float score;
    int win;
    int lose;
    int parity;
    QVector<float> strategy;

    stat(QVector<float> *strat = 0, float s = 0, int w = 0, int l = 0, int p = 0)
    {
        score = s;
        win = w;
        lose = l;
        parity = p;

        if(strat) { strategy = (*strat); }
    }

    friend std::ostream& operator<< (std::ostream& out, const stat& s);
};

inline std::ostream& operator<< (std::ostream& out, const stat& s)
{
    out << "************ Player *************\n";
    out << "* Score... " << s.score << "\n";
    out << "* Vittorie:... " << s.win << "\n";
    out << "* Sconfitte:..." << s.lose << "\n";
    out << "* Pareggiate:... " << s.parity << "\n";
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
    virtual void resetScore() = 0;
    virtual ~Player() {}

    virtual void win(float s) = 0;
    virtual void lose(float s) = 0;
    virtual void parity(float s) = 0;
    virtual stat statistics() = 0;
protected:
};

typedef std::shared_ptr<Player> PlayerPtr;

#endif // GIOCATORE_H
