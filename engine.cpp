#include "engine.h"
#include <time.h>
//#include <qthread.h>
#include <iostream> //debug
#include <QtConcurrentRun>
#include <QtAlgorithms>
#include "sleeper.h"
#include <math.h>

int Engine::istanze = 0;
const float Engine::score_vittoria = 10.0f;
const float Engine::score_pareggio = 5.0f;
const float Engine::score_mossa_valida = 0.1f;
const QVector<int> Engine::topologia = {18,400,6};

/**
 * @brief costruttore
 */
Engine::Engine() :
    generation(0),
    run_flag(false),
    delay_gen(1),
    delay_partita(50),
    numPlayers(30),
    p_crossover(0.8f),
    p_selezione(0.5f),
    i(0),j(0),n(0),
    partita(new Game(this)),
    players(numPlayers),
    best(nullptr)
{
    //debug
    istanze++;
    std::cout<<"creating Engine ["<<istanze<<"]....OK\n";
    //debug

    srand(time(NULL));
    //carico il vettore dei giocatori
    for(int i = 0; i < players.size(); i++)
    {
        Player *an = new AI(new Brain(topologia));
        players[i] = PlayerPtr(an);
        std::cout<<"[CARICATO]"<<i<<"\n"; //debug
    }

    //debug
    for(int i = 0; i < players.size(); i++) {
        PlayerPtr prova = players[i];
        std::cout<<"player "<<i<<" check\n";
    }
    //debug

    //connessioni
    connect(this, SIGNAL(stopGame()), partita, SLOT(stop()));

}

Engine::~Engine()
{
    delete partita;
    //debug
    std::cout<<"deleting Engine ["<<istanze<<"]....";
    istanze--;
    std::cout<<"OK\n";
    //debug
}

/**
 * @brief ciclo principale dell'algoritmo genetico
 */
void Engine::run()
{
    run_flag = true;

    while (run_flag)    //ciclo principale
    {
        Sleeper::msleep(delay_gen);

        for(; i < players.size() && run_flag; i++)
        {
            //std::cout << "FI = " << i << std::endl; //debug
            for(; j < players.size() && run_flag; j++)
            {
                //std::cout << "FJ = " << j << std::endl; //debug
                if(i != j)
                {
                    std::cout << std::endl <<"GEN "<< generation //debug
                        << " PARTITA NUMERO " << n << " ga "<<
                                 i<<" gb "<<j<<std::endl; //debug
                    partita->run(players[i], players[j]);
                    n++;
                    Sleeper::msleep(delay_partita);
                }
            }
            j = 0;
        }

        //ciclo di prova con il tester
//        for(; i < players.size() && run_flag; i++)
//        {
//            std::cout << std::endl <<"GEN "<< generation; //debug
//            std::cout << " PARTITA NUMERO " << n << std::endl;
//            partita->run(players[i], tester);
//            n++;
//            Sleeper::msleep(delay_partita);
//        }

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
            PlayerPtr p = players[0];
            if(best == nullptr || best->getScore() < p->getScore())
                best = p;

            selezioneTorneo(p_selezione);


            generation++;                    //aumento il numero di generazione
            emit GenChanged(generation);     //emetto il segnale
        }

        //da togliere (mettere delay solo sulle partite)
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

void Engine::mossaValida(PlayerPtr p)
{
    //std::cout<<"mossa eseguita quindi premio"<<std::endl; //debug
    p->addScore(score_mossa_valida);
}

void Engine::vincitore(PlayerPtr p)
{
    //std::cout<<"il giocatore ha vinto 10pt"<<std::endl; //debug
    p->addScore(score_vittoria);
}

void Engine::pareggio(PlayerPtr p1, PlayerPtr p2)
{
    //std::cout<<"finita in pareggio 5pt"<<std::endl; //debug
    p1->addScore(score_pareggio);
    p2->addScore(score_pareggio);
}

// funzione per algoritmo di sort
bool Engine::compare(const PlayerPtr a, const PlayerPtr b)
{
    // nego per ottenere l'ordine decrescente
    return !( *(a.get()) < *(b.get()) );
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
    QVector<PlayerPtr> sel(0);
    int prob;

    //debug
//    for(int i = 0; i < players.size(); i++) {
//        PlayerPtr prova = players[i];
//        std::cout<<"player "<<i<<" check\n";
//    }
    //debug

    //carico il vettore i selezione
    for(int i = 0; i < players.size(); i++)
    {
        /* resetto gli score per ripartire da 0 con la generazione successiva */
        players[i]->resetScore();
        prob = static_cast<int> (round((p*pow((1-p),i)) * numPlayers * 100))+1;

        //debug
//        using namespace std;
//        cout<<"Prob " << prob << endl;
        //debug

        //inserisco tanti elementi quanti il numero di probabilita in sel
        int offset = sel.size();
        sel.resize(sel.size()+prob);
        while (prob > 0) {
            sel[offset++] = players[i];
            prob--;
        }
    }

    //std::cout<<"[elementi in sel] "<<sel.size()<<std::endl; //debug

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

    for(int i = (players.size()/2)-1; i < players.size(); i++)
    {
        int idx = rand() % sel.size();
        PlayerPtr a = sel[idx];//seleziono un individuo a caso
        //debug
//        using namespace std;
//        cout << "Indice A "<<idx;
        //debug
        PlayerPtr b;

        //un giocatore non può accoppiarsi con se stesso
        do {
            idx = rand() % sel.size();
            b = sel[idx];
        }while (b == a);
        //debug
//        cout << " Indice B "<<idx<<endl;
//        cout<<"rimuovo l'AI "<<i<<"\t\t"<<__FILE__<<":"<<__LINE__<<"\n";
        //debug
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
PlayerPtr Engine::crossover(PlayerPtr a, PlayerPtr b, float p) const
{
    std::cout<<std::endl<<"[FACCIO IL CROSSOVER] "<<std::endl; //debug
    //AI *ai = dynamic_cast<AI*>(a.get());
    //AI *bi = dynamic_cast<AI*>(b.get());
    if(Brain::randTo(0,1.0f) <= p)
    {
//        std::cout<<"faccio la + in engine\n";
        PlayerPtr c = static_cast<PlayerPtr> (*(AI*)&(*a) + *(AI*)&(*b));
//        std::cout<<"score figlio "<<c->getScore()<<std::endl; //debug
        return c;
    }
    else
    {
        a->resetScore();
        return a;
    }
}
