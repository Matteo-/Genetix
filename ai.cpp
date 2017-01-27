#include "ai.h"
#include <iostream>
#include <QVector>
#include <cstdio> //debug

int AI::max_iter_backprop = 500;
int AI::istanze = 0;

AI::AI(Brain *b): score(0), brain(b)
{
    //debug
    istanze++;
    std::cout<<"creating AI ["<<istanze<<"]....OK\n";
    //debug
//    std::cout << std::endl; //debug
//    brain->info();  //debug
//    brain->print(); //debug
//    std::cout << std::endl; //debug
}

AI::~AI()
{
    delete brain;
    //debug
    std::cout<<"deleting AI ["<<istanze<<"]....";   //debug
    istanze--;
    std::cout<<"OK\n";
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
    QVector<int> mosse;
    //float stima_max;// = -10000.0f;//, stima; //valore piccolo fuori scala
    //int indice = 0;
    QVector<float> result;
    //parametri di valutazione per la rete neurale
    float diff_punti,zero_my,zero_vs;
    int indice;

    mosse = table.mosseValide(turno);
    //for(int i = 0; i < mosse.size(); i++)
    //{
        //creo un tavolo virtuale
        //Table t(table);
        //std::cout << "TAVOLO INIZIALE" << std::endl;    //debug
        //t.stampa(); //debug
        //eseguo la mossa
        diff_punti = static_cast <float> (table.differenzaPunti(turno));
        zero_my = static_cast <float> (table.bucheVuote(turno));
        zero_vs = static_cast <float> (table.bucheVuote(Table::rival(turno)));

        //prova a passargli il tavolo
        QVector<float> in;
        QVector<int> tmp = table.get()[0]+table.get()[1];
        for(int i = 0; i < tmp.size(); i++)
            in.append(static_cast <float> (tmp[i]/10.f));
        in.append(static_cast <float> (turno));
        in.append(diff_punti/10);
        in.append(zero_my/10);
        in.append(-zero_vs/10);
        result = brain->getOutput(in);

        //std::cout<<"MOSSA "<<turno<<" ESEGUITA"<<std::endl;//debug
        //t.stampa(); //debug
        //std::cout<<"STIMA: ";
//        indice = maxValueOf(result);
        //std::cout<<std::endl;   //debug

        //implementazione con backprop
        mosse = table.mosseValide(turno);
        //se la mossa non è valida insegna a non farla
        if( mosse.indexOf(indice) == -1)
        {
            int iter = 0;
            QVector<float> expect(result.size());
            do
            {
                iter++;
                //creo l'output desiderato
                for(int i = 0; i < expect.size(); i++)
                {
                    //se è una mossa valida copio il valore di output
                    //per avere un errore di 0
                    //altrimenti meto il valore 0 per avere un errore alto
                    if(mosse.indexOf(i) == -1)
                    {
                        expect[i] = 0;
                    }
                    else
                    {
                        expect[i] = result[i];
                    }
                }
                //aggiorno la rete
                brain->backprop(result, expect);
                //stabilisco la nuova mossa
                result = brain->getOutput(in);
                indice = maxValueOf(result);

            }while( mosse.indexOf(indice) == -1 && iter <= max_iter_backprop);

            return indice;
        }
        else
        {
            //debug
        //    brain->print();
        //    std::cout<<"INDICE "<<indice<<std::endl;
        //    getchar();
            //debug
            return indice;
        }
//        return indice;
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
    //debug
    //std::cout<<"faccio la + tra in AI\n";
    //debug
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

int AI::maxValueOf(const QVector<float> v)
{
    if(v.size() > 0)
    {
        int indice = v[0];
        float stima_max = v[indice];
        for(int i = 0; i < v.size(); i++)
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
