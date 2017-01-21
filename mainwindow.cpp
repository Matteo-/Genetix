#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFont>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    QFont buttonFont;
    buttonFont.setBold(true);
    buttonFont.setPixelSize(20);

    setWindowTitle("Genetix");
    resize(300,200);

    QPushButton *StartButton = new QPushButton("Start", this);
    StartButton->setFont(buttonFont);
    StartButton->setGeometry(100,200,50,100);
    connect(StartButton, SIGNAL(clicked()), this, SLOT(on_start_clicked()));

    QPushButton *StopButton = new QPushButton("Stop", this);
    StopButton->setFont(buttonFont);
    connect(StopButton, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

    lcdGen = new QLCDNumber(6, this);
    lcdGen->setSegmentStyle(QLCDNumber::Filled);

    QLCDNumber *delay = new QLCDNumber(4, this);
    delay->setSegmentStyle(QLCDNumber::Filled);
    delay->display(50);

    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0,500);
    slider->setValue(50);
    connect(slider, SIGNAL(valueChanged(int)),delay, SLOT(display(int)));
    connect(slider, SIGNAL(valueChanged(int)),this, SLOT(on_delay_changed(int)));




    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(StartButton);
    layout->addWidget(StopButton);
    layout->addWidget(lcdGen);
    layout->addWidget(slider);
    layout->addWidget(delay);
    setLayout(layout);
}

void MainWindow::changedGen(int gen){
    lcdGen->display(gen);
}

void MainWindow::on_start_clicked(){
    emit start_clicked();
}

void MainWindow::on_stop_clicked()
{
    emit stop_clicked();
}

void MainWindow::on_delay_changed(int d){
    emit delay_changed(d);
}
