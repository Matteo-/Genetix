#include "tester.h"

Tester::Tester(): score(0)
{

}

int Tester::calcolaMossa(const Table &table, int turno) const
{
    QVector<int> mosse = table.mosseValide(turno);
    //parametri di valutazione per la rete neurale
    float turno_succ,diff_punti,zero_my,zero_vs, stima, stima_max = -1000.0f;
    int indice = 0;

    for(int i = 0; i < mosse.size(); i++)
    {
        Table t(table); //tavolo virtuale
        turno_succ = static_cast <float> (t.eseguiMossa(turno, mosse[i]));
        diff_punti = static_cast <float> (t.differenzaPunti(turno));
        zero_my = static_cast <float> (t.bucheVuote(turno));
        zero_vs = static_cast <float> (t.bucheVuote(Table::rival(turno)));

        stima = turno_succ*1+diff_punti*2+zero_my*0.5f-zero_vs*0.5f;

        if(stima > stima_max)
        {
            stima_max = stima;
            indice = i;
        }
    }

    return mosse[indice];
}


void Tester::addScore(int s)
{
    score += s;
}

bool Tester::operator<(const Player &p) const
{
    return score < p.getScore();
}

float Tester::getScore() const
{
    return score;
}

void Tester::resetScore()
{
    score = 0;
}
