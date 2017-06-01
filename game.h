#ifndef PARTITA_H
#define PARTITA_H

#include "player.h"
#include "table.h"
#include <QObject>
#include <QVector>
#include "gameabstract.h"
#include "engine.h"
#include <tree.h>

class Engine;

class Game : public GameAbstract//, public QObject
{
    Q_OBJECT

public:
    explicit Game(Engine *en = 0);
    ~Game();
    int run(QVector<PlayerPtr> giocatori, Tree *tree = nullptr);

signals:
    void output(QString s);
//    void mossaErrata();
//    void mossaValida(PlayerPtr);
//    void vittoria(PlayerPtr);
//    void pareggio(PlayerPtr,PlayerPtr);

public slots:
    void stop();

private:
    int turno; // 0 o 1
    bool engine_control;
    Table bantumi;
    static const int turno_iniziale; //0
    Engine *eng;
    static int istanze;                    //debug memory leak
};

#endif // PARTITA_H
