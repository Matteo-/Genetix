#include "brain.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h>       /* exp */

int Brain::istanze = 0;
const float Brain::m_bias = 1.0f;
const float Brain::weightrandmax = 2.0f;

Brain::Brain(const QVector<int> &topology, QObject *parent) : QObject(parent),  m_topology(topology)
{
    //debug
    istanze++;
    std::cout<<"creating Brain ["<<istanze<<"]....";
    //debug

  int neurons = 0;  // numero di neuroni totali da allocare
  int weights = 0;  // numero di pesi totali da allocare

  /**
   * calcolo il numero di neuroni
   * e di pesi totali con dei semplici cicli
   * infine ridimensiono ogni vector con la giusta grandezza
   */
  for (int i = 0; i < m_topology.size(); i++) {
    neurons += m_topology[i];
  }
  m_neurons.resize(neurons);

  for (int i = 0; i < m_topology.size()-1; i++) {
    weights += m_topology[i] * m_topology[i+1];
  }
  m_weights.resize(weights);
  //carico valori random
  for(int i = 0; i < m_weights.size(); i++)
      //rand tra -weightrandmax e weightrandmax
      m_weights[i] = randTo(-weightrandmax, weightrandmax);
  std::cout << "OK" << std::endl; //debug
}

Brain::~Brain()
{
    //debug
    std::cout<<"deleting Brain ["<<istanze<<"]....";
    istanze--;
    std::cout<<"OK\n";
    //debug
}

void Brain::feedForward() {

  int pos = 0;        //primo neurone del layer attuale
  int i_weight = 0;   //indice del peso corrente
  float sum;          //sommatoria volore * peso

  /*
   * scorro i layer partendo dal secondo,
   * non va calcolato il valore degli input
   */
  for (int layer = 1; layer < m_topology.size(); layer++) {

    /*
     * tengo traccia della posizione sommando il numero di neuroni
     * del livello precedente ad ogni ciclo
     * in questo modo mi trovo sempre sul primo neurono del layer attuale
     */
    pos+= m_topology[layer-1];

    /*
     * sommando pos + m_topology[layer] trovo l'indice
     * del primo neurone del layer successivo
     * cioè quello da non toccare
     */
    for (int neuron = pos; neuron < pos + m_topology[layer]; neuron++) {
      /*
       * ciclo n volte con n = numero neuroni layer precedente
       * posizionandomi sul primo neurone del layer precedente
       */
       sum = 0;
       for (int j = pos - m_topology[layer-1]; j < pos; j++) {

         sum += m_neurons[j] * m_weights[i_weight];
         i_weight++;
       }
       sum += m_bias; //bias

       /*
        * l'ultimo layer lo sommo e basta per prova
        */
//       if (layer != m_topology.size()-1) {
//         m_neurons[neuron] = FAttivazione(sum);
//       } else {
//           m_neurons[neuron] = sum;
//       }

       //il layer output viene attivato
       if (layer != m_topology.size()) {
           m_neurons[neuron] = FAttivazione(sum);
       }

    }
  }
}

/*
 * controlla se l'imput è giusto
 * lo carica nella rete e calcola l'output
 * @return vettore di output
 */
QVector<float> Brain::getOutput(const QVector<float> &input) {

  QVector<float> out(m_topology.back(), 0.0f);

  //controllo la corrispondenza con i neuroni di imput
  if (input.size() != m_topology[0]) {
    std::cout << "[ERRORE] errato numero di input";
    return out;
  }

  /*
   * inserimento input
   */
  for (int i = 0; i < input.size(); i++) {
    m_neurons[i] = input[i];
  }

  feedForward();

  for (int val = 0; val < out.size(); val++) {
    out[out.size()-1-val] = m_neurons[m_neurons.size()-1-val];
  }

  return out;

}

//void Brain::test() {

//  for (int i = 0; i < m_neurons.size(); i++)
//    m_neurons[i] = (i+1)/10.0f;

