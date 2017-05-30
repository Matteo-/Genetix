#include "distributednetwork.h"

DistributedNetwork::DistributedNetwork(int p, QObject *parent) :
    QTcpServer(parent)
{
    port = p;
    startServer();
}

void DistributedNetwork::startServer()
{
    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }

    connect(this, SIGNAL(newConnection()), this, SLOT(newClient()));
}


void DistributedNetwork::newClient()
{
    qDebug() << "inizializzazione nuovo client";

    QTcpSocket *clientConnection = this->nextPendingConnection();

    {
        QMutexLocker locker(&worker_mutex);
        Worker *client = new Worker(clientConnection,
                                    workers.size(), tasks, task_mutex);
        workers.append(client);

        connect(clientConnection, SIGNAL(readyRead()),
                client, SLOT(readClient()));

        connect(clientConnection, SIGNAL(disconnected()),
                clientConnection, SLOT(deleteLater()));

        connect(clientConnection, SIGNAL(disconnected()),
                client, SLOT(disconnected()));

        connect(client, SIGNAL(disconnected(int,QByteArray&)),
                this, SLOT(clientDisconnected(int,QByteArray&)));

        connect(this, SIGNAL(newtask()), client, SLOT(getTask()));

        connect(client, SIGNAL(taskComplete(QByteArray&)),
                this, SLOT(getElabData(QByteArray&)));
    }

    qDebug() << "new client connected"; //debug

    //debug
    {
        QMutexLocker locker(&worker_mutex);
        qDebug() << "lista client: ";
        for(int i = 0; i < workers.size(); i++) {
            qDebug() << " " << workers[i]->getID();
        }
    }
    //debug
}

void DistributedNetwork::distribute(QByteArray data)
{
    {
        QMutexLocker locker(&task_mutex);
        tasks.enqueue(data);
    }
    emit newtask();
}

/*
 * aspetta che tutti i lavori siano eseguiti
 */
void DistributedNetwork::wait() const
{
    /*
     * aspetto che tutti i lavori siano stati inviati
     */
    QEventLoop el;
    while(!allDone())
    {
        //qDebug() << "allDone cond: " << allDone();
        el.processEvents();
        Sleeper::msleep(50);
    }
}

bool DistributedNetwork::allDone() const
{
    QMutexLocker locker(&worker_mutex);
    // c'è lavoro?
    bool work = false;

    /* se ci sono elementi nella non ho finito
     * se la coda è vuotaguardo se tutti i lavoratori hanno finito
     */
    if(tasks.isEmpty())
    {
        for(int i = 0; i < workers.size(); i++)
        {
            if (workers[i]->isWorking())
            {
                work = true;
            }
        }

        if(work) return false;
        else return true;
    }
    else return false;
}

void DistributedNetwork::getElabData(QByteArray &result)
{
    qDebug() << "[getelabdata server] emetto risultato";
    emit sendResult(result);
}

void DistributedNetwork::clientDisconnected(int id, QByteArray &data)
{
    {
        QMutexLocker locker(&worker_mutex);
        for(int i = 0; i < workers.size(); i++) {
            if(workers[i]->getID() == id)
            {
                Worker *w = workers[i];
                workers.remove(i);
                delete w;
                break;
            }
        }
    }

    //debug
    {
        QMutexLocker locker(&worker_mutex);
        qDebug() << "lista client: ";
        for(int i = 0; i < workers.size(); i++) {
            qDebug() << " " << workers[i]->getID();
        }
    }
    //debug

    // se i dati non sono vuoti li reimmetto nelle task
    if(!data.isEmpty())
    {
        qDebug() << "ributto in coda: " << data;
        distribute(data);
    }
}
