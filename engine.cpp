#include "engine.h"
#include <time.h>
//#include <qthread.h>
#include <iostream> //debug
#include <QtConcurrentRun>
#include "sleeper.h"

const float Engine::score_vittoria = 10.0f;
const float Engine::score_pareggio = 5.0f;
const float Engine::score_mossa_valida = 0.01f;
const QVector<int> Engine::topologia = {15,10,10,6};

/**
 * @brief costruttore
 */
Engine::Engine() :
    generation(0),
    run_flag(false),
    delay_gen(1),
    delay_partita(50),
    numPlayers(10),
    i(0),j(0),n(0),
    partita(new Game(this)),
    players(numPlayers),
    best(nullptr)
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
    p->addScore(score_mossa_valida);
}

void Engine::vincitore(Player* p)
{
    p->addScore(score_vittoria);
}

void Engine::pareggio(Player* p1, Player* p2)
{
    p1->addScore(score_pareggio);
    p2->addScore(score_pareggio);
}
