#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QThread>

#include <iostream>
#include "player.h"
#include "game.h"
#include "table.h"
#include "umano.h"
#include "ai.h"
#include "random.h"
#include "gameabstract.h"
#include "tester.h"
#include "tree.h"
#include "distributednetwork.h"
#include <QDataStream>

class Game;

class Engine : public QThread
{
    Q_OBJECT

public:
    Engine();
    ~Engine();
    void stop();
    void run();
    static QByteArray serialize(QVector<PlayerPtr> p);
    static QVector<PlayerPtr> deserialize(QByteArray data);


public slots:
    //ciclo principale
    void runEvent();
    void stopEvent();
    void setDelay(int);
    void fitness(QByteArray &elab);
    //Game
    //void mossaErrata();
    //void mossaValida(PlayerPtr);
    //void vincitore(PlayerPtr);
    //void pareggio(PlayerPtr, PlayerPtr);

signals:
    void GenChanged(int newGen);    //segnala l'avanzamento di gen.
    //Game
    void stopGame();                //ferma la partita corrente

private:
    static bool compare(const PlayerPtr, const PlayerPtr);
    void selezioneTorneo(float p);
    PlayerPtr crossover(PlayerPtr, PlayerPtr, float p);
    void fitness(PlayerPtr p0, PlayerPtr p1);
    float fitnessAVG() const;
    int generation;
    bool run_flag;                          //controllo
    int delay_gen;
    int delay_partita;
    int numPlayers;
    float p_crossover;
    float p_selezione;
    int i,j,n;                              //variabili di stato ciclo principale
    GameAbstract *partita;
    QVector<PlayerPtr> players;              //giocatori
    PlayerPtr best;
    static const float score_vittoria;
    static const float score_pareggio;
    static const float score_sconfitta;
    static const float score_mossa_valida;
    static const QVector<int> topologia;
    static int istanze;                    //debug memory leak
    Tree *tree;
    DistributedNetwork server;
};

#endif // ENGINE_H
