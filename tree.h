#ifndef TREE_H
#define TREE_H

#include <QVector>

struct nodo {
    int mossa;
    int turno;

    QVector<nodo *> next;

    nodo(int m, int t): next(6, nullptr)
    {
        mossa = m;
        turno = t;
    }

    ~nodo() {
        //TODO fare eliminazione ricorsiva di nodo
        for(int i = 0; i < next.size(); i++) delete next[i];
        delete this;
    }
};

struct data {
    int mossa;
    int turno;

    data(int m, int t){ mossa = m; turno = t;}
};


class Tree
{
public:
    Tree();
    ~Tree();
    QVector<data *>* open_game() const;
    void add(QVector<data *> &id);

    void stampa() const;
    void info() const;

    QVector<nodo *> tree;
private:
    void recursive_add(QVector<nodo *> &tree, QVector<data *> &id);
    void stampa_ric(QVector<nodo*> tree, int depth, std::ofstream &) const;
};

#endif // TREE_H
