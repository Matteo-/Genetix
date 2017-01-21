#include "mainwindow.h"
#include <QApplication>

#include "engine.h"

//test
#include "brain.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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


//    //test
//    QVector<int> topologia =  {4,3,2,1};
//    Brain Ba(topologia);
//    Brain Bb(topologia);

//    Brain *c = Ba+Bb;

//    Ba.print();
//    Ba.info();
//    std::cout << std::endl;
//    Bb.print();
//    Bb.info();
//    std::cout << "stampa finita" << std::endl;
//    QVector<float> input = {5.0f,2.0f,1.0f,3.0f};
//    QVector<float> result = Bb.getOutput(input);

//    std::cout << "risultato size " << result.size() <<std::endl;
//    //for(int i = 0; i < result.size(); i++)
//        std::cout << result[0] << " ";
//    //test

    w.show();
    a.exec();
}

