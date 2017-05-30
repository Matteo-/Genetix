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
//    in.setByteOrder(QDataStream::LittleEndian);
//    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    if (data_size == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> data_size;
    }

    if (tcpSocket->bytesAvailable() < data_size)
        return;

    in >> data;

    //debug
    qDebug() << "[CLIENT] data recived...";
    //debug

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
                                    "Make sure the Genetix server is running, "
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

    /*
     * TODO controllare possibili errori di creazione giocatore
     * magari con un try catch
     */
    QVector<PlayerPtr> p = Engine::deserialize(data);

    int winner = g.run(p[0], p[1]);

    QDataStream out(&result, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    /*
     * pacchetto: | n° vincitore | ID | ID |
     */

    out << winner << p[0]->getID() << p[1]->getID();

    //debug
    qDebug() << "[ProcData client] debug...";
    QDataStream in(result);
    in.setVersion(QDataStream::Qt_4_0);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    QVector<int> res(3);

    /*
     * pacchetto: | n° vincitore | ID | ID |
     */
    in >> res[0] >> res[1] >> res[2];
    qDebug() << "[processdata client] v:" << res[0] << "ID0: " << res[1]
             << "ID1: " << res[2];

    //debug

    data.clear();
    data_size = 0;
    sendResult();
}

void Client::sendResult(){
    /* invio i dati al server */

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
//    out.setByteOrder(QDataStream::LittleEndian);
//    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    /* pacchetto formato da |n° byte data|   data   | */
    out << (int)0;
    out << result;
    out.device()->seek(0);
    out << (int)(block.size() - sizeof(int));

    if( tcpSocket->write(block) == -1)
    {
        qDebug() << "[ERRORE] trasmissione dati";
    }
}
