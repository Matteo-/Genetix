#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

#include "engine.h"
#include "sleeper.h"
#include "style.h"

#include <iostream> //debug
#include <stdio.h> //test
#include <QThread>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "tree.h"
#include "client.h"

class T : public QThread
{
    void run() { Sleeper::sleep(5); }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(Style::logo());
    QSplashScreen splash(pixmap);
    splash.setEnabled(false);
    splash.show();
    Sleeper::msleep(20);
    a.processEvents();

    Engine engine;
    MainWindow w;

    //connetto il numero di generazione con la gui
    QObject::connect(&engine,SIGNAL(GenChanged(int)),&w,SLOT(changedGen(int)));

    //connetto il pulsante start al metodo run di engine
    QObject::connect(&w , SIGNAL(start_clicked()), &engine, SLOT(runEvent()));

    //connetto il pulsante stop al metodo stop di engine
    QObject::connect(&w, SIGNAL(stop_clicked()), &engine, SLOT(stopEvent()));

    //connetto il delay
    QObject::connect(&w, SIGNAL(delay_changed(int)),
                     &engine, SLOT(setDelay(int)));

    //display splash screen
    for(int i = 0; i <= 4; i++) {
        a.processEvents();
        Sleeper::msleep(500);
    }

    srand (time(NULL));

    //test
    //    Tree tree;

    //    int p;
    //    std::cin >> p;
    //    for(int i = 0; i < p; i++) {
    //    Game prova;
    //    prova.run(PlayerPtr(new AI(new Brain({5,1}))),
    //              PlayerPtr(new AI(new Brain({5,1}))), &tree);
    //    }

    //    tree.stampa();
    //test

    w.show();
    splash.finish(&w);

    //Client c;

//    Client c("10.0.0.106");

a.exec();
}

