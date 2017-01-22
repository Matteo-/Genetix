#include "ai.h"
#include <iostream>
#include <QVector>

AI::AI(Brain *b): score(0), brain(b)
{
    std::cout << std::endl; //debug
    brain->info();  //debug
    //brain->print(); //debug
    std::cout << std::endl; //debug
}

/**
 * @brief AI::calcolaMossa
 * @param tavolo
 * @param turno
 * @return ritorna la mossa con la valutazione migliore
 */
int AI::calcolaMossa(const Table &table, int turno) const
{
    QVector<int> mosse;
    float stima_max = -10000.0f;//, stima; //valore piccolo fuori scala
    int indice = 0;
    QVector<float> result;
    //parametri di valutazione per la rete neurale
    float turno_succ,diff_punti,zero_my,zero_vs;

    mosse = table.mosseValide(turno);
    //for(int i = 0; i < mosse.size(); i++)
    //{
        //creo un tavolo virtuale
        Table t(table);
        //std::cout << "TAVOLO INIZIALE" << std::endl;    //debug
        //t.stampa(); //debug
        //eseguo la mossa
        //turno_succ = static_cast <float> (t.eseguiMossa(turno, mosse[i]));
        diff_punti = static_cast <float> (t.differenzaPunti(turno));
        zero_my = static_cast <float> (t.bucheVuote(turno));
        zero_vs = static_cast <float> (t.bucheVuote(Table::avversario(turno)));

//        stima = brain->getOutput({diff_punti,turno_succ,zero_my,zero_vs})[0];

        //prova a passargli il tavolo
        QVector<float> in;
        QVector<int> tmp = t.get()[0]+t.get()[1];
        for(int i = 0; i < tmp.size(); i++)
            in.append(static_cast <float> (tmp[i]));
        in.append(static_cast <float> (turno));
        in.append(diff_punti);
        in.append(zero_my);
        in.append(zero_vs);
        result = brain->getOutput(in);



        //std::cout<<"MOSSA "<<turno<<" ESEGUITA"<<std::endl;//debug
        //t.stampa(); //debug
        //std::cout<<"STIMA: ";
        for(int i = 0; i < result.size(); i++)
        {
            if (result[i] > stima_max)
            {
                stima_max = result[i];
                indice = i;
            }
            //std::cout << result[i]<< " ";
        }
        //std::cout<<std::endl;   //debug

        //salvo l'indice della mossa valutata meglio
//        if (stima > stima_max)
//        {
//            stima_max = stima;
//            indice = i;
//        }
    //}

    return indice;
    //return mosse.at(indice);
}

float AI::getScore() const
{
    return score;
}

void AI::addScore(int s)
{
    score += s;
}

void AI::resetScore()
{
    score = 0;
}

AI* operator+(const AI &a, const AI &b)
{
    AI *figlio = new AI(*(a.brain) + *(b.brain));
    return figlio;
}

bool AI::operator<(const Player &a) const
{
    return score < a.getScore();
}

bool operator<(const AI &a, const AI &b)
{
    if (a.score < b.score) return true;
    else return false;
}

bool operator>(const AI &a, const AI &b)
{
    if (a.score > b.score) return true;
    else return false;
}

bool operator>=(const AI &a, const AI &b)
{
    if (a.score >= b.score) return true;
    else return false;
}

bool operator<=(const AI &a, const AI &b)
{
    if (a.score <= b.score) return true;
    else return false;
}
