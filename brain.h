#ifndef BRAIN_H
#define BRAIN_H

#include <QVector>
#include <QObject>

class Brain : public QObject
{
    Q_OBJECT

public:
    Brain(const QVector<int> &topology, QObject *parent = 0);
    ~Brain();
    friend Brain* operator+ (const Brain&, const Brain&);
    //explicit Brain(QObject *parent = 0);
    QVector<float> getOutput(const QVector<float> &input);
    void backprop(const QVector<float> &out,const QVector<float> &out_expct);
    //void test();
    void print() const;
    void info() const;
    static float randTo(float min, float max);

private:
    void feedForward();
    float squaredError(float t, float y) const;
    void updateWeights(int w_index, int dim, int n_index, float Er);
    void ErrorPropagation(float Er, int l_index);

    QVector<int> m_topology;
    QVector<float> m_neurons;
    QVector<float> m_weights;
    static const float m_bias;
    static const float weightrandmax;
    float FAttivazione(float sum) const;
    //TODO inizializzare e fare funzioni di modifica
    //float eta;
    //float momentum;
    float learning_rate;
    static int istanze;                    //debug memory leak

signals:

public slots:
};

#endif // BRAIN_H
