#include "sleeper.h"

void Sleeper::usleep(unsigned long usecs)
{
    QThread::usleep(usecs);
}

void Sleeper::msleep(unsigned long msecs)
{
    QThread::msleep(msecs);
}

void Sleeper::sleep(unsigned long secs)
{
    QThread::sleep(secs);
}
