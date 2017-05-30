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
    explicit Client(QString host = "localhost",
                    quint16 port = 2501, QObject *parent = 0);

private slots:
    void readData();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    void processData();
    void sendResult();

    QTcpSocket *tcpSocket;
    QByteArray data;
    quint16 data_size;

    QByteArray result;
};

#endif // CLIENT_H
