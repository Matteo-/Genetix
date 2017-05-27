#ifndef GAMEABSTRACT_H
#define GAMEABSTRACT_H

#include <QObject>
#include "player.h"
#include "tree.h"

class GameAbstract : public QObject
{
    //Q_OBJECT
public:
    virtual int run(PlayerPtr g1, PlayerPtr g2, Tree *) = 0;
    virtual ~GameAbstract() {}
protected:

//signals:
//    virtual void mossaErrata() = 0;
//    virtual void mossaValida(Player*) = 0;
//    virtual void vittoria(Player*) = 0;
//    virtual void pareggio(Player*,Player*) = 0;

public slots:
    virtual void stop() = 0;

};

#endif // GAMEABSTRACT_H
