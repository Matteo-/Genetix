#include "worker.h"

Worker::Worker(QTcpSocket *socket, int id, QQueue<QByteArray> &t, QMutex &m):
    tasks(t), mutex(m)
{
    Socket = socket;
    Id = id;
    working = false;
    result_size = 0;
    getTask();
}

void Worker::readClient()
{
    if(working) {
        QDataStream in(Socket);
        in.setVersion(QDataStream::Qt_4_0);
//        in.setByteOrder(QDataStream::LittleEndian);
//        in.setFloatingPointPrecision(QDataStream::SinglePrecision);

        if (result_size == 0) {

            //non ho abbastanza dati per leggere la dimensione del risultato
            if (Socket->bytesAvailable() < (int)sizeof(quint16))
                return;

            in >> result_size;
        }

        //non ho abbastanza dati per leggere il risultato
        if (Socket->bytesAvailable() < result_size)
            return;

        in >> result;

        qDebug() << "Client scemo numero " << Id << ": " << result;
        working = false;
        emit taskComplete(result);
        result_size = 0;
        result.clear();
        getTask();
    }
}
void Worker::getTask()
{
    //se non sto aspettando il risultato di una elaborazione
    if (!working) {
        {
            QMutexLocker locker(&mutex);
            /* controllo lista vuota */
            if(tasks.isEmpty()) { return; }
            data = tasks.dequeue();
        }

        /* invio i dati al client */

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
//        out.setByteOrder(QDataStream::LittleEndian);
//        out.setFloatingPointPrecision(QDataStream::SinglePrecision);

        /* pacchetto formato da |n° byte data|   data   | */
        out << (quint16)0;
        out << data;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        if( Socket->write(block) == -1)
        {
            qDebug() << "[ERRORE] trasmissione dati";
        }

        qDebug() << "worker " << Id << "data sent";

        working = true;
    }
}

void Worker::disconnected()
{
    /* se ero in attesa di un dato devo rifare il lavoro
     * altrimenti passo il dato vuoto
     */
    if(!working)
    {
        qDebug() << "lavoro svolto posso eliminarlo";
        data.clear();
    }

    qDebug() << "lavoro da rifare: " << data;
    emit disconnected(Id, data);

}

int Worker::getID()
{
    return Id;
}

bool Worker::isWorking()
{
    return working;
}
