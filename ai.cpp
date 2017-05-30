#include "ai.h"
#include <iostream>
#include <QVector>
#include <cstdio> //debug
#include <algorithm> //min e max
using namespace std;

struct eval {
    int mossa;
    float v;

    bool operator<(const eval& r) const
    {
        return v < r.v;
    }
};

int AI::max_iter_backprop = 500;
int AI::istanze = 0;
int AI::max_depth = 7;

AI::AI(Brain *b, int id): brain(b)
{
    if(id) { ID = id; }
    else { ID = istanze; }

    istanze++;

    cout<<"creating AI ["<<istanze<<"]....OK\n"; //debug
    //debug
//    cout << std::endl; //debug
//    brain->info();  //debug
//    brain->print(); //debug
//    cout << std::endl; //debug

    QVector<int> t = brain->getTopology();
    QVector<float> w = brain->getWeights();
    statistics_ = stat(&t, &w, ID);
}

AI::~AI()
{
    delete brain;
    //debug
    cout<<"deleting AI ["<<istanze<<"]....";   //debug
    istanze--;
    cout<<"OK\n";
    //debug
}

/**
 * @brief AI::calcolaMossa
 * @param tavolo
 * @param turno
 * @return ritorna la mossa con la valutazione migliore
 */
int AI::calcolaMossa(const Table &table, int turno) const
{
    //creo il vettore delle mosse valide per il mio turno
    QVector<int> mosse = table.mosseValide(turno);
    //creo un vettore contenente la valutazione fatta per ogni mossa
    QVector<float> value(mosse.size(), 0.0f);

    for(int i = 0; i < value.size(); i++)
    {
        value[i] = alphabeta(table, mosse[i],
                             turno, turno, -2000.0f, 2000.0f, max_depth);
    }

    int i_max = maxValueOf(value);

//    brain->print(); //debug
//    brain->info();  //debug

    return mosse[i_max];
}

float AI::alphabeta(Table table,int mossa, int turno, int player, float a, float b, int depth) const {
    //il tavolo è passato per valore quindi posso simulare le mosse
    int state = table.eseguiMossa(turno,mossa);

    if(depth == 0 || table.fineGioco()) {
        /*sono arrivato alla fine e quindi eseguo la valutazione del tavolo
         * la rete prende in input i seguenti valori:
         * - differenza punti
         * - numero delle proprie buche vuote
         * - numero buche vuote avversario
         * - turno bonus?
         * - vittoria o no
         */
        QVector<float> in;
        in.append(table.differenzaPunti(turno));
        in.append(table.bucheVuote(turno));
        in.append(table.bucheVuote(Table::rival(turno)));
        in.append(state);

        if(table.fineGioco())
        {
            int winner = table.calcolaVincitore();
            if(winner == player) { in.append(1); }
            if(winner == Table::rival(player)) { in.append(-1); }
            if(winner == 2) { in.append(0); }
        }
        else
        {
            in.append(0);
        }
        return brain->getOutput(in).first();
    }
    else
    {
        //creo il vettore delle mosse valide per il mio turno
        int next_turn = (state) ? turno : Table::rival(turno);

        /*
         * nell' alpha beta pruning è importante osservare per prime
         * le mosse valutate migliori a una prima stima
         * in modo da effettuare tagli il prima possibile per velocizzare la ricerca
         */
        QVector<int> mosse = table.mosseValide(next_turn);
        QVector<eval> s(mosse.size());
        for(int i = 0; i < mosse.size(); i++)
        {
            s[i].mossa = mosse[i];
            s[i].v = alphabeta(table,mosse[i],turno,player,a,b,0);
        }
        qSort(s);
        reverse(s.begin(), s.end());

        //massimizzo se il turno è mio
        if(turno == player)
        {
            float v = -2000.0f; // -infinito
            for(int i = 0; i < s.size(); i++)
            {
                v = max(v, alphabeta(table, s[i].mossa, next_turn, player,
                                            a, b, depth-1));
                a = max(a, v);
                if(b <= a) { break; }
            }
            return v;
        }
        else
        {
            float v = 2000.0f; // + infinito
            for(int i = 0; i < mosse.size(); i++)
            {
                v = min(v, alphabeta(table, s[i].mossa, next_turn, player,
                                            a, b, depth-1));
                b = min(b, v);
                if(b <= a) { break; }
            }
            return v;
        }
    }

}

float AI::getScore() const
{
    return statistics_.score;
}

void AI::addScore(float s)
{
    statistics_.score += s;
}

void AI::resetScore()
{
    QVector<int> t = brain->getTopology();
    QVector<float> w = brain->getWeights();
    statistics_ = stat(&t,&w,ID);
}

void AI::win(float s)
{
    addScore(s);
    statistics_.win++;
}

void AI::lose(float s)
{
    addScore(s);
    statistics_.lose++;
}

void AI::parity(float s)
{
    addScore(s);
    statistics_.parity++;
}

stat AI::statistics() const
{
    return statistics_;
}

AI* operator+(const AI &a, const AI &b)
{
    //debug
    //cout<<"faccio la + tra in AI\n";
    //debug
    AI *figlio = new AI((*(a.brain) + *(b.brain)));
    return figlio;
}

bool AI::operator<(const Player &a) const
{
    return statistics_.score < a.getScore();
}

bool operator<(const AI &a, const AI &b)
{
    if (a.getScore() < b.getScore()) return true;
    else return false;
}

bool operator>(const AI &a, const AI &b)
{
    if (a.getScore() > b.getScore()) return true;
    else return false;
}

bool operator>=(const AI &a, const AI &b)
{
    if (a.getScore() >= b.getScore()) return true;
    else return false;
}

bool operator<=(const AI &a, const AI &b)
{
    if (a.getScore() <= b.getScore()) return true;
    else return false;
}

/*
 * ritorna l'indice dell'elemento maggiore
 * -1 se vettore vuoto
 */
int AI::maxValueOf(const QVector<float> v)
{
    if(v.size() > 0)
    {
        int indice = 0;
        float stima_max = v[indice];
        for(int i = 1; i < v.size(); i++)
        {
            if (v[i] > stima_max)
            {
                stima_max = v[i];
                indice = i;
            }
        }
        return indice;
    }
    else { return -1; }
}

int AI::getID() const
{
    return ID;
}
