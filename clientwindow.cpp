#include "clientwindow.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

ClientWindow::ClientWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Genetix Client");
    resize(600,300);

    QLabel *lhost = new QLabel("Address:", this);
    QLabel *lport = new QLabel("Port:", this);

    host = new QLineEdit("localhost", this);
    port = new QLineEdit("2501", this);

    bConnect = new QPushButton("Connect", this);

    QTextEdit *text_output = new QTextEdit(this);
    text_output->setReadOnly(true);
    QPalette p = text_output->palette();
    p.setColor(QPalette::Base, QColor(0, 0, 0));
    text_output->setPalette(p);
//    text_output->setTextColor(QColor(15, 30, 242));   //blue
//    text_output->setTextColor(QColor(102, 250, 57));  //verde
    text_output->setTextColor(QColor(201, 201, 201)); //grigio

    status = new QLabel("Status: Disconnected", this);
    status->setDisabled(true);

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(lhost, 0, 0);
    grid->addWidget(host, 0, 1);
    grid->addWidget(bConnect, 0, 2, 2, 1);
    grid->addWidget(lport, 1, 0);
    grid->addWidget(port, 1, 1);
    grid->addWidget(text_output, 2, 0, 2, 3);
    grid->addWidget(status, 4, 1);
    setLayout(grid);

    client = new Client();

    connect(client, SIGNAL(output(QString)),
            text_output, SLOT(append(QString)));

    connect(bConnect, SIGNAL(clicked(bool)), this, SLOT(connetti()));

    connect(client, SIGNAL(status(QAbstractSocket::SocketState)),
            this, SLOT(connectionState(QAbstractSocket::SocketState)));



}

void ClientWindow::connetti()
{
    client->connetti(host->text(), port->text().toInt());
}

void ClientWindow::connectionState(QAbstractSocket::SocketState s)
{
    switch (s) {
    case 0:
        bConnect->setEnabled(true);
        status->setText("Status: Disconnected");
        break;
    case 1:
        status->setText("Status: Host lookup...");
        break;
    case 2:
        status->setText("Status: Connecting...");
        break;
    case 3:
        bConnect->setDisabled(true);
        status->setText("Status: Connected");
    default:
        break;
    }
}
