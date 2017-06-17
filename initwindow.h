#ifndef INITWINDOW_H
#define INITWINDOW_H

#include <QWidget>
#include "mainwindow.h"
#include "clientwindow.h"
#include "engine.h"

namespace Ui {
class initwindow;
}

class initwindow : public QWidget
{
    Q_OBJECT

public:
    explicit initwindow(QWidget *parent = 0);
    ~initwindow();

private:
    Ui::initwindow *ui;

    Engine *engine;
    MainWindow *w;
    ClientWindow *wclient;

public slots:
    void runGenetix();
    void runClient();
};

#endif // INITWINDOW_H
