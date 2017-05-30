#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QDataStream>

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(QTcpSocket *socket, int id, QQueue<QByteArray> &t, QMutex &m);
    int getID();
    bool isWorking();

signals:
    void taskComplete(QByteArray &result);
    void disconnected(int id, QByteArray &data);

public slots:
    void readClient();
    void getTask();
    void disconnected();

private:
    QQueue<QByteArray> &tasks;
    QMutex &mutex;

    QTcpSocket *Socket;
    int Id;
    bool working;

    QByteArray data;

    //ridefinire funzione di invio
    QByteArray result;
    qint16 result_size;
};

#endif // WORKER_H
