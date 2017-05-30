#include "engine.h"
#include <time.h>
//#include <qthread.h>
#include <iostream> //debug
#include <QtConcurrentRun>
#include <QtAlgorithms>
#include "sleeper.h"
#include <math.h>
using namespace std;

int Engine::istanze = 0;
const float Engine::score_vittoria = 10.0f;
const float Engine::score_pareggio = 5.0f;
const float Engine::score_sconfitta = 0.0f;
const float Engine::score_mossa_valida = 0.0f;
const QVector<int> Engine::topologia = {5,1};

/**
 * @brief costruttore
 */
Engine::Engine() :
    generation(0),
    run_flag(false),
    delay_gen(1),
    delay_partita(50),
    numPlayers(3),
    p_crossover(0.4f),
    p_selezione(0.6f),
    i(0),j(0),n(0),
    partita(new Game(this)),
    players(numPlayers),
    best(nullptr),
    tree(NULL),
    server()
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

    connect(&server, SIGNAL(sendResult(QByteArray&)),
            this, SLOT(fitness(QByteArray&)));

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
    QVector<PlayerPtr> result;

    while (run_flag)    //ciclo principale
    {
        Sleeper::msleep(delay_gen);

        for(; i < players.size() && run_flag; i++)
        {
            //std::cout << "FI = " << i << std::endl; //debug
            for(; j < players.size() && run_flag; j++)
            {
                if(i != j)
                {
                    std::cout << std::endl <<"GEN "<< generation //debug
                        << " PARTITA NUMERO " << n
                        << " ga "<< players[i]->getID()
                        << " gb "<< players[j]->getID() <<std::endl; //debug

                    server.distribute(serialize({players[i], players[j]}));

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


            //debug stampo i punteggi
            for(int i = 0; i < numPlayers; i++)
                std::cout<<"("<<i<<","<<players[i]->getScore()<<") ";
            std::cout<<std::endl;
            //debug

            //salvo la rete migliore
            PlayerPtr p = players[0];
            if(best == nullptr || best->getScore() < p->getScore())
            {
                best = p;
                std::cout << best->statistics();
            }

            server.wait();

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

//void Engine::mossaErrata()
//{
//    emit stopGame();
//}

//void Engine::mossaValida(PlayerPtr p)
//{
//    //std::cout<<"mossa eseguita quindi premio"<<std::endl; //debug
//    p->addScore(score_mossa_valida);
//}

//void Engine::vincitore(PlayerPtr p)
//{
//    //std::cout<<"il giocatore ha vinto 10pt"<<std::endl; //debug
//    p->addScore(score_vittoria);
//}

//void Engine::pareggio(PlayerPtr p1, PlayerPtr p2)
//{
//    //std::cout<<"finita in pareggio 5pt"<<std::endl; //debug
//    p1->addScore(score_pareggio);
//    p2->addScore(score_pareggio);
//}

// funzione per algoritmo di sort
//TODO rifare con overloading operator<
bool Engine::compare(const PlayerPtr a, const PlayerPtr b)
{
    // nego per ottenere l'ordine decrescente
    return !( *(a.get()) < *(b.get()) );
}

void Engine::fitness(QByteArray &elab)
{
    QDataStream in(elab);
    in.setVersion(QDataStream::Qt_4_0);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    quint16 dim;
    in >> dim;
    QVector<int> res(3);

    in >> res[0] >> res[1] >> res[2];

    PlayerPtr p0, p1;
    int find = 0;
    for(int i = 0; i < players.size(); i++)
    {
        if(players[i]->getID() == res[1]) { p0 = players[i]; find++; }

        if(players[i]->getID() == res[2]) { p1 = players[i]; find++; }

        if(find == 2) break;
    }

    switch (res[0]) {
    case 0:
        p0->win(score_vittoria);
        p1->lose(score_sconfitta);
        cout << "HA VINTO IL GIOCATORE 0" << std::endl; //debug
        break;
    case 1:
        p0->lose(score_sconfitta);
        p1->win(score_vittoria);
        cout << "HA VINTO IL GIOCATORE 1" << std::endl; //debug
        break;
    case 2:
        p0->parity(score_pareggio);
        p0->parity(score_pareggio);
        cout << "PARITA'" << std::endl; //debug
        break;
    default:
        break;
    }
}

QByteArray Engine::serialize(QVector<PlayerPtr> p)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setVersion(QDataStream::Qt_4_0);

    for (int i = 0; i < p.size(); i++)
    {
        stat s = p[i]->statistics();

//        std::cout << s; //debug

        /* costruisco il pacchetto
         * | n° elem. topologia | topologia | n° elem. weights | weights | ID |
         */
        out << s.topology.size();
        for(int i = 0; i < s.topology.size(); i++) {
            out << s.topology[i];
        }

        out << s.strategy.size();
        for(int i = 0; i < s.strategy.size(); i++) {
           out << s.strategy[i];
        }
        out << s.ID;
    }

//    //debug
//    QDataStream in(data);
//    in.setVersion(QDataStream::Qt_4_0);
//    in.setByteOrder(QDataStream::LittleEndian);
//    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

//    int id;
//    int dim;
//    while(!in.atEnd()) {
//        in >> dim;
//        qDebug() << dim;
//        QVector<int> t(dim);
//        for(int i = 0; i < dim; i++)
//        {
//            in >> t[i];
//            qDebug() << t[i];
//        }

//        in >> dim;
//        qDebug() << dim;
//        QVector<float> w(dim);
//        for(int i = 0; i < dim; i++)
//        {
//            in >> w[i];
//            qDebug() << w[i];
//        }

//        in >> id;
//        qDebug() << id;
//    }

//    QVector<PlayerPtr> pr = deserialize(data);
//    stat s = pr[0]->statistics();
//    std::cout << s;

//    getchar();
//    //debug

    return data;

}

QVector<PlayerPtr> Engine::deserialize(QByteArray data)
{
    //qDebug() << "size: " << data.size();

    QVector<PlayerPtr> p;

    QDataStream in(data);
    in.setVersion(QDataStream::Qt_4_0);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    int id;
    int dim;
    while(!in.atEnd()) {
        in >> dim;
        QVector<int> t(dim);
        for(int i = 0; i < dim; i++)
        {
            in >> t[i];
        }

        in >> dim;
        QVector<float> w(dim);
        for(int i = 0; i < dim; i++)
        {
            in >> w[i];
        }

        in >> id;

//        qDebug() << "vettore:";
//        for(int i = 0; i < t.size(); i++) qDebug() << t[i];
//        qDebug() << "creo brain";
        Brain *b = new Brain(t,&w);
//        qDebug() << "brain fatto";
        Player *ai = new AI(b, id);

        p.append(PlayerPtr(ai));

    }

    return p;

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
    QVector<int> sel(players.size());
    int prob, offset = 0;

    /* ordino in base al punteggio
     * per ottenere un vettore delle probabilita ordinato
     * dalla piu alta alla più piccola
     * questo processo mi aiutera in futuro nella selezione del giocatore
     */
    qSort(players.begin(), players.end(), Engine::compare);

    cout << "entro ciclo 1..." << flush; //debug

    /*
     * costruisco il vettore di selezione
     */
    for(int i = 0; i < players.size(); i++)
    {
        /* resetto gli score per ripartire da 0 con la generazione successiva */
        players[i]->resetScore();
        prob = static_cast<int> (round((p*pow((1-p),i)) * numPlayers * 100))+1;

        /*
         * construisco sel con la probabilita di ogni giocatore di essere scelto
         * l'indice di sel rappresenta l'indice del giocatore e
         * il valore la sua probabilità CUMULATIVA di essere selezionato
         *
         * es.
         *  giocatore 0 prob 50
         *  giocatore 1 prob 20
         *  giocatore 2 prob 10
         *
         *       0  1  2
         * sel: 50 70 80
         *
         */
        sel[i] = offset + prob;
        offset = sel[i];

        //debug
        cout << "[]vettore probabilita:";
        for (int i = 0; i < sel.size(); i++) cout << " " << sel[i];
        cout << endl;
        //debug

    }
    cout << "esco ciclo 1\n"; //debug

    cout << "smadonno forte" << endl; //debug

    cout << "entro ciclo 3..." << flush; //debug
    printf ("%s \n", "A string");
    /* ordinando i giocatori per bravura
     * ottengo che nelle prime posizioni ho individui piu promettenti
     * che vale la pena tenere
     * quindi decido di evolvere solo metà della popolazione
     * e avere cosi la possibilita di superare massimi locali
     */
    for(int i = (players.size()/2); i < players.size(); i++)
    {
        QVector<int> sim_sel = sel;
        PlayerPtr father, mother;
        /* ottengo un numero "gettone" random tra 0 e il valore di sel piu alto
         * e lo utilizzo per selezionare il padre
         */
        int coin = rand() % sel.last();

        for(int i = 0; i < sim_sel.size(); i++)
        {
            /* se il gettone è minore del valore di probabilita dell'array
             * scorrendolo in maniera ordinata
             * allora l'indice è l'indice del giocatore selezionato
             */
            if(coin < sim_sel[i])
            {
                father = players[i];

                /* impedisco che possa venire selezionato di nuovo */
                sim_sel[i] = 0;
                break;
            }
        }

        cout << "entro ciclo 4..." << flush; //debug

        /* ora devo selezionare la madre
         * con la stessa logica usata per il padre
         */
        bool done = false;
        do {
            coin = rand() % sel.last();
            for(int i = 0; i < sim_sel.size(); i++)
            {
                if(coin < sim_sel[i])
                {
                    mother = players[i];
                    done = true;
                    break;
                }
            }
        }while (!done);

        //debug
            cout << "esco ciclo 4\n"; //debug
//        cout << " Indice B "<<idx<<endl;
//        cout<<"rimuovo l'AI "<<i<<"\t\t"<<__FILE__<<":"<<__LINE__<<"\n";
        //debug
        players[i] = crossover(father, mother, p_crossover);
    }
    cout << "esco ciclo 3\n"; //debug

}

/**
 * @brief Engine::crossover
 * @param a
 * @param b
 * @param p
 * @return figlio con meta dei geni del padre e meta dei geni della madre
 */
PlayerPtr Engine::crossover(PlayerPtr a, PlayerPtr b, float p)
{
    std::cout<<std::endl<<"[FACCIO IL CROSSOVER] "<<std::endl; //debug

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
