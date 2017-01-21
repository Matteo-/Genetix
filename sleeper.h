#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

/**
 * @brief semplice classe per avere un delay
 */
class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs);
    static void msleep(unsigned long msecs);
    static void sleep(unsigned long secs);
};

#endif // SLEEPER_H
