#include "engine.h"
#include <time.h>
//#include <qthread.h>
#include <iostream> //debug
#include <QtConcurrentRun>
#include <QtAlgorithms>
#include "sleeper.h"
#include <math.h>

const float Engine::score_vittoria = 10.0f;
const float Engine::score_pareggio = 5.0f;
const float Engine::score_mossa_valida = 1.0f;
const QVector<int> Engine::topologia = {18,10,10,6};

/**
 * @brief costruttore
 */
Engine::Engine() :
    generation(0),
    run_flag(false),
    delay_gen(1),
    delay_partita(50),
    numPlayers(10),
    p_crossover(0.7f),
    p_selezione(0.2f),
    i(0),j(0),n(0),
    partita(new Game(this)),
    players(numPlayers),
    best(new Brain(topologia))//best(nullptr)
{
    srand(time(NULL));
    //carico il vettore dei giocatori
    for(int i = 0; i < players.size(); i++)
        players[i] = new AI(new Brain(topologia));

    //connessioni
    connect(this, SIGNAL(stopGame()), partita, SLOT(stop()));

}

/**
 * @brief ciclo principale dell'algoritmo genetico
 */
void Engine::run()
{
    run_flag = true;

    while (run_flag)    //ciclo principale
    {
        for(; i < players.size() && run_flag; i++)
        {
            //std::cout << "FI = " << i << std::endl; //debug
            for(; j < players.size() && run_flag; j++)
            {
                //std::cout << "FJ = " << j << std::endl; //debug
                if(i != j)
                {
                    std::cout << std::endl <<"GEN "<< generation; //debug
                    std::cout << " PARTITA NUMERO " << n << std::endl;
                    partita->run(players[i], players[j]);
                    n++;
                    Sleeper::msleep(delay_partita);
                }
            }
            j = 0;
        }
        if(run_flag)
        {
            i = 0, j = 0, n = 0;             //resetto lo stato

            //ordino in base al punteggio
            qSort(players.begin(), players.end(), Engine::compare);
            //debug stampo i punteggi
            for(int i = 0; i < numPlayers; i++)
                std::cout<<"("<<i<<","<<players[i]->getScore()<<") ";
            std::cout<<std::endl;
            //debug

            //salvo la rete migliore
            Player *p = players[0];
            if(/*best == nullptr || */ best.getScore() < p->getScore())
                best = *(static_cast<AI*>(p));

            selezioneTorneo(p_selezione);


            generation++;                    //aumento il numero di generazione
            emit GenChanged(generation);     //emetto il segnale
        }

        //da togliere (mettere delay solo sulle partite)
        Sleeper::msleep(delay_gen);           //aspetto mezzo secondo
    }
}

void Engine::stop()
{
    run_flag = false;
}

void Engine::runEvent()
{
    start();
}

void Engine::stopEvent()
{
    stop();
}

void Engine::setDelay(int d)
{
    delay_partita = d;
}

void Engine::mossaErrata()
{
    emit stopGame();
}

void Engine::mossaValida(Player* p)
{
    std::cout<<"mossa eseguita quindi premio"<<std::endl; //debug
    p->addScore(score_mossa_valida);
}

void Engine::vincitore(Player* p)
{
    std::cout<<"il giocatore ha vinto 10pt"<<std::endl; //debug
    p->addScore(score_vittoria);
}

void Engine::pareggio(Player* p1, Player* p2)
{
    std::cout<<"finita in pareggio 5pt"<<std::endl; //debug
    p1->addScore(score_pareggio);
    p2->addScore(score_pareggio);
}

// funzione per algoritmo di sort
bool Engine::compare(const Player *a, const Player *b)
{
    // nego per ottenere l'ordine decrescente
    return !((*a) < *(b));
}

/**
 * @brief Engine::fitnessAVG
 * @return il fitness medio della popolazione corrente
 */
float Engine::fitnessAVG() const
{
    float sum = 0;
    for(int i; i < players.size(); i++)
        sum += players[i]->getScore();
    return sum/players.size();
}

void Engine::selezioneTorneo(float p)
{
    QVector<Player*> sel;
    int prob;

    //carico il vettore i selezione
    for(int i = 0; i < players.size(); i++)
    {
        prob = static_cast<int> (round((p*pow((1-p),i)) * numPlayers * 100));
        //inserisco tanti elementi quanti il numero di probabilita in sel
        for(int j = 0; j < prob; j++)
            sel.append(players[i]);
    }

    std::cout<<"[elementi in sel] "<<sel.size()<<std::endl; //debug

    //creo la nuova popolazione
//    for(int i = 0; i < players.size()-1; i++)
//    {
//        Player *a = sel[rand() % sel.size()];    //seleziono un individuo  caso
//        Player *b;

//        //un giocatore non puo accoppiarsi con se stesso
//        do {
//            b = sel[rand() % sel.size()];
//        }while (b == a);

//        //crossover a punto singolo
//        //(scambio i giocatori per ottenere la combinazione dei 2)
//        players[i] = crossover(a, b, p_crossover);
//        players[i+1] = crossover(b, a, p_crossover);
//    }

    for(int i = 0; i < players.size(); i++)
        {
            Player *a = sel[rand() % sel.size()];//seleziono un individuo a caso
            Player *b;

            //un giocatore non puo accoppiarsi con se stesso
            do {
                b = sel[rand() % sel.size()];
            }while (b == a);

            players[i] = crossover(a, b, p_crossover);
        }

}

/**
 * @brief Engine::crossover
 * @param a
 * @param b
 * @param p
 * @return figlio con meta dei geni del padre e meta dei geni della madre
 */
Player* Engine::crossover(Player *a, Player *b, float p) const
{
    std::cout<<std::endl<<"[FACCIO IL CROSSOVER] "<<std::endl; //debug
    AI *ai = static_cast<AI*>(a);
    AI *bi = static_cast<AI*>(b);
    if(Brain::randTo(1.0f) <= p)
    {
        Player *c = static_cast<Player*> ((*ai) + (*bi));
        std::cout<<"score figlio "<<c->getScore()<<std::endl; //debug
        return c;
    }
    else
    {
        a->resetScore();
        return a;
    }
}
