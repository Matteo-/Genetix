#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDataStream>
#include <QWidget>
#include "engine.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void connetti(QString host = "localhost", quint16 port = 2501);

signals:
    void output(QString);
    void status(QAbstractSocket::SocketState s);

private slots:
    void state(QAbstractSocket::SocketState s);
    void readData();
    void displayError(QAbstractSocket::SocketError socketError);
    void getOutput(QString s);

private:
    void processData();
    void sendResult();

    QTcpSocket *tcpSocket;
    QByteArray data;
    int data_size;

    QByteArray result;
};

#endif // CLIENT_H
