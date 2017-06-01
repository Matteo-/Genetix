#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include "client.h"

class ClientWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWindow(QWidget *parent = 0);

private:
    Client *client;

    QLineEdit *host;
    QLineEdit *port;
    QPushButton *bConnect;
    QLabel *status;

signals:

public slots:
    void connetti();
    void connectionState(QAbstractSocket::SocketState s);
};

#endif // CLIENTWINDOW_H
