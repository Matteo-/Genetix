#ifndef BRAIN_H
#define BRAIN_H

#include <QVector>
#include <math.h>       /* exp */
#include <QObject>

class Brain : public QObject
{
    Q_OBJECT

public:
    Brain(const QVector<int> &topology, QObject *parent = 0);
    friend Brain* operator+ (const Brain&, const Brain&);
    //explicit Brain(QObject *parent = 0);
    QVector<float> getOutput(const QVector<float> &input);
    //void test();
    void print() const;
    void info() const;
    static float randTo(float);

private:
    void feedForward();

    QVector<int> m_topology;
    QVector<float> m_neurons;
    QVector<float> m_weights;
    static const float m_bias;
    const static float weightrandmax;
    float FAttivazione(float sum) const;
    //TODO inizializzare e fare funzioni di modifica
    //float eta;
    //float momentum;

signals:

public slots:
};

#endif // BRAIN_H
