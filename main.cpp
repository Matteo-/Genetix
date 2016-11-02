#include "mainwindow.h"
#include <QApplication>

//TEST
#include <time.h>
//#include <QTextStream>
#include <iostream>
#include <giocatore.h>
#include <partita.h>
#include <tavolo.h>
#include <umano.h>
#include <ai.h>
using namespace std;

//TEST
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //TEST

    //QTextStream cout(stdout);
    Giocatore *g1 = new AI;
    Giocatore *g2 = new AI;

    srand(time(NULL)); //debug
    Partita partita;
    for(int i = 0; i < 200; i++)
    {
        cout << "PARTITA NUMERO " << i << endl;
        partita.run(g1, g2);
    }
    //Tavolo tavolo;
    //tavolo.stampa();

    delete g1;
    delete g2;
    //TEST

    return 0; //a.exec();
}
