#ifndef GAMEABSTRACT_H
#define GAMEABSTRACT_H

#include <QObject>
#include "player.h"
#include "tree.h"

class GameAbstract : public QObject
{
    //Q_OBJECT
public:
    virtual int run(QVector<PlayerPtr> giocatori, Tree *) = 0;
    virtual ~GameAbstract() {}
protected:

signals:
    virtual void output(QString s) = 0;

public slots:
    virtual void stop() = 0;

};

#endif // GAMEABSTRACT_H
