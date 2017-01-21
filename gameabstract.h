#ifndef GAMEABSTRACT_H
#define GAMEABSTRACT_H

#include <QObject>
class Player;

class GameAbstract : public QObject
{
    //Q_OBJECT
public:
    virtual QVector<Player*> run(Player *g1, Player *g2) = 0;
    virtual ~GameAbstract() {}

//signals:
//    virtual void mossaErrata() = 0;
//    virtual void mossaValida(Player*) = 0;
//    virtual void vittoria(Player*) = 0;
//    virtual void pareggio(Player*,Player*) = 0;

public slots:
    virtual void stop() = 0;

};

#endif // GAMEABSTRACT_H
