#ifndef PARTITA_H
#define PARTITA_H

#include "player.h"
#include "table.h"
#include <QObject>
#include <QVector>
#include "gameabstract.h"
#include "engine.h"

class Engine;

class Game : public GameAbstract//, public QObject
{
    Q_OBJECT

public:
    explicit Game(Engine *en = 0);
    QVector<Player*> run(Player *g1, Player *g2);

signals:
    void mossaErrata();
    void mossaValida(Player*);
    void vittoria(Player*);
    void pareggio(Player*,Player*);

public slots:
    void stop();

private:
    int turno; // 0 o 1
    bool engine_control;
    Table bantumi;
    static const int turno_iniziale; //0
    Engine *eng;
};

#endif // PARTITA_H