//    for (int i = 0; i < m_weights.size(); i++)
//      m_weights[i] = (i+1)/10.0f;
//}

void Brain::print() const {

  for (int i = 0; i < m_neurons.size(); i++)
    std::cout << m_neurons[i] << " ";
  std::cout << std::endl;

  for (int i = 0; i < m_weights.size(); i++)
    std::cout << m_weights[i] << " ";
  std::cout << std::endl;

}

void Brain::info() const {
  float neuronskb = (sizeof(float)*m_neurons.size())/1024.0f;
  float weightskb = (sizeof(float)*m_weights.size())/1024.0f;
  std::cout << "topology: ";
  for (int i = 0; i < m_topology.size(); i++) std::cout << m_topology[i] << " ";
  std::cout << std::endl;

  std::cout << "layers: " << m_topology.size() << std::endl;
  std::cout << "neurons: " << m_neurons.size() << " .... "
    << std::fixed << std::setprecision(2) << neuronskb << "KB" << std::endl;
  std::cout << "weights: " << m_weights.size() << " .... "
    << std::fixed << std::setprecision(2) << weightskb << "KB" << std::endl;
}

Brain* operator+(const Brain &a, const Brain &b)
{
    //debug
    using namespace std;
    cout<<"[STAMPO A]"<<__FILE__<<__LINE__<<"\n";
    a.info();
    cout<<"[STAMPO B]\n";
    b.info();

    //debug

    float prob = 0.01f;

    if(a.m_topology != b.m_topology)
    {
        std::cout<<"[ERRORE TOPOLOGIA DIVERSA]\n"; //debug
        return nullptr;
    }
    else
    {
        Brain *figlio = new Brain(a.m_topology);
        int middle = a.m_weights.size()/2;
        std::cout<<"middle: "<<middle<<"\n";

        QVector<float> mid_a = a.m_weights.mid(0,middle);
        QVector<float> mid_b = b.m_weights.mid(middle,
                                               b.m_weights.size()-1);
        std::cout<<"size a: "<<mid_a.size()<<"size b: "<<mid_b.size()<<
                    "size f: "<<figlio->m_weights.size()<<"\n";

        //mutazioni
        for (int i = 0; i < mid_a.size(); i++)
        {
            if(Brain::randTo(0, 1.0f) <= prob)
            {
                std::cout<<"MUTAZIONE AVVENUTA"<<std::endl; //debug
                mid_a[i] = Brain::randTo(-Brain::weightrandmax,
                                         Brain::weightrandmax);
                // emit mutazione()
            }
        }
        for (int i = 0; i < mid_b.size(); i++)
        {
            if(Brain::randTo(0, 1.0f) <= prob)
            {
                //std::cout<<"MUTAZIONE AVVENUTA"<<std::endl; //debug
                mid_b[i] = Brain::randTo(-Brain::weightrandmax,
                                         Brain::weightrandmax);
                //emit mutazione()
            }
        }

        figlio->m_weights = mid_a + mid_b;
        return figlio;
    }
}

float Brain::FAttivazione(float sum) const
{
    float actfun;
    /*
     * funzione sigmoide ____1____   x = sum
     *                   1 + e^(-x)
     */
    actfun = 1/(1+ exp(-sum));
    return actfun;
}

/**
 * @brief Brain::randTo
 * @param max
 * @return float tra min e max
 */
float Brain::randTo(float min, float max)
{
    return static_cast<float>(rand())/(RAND_MAX/(max-min))+min;

}

void Brain::backprop(const QVector<float> &out,const QVector<float> &out_expct)
{
    float Etot = 0.0f;

    for(int i = 0; i < out.size(); i++)
    {
        Etot += squaredError(out_expct[i], out[i]);
    }
    //std::cout<<"Etot: "<<Etot<<"\n";

}

float Brain::squaredError(float t, float y) const
{
    float learning_rate = 1.0f; //TODO posibilità di modificarla
    return 0.5f * pow((t - y),2) * learning_rate;
}
