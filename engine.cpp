#include "engine.h"
#include <time.h>
//#include <qthread.h>
#include <iostream> //debug
#include <QtConcurrentRun>
#include <QtAlgorithms>
#include "sleeper.h"
#include <math.h>
#include <QFile>
using namespace std;

int Engine::istanze = 0;
const float Engine::score_vittoria = 10.0f;
const float Engine::score_pareggio = 5.0f;
const float Engine::score_sconfitta = -0.0f;
const QVector<int> Engine::topologia = {5,1};

/**
 * @brief costruttore
 */
Engine::Engine() :
    generation(0),
    run_flag(false),
    delay_gen(1),
    delay_partita(50),
    numPlayers(4),
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
//    std::cout<<"creating Engine ["<<istanze<<"]....OK\n";
    //debug

    srand(time(NULL));
    //carico il vettore dei giocatori
    for(int i = 0; i < players.size(); i++)
    {
        Player *an = new AI(new Brain(topologia));
        players[i] = PlayerPtr(an);
//        std::cout<<"[CARICATO]"<<i<<"\n"; //debug
    }

    //debug
//    for(int i = 0; i < players.size(); i++) {
//        PlayerPtr prova = players[i];
//        std::cout<<"player "<< players[i]->getID() <<" check\n";
//    }
    //debug

    //connessioni
    connect(this, SIGNAL(stopGame()), partita, SLOT(stop()));

    connect(&server, SIGNAL(sendResult(QByteArray&)),
            this, SLOT(fitness(QByteArray&)));

}

Engine::~Engine()
{
    delete partita;
    delete tree;
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
    //QVector<PlayerPtr> result;

    while (run_flag)    //ciclo principale
    {
        Sleeper::msleep(delay_gen);

        for(; i < players.size() && run_flag; i++)
        {
            for(; j < players.size() && run_flag; j++)
            {
                if(players[i]->getID() != players[j]->getID())
                {
                    //debug
                    std::cout << std::endl <<"GEN "<< generation //debug
                        << " PARTITA NUMERO " << n
                        << " ga "<< players[i]->getID()
                        << " gb "<< players[j]->getID() <<std::endl; //debug

                    qDebug() << "[array giocator]";
                    for(int k = 0; k < players.size(); k++)
                    {
                        qDebug() << players[k]->getID();
                    }
                    //debug

                    server.distribute(serialize({players[i], players[j]}));

                    n++;
                    Sleeper::msleep(delay_partita);
                }
            }
            j = 0;
        }

        if(run_flag)
        {
            i = 0, j = 0, n = 0;             //resetto lo stato

            server.wait();

            //debug stampo i punteggi
            for(int i = 0; i < numPlayers; i++)
                std::cout<<"("<<i<<","<<players[i]->getScore()<<") ";
            std::cout<<std::endl;
            //debug


            save();                          //salvo la generazione
            generation++;                    //aumento il numero di generazione
            emit GenChanged(generation);     //emetto il segnale

            selezioneTorneo(p_selezione);
        }
        else
        {
            //aspetto eventuali partite in coda prima dello stop
            server.wait();
        }

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

// funzione per algoritmo di sort
//TODO rifare con overloading operator<
bool Engine::compare(const PlayerPtr a, const PlayerPtr b)
{
    // nego per ottenere l'ordine decrescente
    return !( *(a.get()) < *(b.get()) );
}

void Engine::fitness(QByteArray &elab)
{
    qDebug() << "[fitness] faccio la fitness";

    QDataStream in(elab);
    in.setVersion(QDataStream::Qt_4_0);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    QVector<int> res(3);

    /*
     * pacchetto: | n° vincitore | ID | ID |
     */
    in >> res[0] >> res[1] >> res[2];
    qDebug() << "[fitness] v:" << res[0] << "ID0: " << res[1] << "ID1: " << res[2];

    PlayerPtr p0, p1;
    int find = 0;
    for(int i = 0; i < players.size(); i++)
    {
        if(players[i]->getID() == res[1]) { p0 = players[i]; find++;
        qDebug() << "[fitness]trovato p0"; }

        if(players[i]->getID() == res[2]) { p1 = players[i]; find++;
        qDebug() << "[fitness]trovato p1"; }

        if(find == 2) break;
    }

    //getchar(); //debug

    switch (res[0]) {
    case 0:
        p0->win(score_vittoria);
        p1->lose(score_sconfitta);
        cout << "HA VINTO IL GIOCATORE " << res[1] << std::endl; //debug
        break;
    case 1:
        p0->lose(score_sconfitta);
        p1->win(score_vittoria);
        cout << "HA VINTO IL GIOCATORE " << res[2] << std::endl; //debug
        break;
    case 2:
        p0->parity(score_pareggio);
        p0->parity(score_pareggio);
        cout << "PARITA'" << std::endl; //debug
        break;
    default:
        break;
    }

    qDebug() << "[fitness] fine";
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

        qDebug() << "[serialize] serializzato id:" << s.ID;
        //getchar(); //debug
    }

    //debug
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_4_0);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    qDebug() << "[serialize] print data";
    int tmp;
    float ftmp;
    int dim;
    while(!in.atEnd()) {
        in >> dim;
        qDebug() << dim;
        for(int i = 0; i < dim; i++)
        {
            in >> tmp;
            qDebug() << tmp;
        }

        in >> dim;
        qDebug() << dim;
        for(int i = 0; i < dim; i++)
        {
            in >> ftmp;
            qDebug() << ftmp;
        }

        in >> tmp;
        qDebug() << tmp;
    }

    //debug

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
    qDebug() << "[deserialize] print data;";
    while(!in.atEnd()) {
        in >> dim;
        qDebug() << dim;
        QVector<int> t(dim);
        for(int i = 0; i < dim; i++)
        {
            in >> t[i];
            qDebug() << t[i];
        }

        in >> dim;
        qDebug() << dim;
        QVector<float> w(dim);
        for(int i = 0; i < dim; i++)
        {
            in >> w[i];
            qDebug() << w[i];
        }

        in >> id;
        qDebug() << id;

        Brain *b = new Brain(t,&w);
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

    /*
     * salvo la rete migliore
     */
    PlayerPtr first = players[0];
    if(best == nullptr || best->getScore() < first->getScore())
    {
        best = first;
        std::cout << best->statistics();
    }

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
        QByteArray ser = serialize({a});
        PlayerPtr copia = deserialize(ser).first();
        copia->setID();
        copia->resetScore();
        return copia;
    }
}

/**
 * @brief Engine::save funzione che salva lo stato dell'Engine
 * @param s destinazine del file
 * @return 0 se è andato tutto a buon fine
 *        -1 se è stato impossibile salvare
 */
int Engine::save(QString s)
{
    QString file_name = QString(s+"Gen-%1.dat").arg(generation);
    QFile file( file_name );
    if( !file.open( QIODevice::WriteOnly ) )
        return -1;

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_4_0 );
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    QByteArray dat = serialize(players);

    stream << dat;

    file.close();
    return 0;
}
