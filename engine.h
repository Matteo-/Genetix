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
#include "gameabstract.h"
#include "tester.h"

class Game;

class Engine : public QThread
{
    Q_OBJECT

public:
    Engine();
    void stop();
    void run();

public slots:
    //ciclo principale
    void runEvent();
    void stopEvent();
    void setDelay(int);
    //Game
    void mossaErrata();
    void mossaValida(Player*);
    void vincitore(Player*);
    void pareggio(Player*, Player*);

signals:
    void GenChanged(int newGen);    //segnala l'avanzamento di gen.
    //Game
    void stopGame();                //ferma la partita corrente

private:
    static bool compare(const Player*, const Player*);
    void selezioneTorneo(float p);
    Player* crossover(Player*, Player*, float p) const;
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
    QVector<Player *> players;              //giocatori
    AI best; //introdurre i puntatori smart per rimettere Player *best
    Tester *tester;
    static const float score_vittoria;
    static const float score_pareggio;
    static const float score_mossa_valida;
    static const QVector<int> topologia;
};

#endif // ENGINE_H
