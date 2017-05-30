#include "brain.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h>       /* exp */

int Brain::istanze = 0;
const float Brain::m_bias = 1.0f;
const float Brain::weightrandmax = 1.0f; //from -weightra.. to +weighra..
float Brain::mutation_prob = 0.015f;

Brain::Brain(const QVector<int> &topology, const QVector<float> *w, QObject *parent) : QObject(parent),  m_topology(topology), learning_rate(0.3f)
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

    if(w) {

//        std::cout << "[costr brain] creo pesi"; //debug
        m_weights = *w;
//        std::cout << "[costr brain] finito creo pesi"; //debug
    }
    else {
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
    //TODO try catch
  }

  /*
   * inserimento input
   */
  for (int i = 0; i < input.size(); i++) {
    m_neurons[i] = input[i];
  }

  feedForward();

  //primo neurone di output
  int first_n_out = m_neurons.size()-m_topology.last();
  //ritorno il campo di neuroni di output
  return m_neurons.mid(first_n_out, m_topology.last());
  //debug controllare che ritorni giusto

//  for (int val = 0; val < out.size(); val++) {
//    out[out.size()-1-val] = m_neurons[m_neurons.size()-1-val];
//  }

//  return out;

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
//    using namespace std;
//    cout<<"[STAMPO A]"<<__FILE__<<__LINE__<<"\n";
//    a.info();
//    cout<<"[STAMPO B]\n";
//    b.info();
    //debug

    if(a.m_topology != b.m_topology)
    {
        std::cout<<"[ERRORE TOPOLOGIA DIVERSA]\n"; //debug
        return nullptr;
    }
    else
    {
        Brain *figlio = new Brain(a.m_topology);
        int middle = a.m_weights.size()/Brain::randTo(2.0f, 5.0f); //provo con randTo (prima era 2)

//        std::cout<<"middle: "<<middle<<"\n"; //debug

        QVector<float> mid_a = a.m_weights.mid(0, middle);
        QVector<float> mid_b = b.m_weights.mid(middle, b.m_weights.size()-1);

        //debug
//        std::cout<<"size a: "<<mid_a.size()<<"size b: "<<mid_b.size()<<
//                    "size f: "<<figlio->m_weights.size()<<"\n";
        //debug

        //mutazioni
        for (int i = 0; i < mid_a.size(); i++)
        {
            if(Brain::randTo(0, 1.0f) <= Brain::mutation_prob)
            {
                //std::cout<<"MUTAZIONE AVVENUTA"<<std::endl; //debug
                mid_a[i] = Brain::randTo(-Brain::weightrandmax,
                                         Brain::weightrandmax);
                // emit mutazione()
            }
        }
        for (int i = 0; i < mid_b.size(); i++)
        {
            if(Brain::randTo(0, 1.0f) <= Brain::mutation_prob)
            {
                //std::cout<<"MUTAZIONE AVVENUTA"<<std::endl; //debug
                mid_b[i] = Brain::randTo(-Brain::weightrandmax,
                                         Brain::weightrandmax);
                //emit mutazione()
            }
        }

//        figlio->m_weights = mid_a + mid_b;
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

    float n_out;
    float Ersum = 0;
    int num_n_up;   //numero neuroni del layer superiore
    int last_l = m_topology.size()-1;      //inizializo a indice ultimo layer

    //mi posiziono sul primo neurone di output
    int n_index = m_neurons.size()-m_topology[last_l];
    //inizializzo al primo peso del primo neurone di output
    int w_index = m_weights.size()-(m_topology[last_l-1]*m_topology[last_l]);
    QVector<float> Er(m_topology[last_l]); //vettore errori
    QVector<float> newEr(m_topology[last_l-1]);

    //controllo che non sia cambiato nulla nella rete
    if( m_neurons.mid(n_index, m_topology[last_l]) != out ) {
        std::cout<<"[ERRORE] rete cambiata\t\t"<<__FILE__<<__LINE__<<"\n";
        exit(1); //TODO sostituire con try catch
    }

//    std::cout<<"[INIT] last_l: "<<last_l<<" n_index: "<<n_index<<
//               " w_index: "<<w_index<<"\n"; //debug

    for(int l_index = m_topology.size()-1; l_index > 0; l_index--)
    {

//        std::cout<<"[Layer] "<<l_index<<"\n"; //debug

        /* numero neuroni del prossimo layer*/
        num_n_up = m_topology[l_index-1];

        //scorro tutti i neuroni dello strato
        for(int offset = 0; offset < m_topology[l_index]; offset++)
        {
            //se sono nello strato di output
            if(l_index == m_topology.size()-1)
            {
//                std::cout<<"\tlayer di output\n"<<
//                           "\t[STATO] offset: "<<offset<<" n_index: "<<n_index<<
//                           " w_index: "<<w_index<<"\n"; //debug


                //ErrorB = OutputB(1-OutputB)(TargetB – OutputB)
                Er[offset] = m_neurons[n_index+offset]*(1-m_neurons[n_index+offset])*
                        (out_expct[offset]-m_neurons[n_index+offset]);

//                std::cout<<"\tErrore: "<<Er[offset]<<"\n"; //debug

                /* aggiorno i pesi */
                            /* aggiorno l'ofset del peso */
                updateWeights(w_index+(offset*num_n_up), num_n_up, n_index, Er[offset]);
            }
            else
            {

//                //debug
//                std::cout<<"\tEr[]: ";
//                for(int i = 0; i < Er.size(); i++) {
//                    std::cout<<Er[i]<<" ";
//                }
//                std::cout<<"\n";
//                std::cout<<"\tnewEr[]: ";
//                for(int i = 0; i < newEr.size(); i++) {
//                    std::cout<<newEr[i]<<" ";
//                }
//                std::cout<<"\n";
//                //debug

//                std::cout<<"\thidden layer\n"<<
//                           "\t[STATO] offset: "<<offset<<" n_index: "<<n_index<<
//                           " w_index: "<<w_index<<"\n"; //debug

                //calcolo il nuovo errore
                //δA = outA(1 – outA)(δαWAα+δβWAβ)
                n_out = m_neurons[n_index+offset];
                for(int e = 0; e < Er.size(); e++)
                {
                    Ersum += Er[e]*m_weights[w_index+offset+(e*m_neurons[l_index+1])];

//                    std::cout<<"\t\tErsum: "<<Ersum<<" Er[e]: "<<Er[e]<<" e: "<<e<<
//                               " m_weights[calc]: "<<
//                               m_weights[w_index+offset+(e*m_neurons[l_index+1])]<<"\n";
                }
                newEr[offset] = n_out * (1- n_out)*Ersum;

//                std::cout<<"\tErrore: "<<newEr[offset]<<"\n"; //debug

                /* aggiorno i pesi */
                            /* aggiorno l'ofset del peso */
                updateWeights(w_index+(offset*num_n_up), num_n_up, n_index,
                              newEr[offset]);

            }


        }

        if(l_index != m_topology.size()-1)
        {
            //il nuvo errore diventa quello vecchio
            //e costruisco il nuovo vettore di errori
            Er = newEr;
            newEr.resize(m_topology[l_index-1]);
        }

        //aggiorno l'indice del primo neurone
        //sottraendo la quantita di neuroni presenti nello strato
        n_index -= m_topology[l_index-1];

        if(l_index >= 2) {
            //aggiorno l'indice del primo peso
            w_index -= (m_topology[l_index-2]*m_topology[l_index-1]);
        }
        else { break; }
    }
}

/**
 * @brief Brain::updateWeights aggiorna i pesi del neurone
 * @param w_index
 * @param dim
 * @param n_index
 */
void Brain::updateWeights(int w_index, int dim, int n_index, float Er)
{
    float n_out;
    //scorro i pesi del neurone
    for(int offset = 0; offset < dim; offset++)
    {
        /* tovo il valore di output del neurone da cui arriva il segnale
         * sottraendo all'indice del primo neurone del layer il numero di
         * neuroni del layer preedente
         */
        n_out = m_neurons[n_index-dim+offset];

        /* aggiorno i pesi */
        m_weights[w_index+offset] = m_weights[w_index+offset] +
                (Er * learning_rate * n_out);
    }
}

float Brain::squaredError(float t, float y) const
{
    float learning_rate = 1.0f; //TODO posibilità di modificarla
    return 0.5f * pow((t - y),2) * learning_rate;
}

QVector<float>& Brain::getWeights()
{
    return m_weights;
}

QVector<int>& Brain::getTopology()
{
    return m_topology;
}
