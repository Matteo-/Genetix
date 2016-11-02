#ifndef BANTUMI_H
#define BANTUMI_H

#include <giocatore.h>
#include <QVector>

class Bantumi
{
public:
    Bantumi();
    Bantumi(Giocatore *p1, Giocatore *p2);
    Giocatore * run();
private:

    QVector<Giocatore> giocatori;

    int B[2][7]; //tavolo da gioco

    //variabili globali
    int const profondita = 2;
    int const fagioli_inizio = 3;
    int const g_iniziale = 0;

    int giocatore;

    int other(int giocatore) const;
    int calcolo_vincitore();
    bool controllo_fine() const;
    void stampa() const;
    void inizializza();
    bool semina(int buca);
};

#endif // BANTUMI_H
