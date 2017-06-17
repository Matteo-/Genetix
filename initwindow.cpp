#include "initwindow.h"
#include "ui_initwindow.h"
#include <iostream> //debug
#include <stdio.h> //test
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "tree.h"

initwindow::initwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initwindow)
{
    ui->setupUi(this);

    connect(ui->GenetixButton, SIGNAL(clicked()), this, SLOT(runGenetix()));

    connect(ui->ClientButton, SIGNAL(clicked()), this, SLOT(runClient()));
}

initwindow::~initwindow()
{
    delete ui;
}

void initwindow::runGenetix()
{
    engine = new Engine();
    w = new MainWindow();


    //connetto il numero di generazione con la gui
    QObject::connect(engine,SIGNAL(GenChanged(int)),w,SLOT(changedGen(int)));

    //connetto il pulsante start al metodo run di engine
    QObject::connect(w , SIGNAL(start_clicked()), engine, SLOT(runEvent()));

    //connetto il pulsante stop al metodo stop di engine
    QObject::connect(w, SIGNAL(stop_clicked()), engine, SLOT(stopEvent()));

    //connetto il delay
    QObject::connect(w, SIGNAL(delay_changed(int)),
                     engine, SLOT(setDelay(int)));


    w->show();
    this->hide();
}

void initwindow::runClient()
{
    wclient = new ClientWindow;
    wclient->show();
    this->hide();
}
