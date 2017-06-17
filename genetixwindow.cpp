#include "genetixwindow.h"
#include "ui_genetixwindow.h"

GenetixWindow::GenetixWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GenetixWindow)
{
    ui->setupUi(this);
}

GenetixWindow::~GenetixWindow()
{
    delete ui;
}
