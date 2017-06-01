#include "tree.h"
#include "iostream"
#include <fstream>
using namespace std;

Tree::Tree(): tree(6, nullptr) {}

Tree::~Tree()
{
    for(int i = 0; i < tree.size(); i++) delete tree[i];
}

QVector<data *>* Tree::open_game() const
{
    return new QVector<data*>();
}

void Tree::add(QVector<data *> &id)
{
    recursive_add(tree, id);

    //dealloco id
    for(int i = 0; i < id.size(); i++) delete id[i];
    delete &id;
}

void Tree::recursive_add(QVector<nodo*> &tree, QVector<data*> &id)
{
    if(id.size() > 0) {
        int m = id[0]->mossa;

        //se il nodo non è ancora stato giocato
        //quindi non esiste
        if(tree[m] == nullptr)
        {
            int t = id[0]->turno;
            tree[m] = new nodo(m,t);
            id.remove(0);
            recursive_add(tree[m]->next,id);
        }
        else{
            id.remove(0);
            recursive_add(tree[m]->next, id);
        }
    }
}

void Tree::stampa() const
{
    ofstream out;
    out.open("tree.txt",  ios::out);
    for(int i = 0; i < tree.size(); i++)
        if(tree[i] != nullptr)
        {
            int t = tree[i]->turno;
            int m = tree[i]->mossa;

            out<<"("<<t<<m<< ")\n";
            stampa_ric(tree[i]->next, 1, out);
        }
}

void Tree::info() const
{

}

void Tree::stampa_ric(QVector<nodo *> tree, int depth, ofstream& out) const
{
    for(int i = 0; i < tree.size(); i++)
        if(tree[i] != nullptr)
        {
            int t = tree[i]->turno;
            int m = tree[i]->mossa;

            for(int i = 0; i < depth; i++)
                out<<" ";
            out<<"("<<t<<m<< ")\n";
            stampa_ric(tree[i]->next, depth+1, out);
        }
}
