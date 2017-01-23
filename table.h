#ifndef TAVOLO_H
#define TAVOLO_H

#include <QVector>

/**
 * @brief The Tavolo class
 * classe che implementa e gestisce il campo da gioco
 */
class Table
{
public:
    Table();
    void stampa() const;
    QVector< QVector<int> > get() const;
    bool fineGioco() const;
    int calcolaVincitore() const;
    QVector<int> mosseValide(int turno) const;
    QVector<int> mosseNonValide(int turno) const;
    int eseguiMossa(int turno, int mossa);
    int bucheVuote(int turno) const;
    int differenzaPunti(int turno) const;
    void inizializza();
    static int rival(int turno);
private:
    int numMosse(int turno) const;

    QVector< QVector<int> > buca;
    const int fagioli_inizio;
    static const int numero_buche;
};

#endif // TAVOLO_H
