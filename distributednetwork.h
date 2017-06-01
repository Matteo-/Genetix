#ifndef DISTRIBUTEDNETWORK_H
#define DISTRIBUTEDNETWORK_H

#include <QTcpServer>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QEventLoop>
#include "worker.h"
#include "sleeper.h"

/**
 * @brief The DistributedNetwork class
 *        Gestisce il sistema di calcolo distribuito
 *
 *        LIMITAZIONI: sia questa Classe che la Classe Client
 *                     devono essere eseguiti su architetture a 64 bit
 */
class DistributedNetwork : public QTcpServer
{
    Q_OBJECT
public:
    explicit DistributedNetwork(int port = 2501, QObject *parent = 0);
    ~DistributedNetwork();

    void startServer(int p);

    /*
     * invia un pacchetto dati al primo client libero disponibile
     */
    void distribute(QByteArray data);

    /*
     * attende che tutti i lavori siano stati eseguiti
     */
    void wait() const;

private:
    bool allDone() const;
    int port;

    /*
     * nel caso alcuni client si disconnettano senza restituire il risultato
     * utilizzo questo per calcolarlo in locale e non perdere partite */
    //Client local_client;

    /*
     * coda dei client connessi
     */
    QVector<Worker*> workers;
    mutable QMutex worker_mutex;

    /*
     * coda dei lavori in attesa di elaborazione
     */
    QQueue<QByteArray> tasks;
    mutable QMutex task_mutex;

    //QDataStream in;

signals:
    void newtask();

    //TODO connettere con engine
    void sendResult(QByteArray &result);


public slots:
    void newClient();
    void getElabData(QByteArray &result);
    void clientDisconnected(int id, QByteArray &data);

protected:
    //void incomingConnection(int socketDescriptor);

};

#endif // DISTRIBUTEDNETWORK_H
