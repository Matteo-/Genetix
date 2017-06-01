#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    data_size = 0;
    tcpSocket = new QTcpSocket();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(state(QAbstractSocket::SocketState)));
}

Client::~Client()
{
    tcpSocket->close();
    delete tcpSocket;
}

void Client::connetti(QString host, quint16 port)
{
    emit output("in connessione...");

    tcpSocket->connectToHost(host, port);
}

void Client::readData()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
//    in.setByteOrder(QDataStream::LittleEndian);
//    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    if (data_size == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(int))
            return;

        in >> data_size;
        emit output("nuovo pacchetto in arrivo...");
    }

    if (tcpSocket->bytesAvailable() < data_size)
        return;

    in >> data;

    //debug
    //qDebug() << "[CLIENT] data recived...";
    emit output("   ricevuto\n");
    //debug

    processData();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    QWidget *avviso = new QWidget();

    //TODO attenzione memory leak trovare un modo per eliminare avviso

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(avviso, tr("Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(avviso, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the Genetix server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(avviso, tr("Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
}

void Client::processData()
{
    emit output("inizio elaborazione dati...");

    Game g;

    connect(&g, SIGNAL(output(QString)), this, SLOT(getOutput(QString)));

    /*
     * TODO controllare possibili errori di creazione giocatore
     * magari con un try catch
     */
    QVector<PlayerPtr> p = Engine::deserialize(data);

    int winner = g.run({p[0], p[1]});

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
    emit output("   invio risultato...");

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
        emit output("   [ERRORE] trasmissione dati");
    }

    emit output("       OK\n");
}

void Client::state(QAbstractSocket::SocketState s)
{
    if(s == 3) emit output("   connesso\n");
    emit status(s);
}

void Client::getOutput(QString s)
{
    emit output(s);
}
