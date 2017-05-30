#include "client.h"

Client::Client(QString host, quint16 port, QObject *parent) : QObject(parent)
{
    data_size = 0;
    tcpSocket = new QTcpSocket();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    tcpSocket->connectToHost(host, port);

    qDebug() << "[CLIENT] connesso";
}

void Client::readData()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (data_size == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> data_size;
    }

    if (tcpSocket->bytesAvailable() < data_size)
        return;

    in >> data;

    qDebug() << "[CLIENT] data recived...";

    processData();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(new QWidget, tr("Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(new QWidget, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(new QWidget, tr("Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
}

void Client::processData()
{
//    qDebug() << "process: " << data;

    Game g;
    QVector<PlayerPtr> p = Engine::deserialize(data);

    int winner = g.run(p[0], p[1]);

    QDataStream out(&result, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    /*
     * pacchetto: | n° vincitori | ID | ID |
     */

    out << winner << p[0]->getID() << p[1]->getID();

    //result = data.append("-elab");

    data.clear();
    data_size = 0;
    sendResult();
}

void Client::sendResult(){
    /* invio i dati al server */

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    /* pacchetto formato da |n° byte data|   data   | */
    out << (quint16)0;
    out << result;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    if( tcpSocket->write(block) == -1)
    {
        qDebug() << "[ERRORE] trasmissione dati";
    }
}
