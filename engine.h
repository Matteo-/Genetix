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
    int generation;
    bool run_flag;      //controllo

    int delay_gen;
    int delay_partita;
    int numPlayers;

    int i,j,n;          //variabili di stato ciclo principale

    Game partita;
    QVector<Player *> players; //giocatori

    static const float score_vittoria;
    static const float score_pareggio;
    static const float score_mossa_valida;
    static const QVector<int> topologia;
};

#endif // ENGINE_H
