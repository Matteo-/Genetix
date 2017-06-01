#include "game.h"
#include <iostream>
#include "sleeper.h"
#include "tree.h"
using namespace std;

int Game::istanze = 0;
const int Game::turno_iniziale = 0;

Game::Game(Engine *en): engine_control(true),
    bantumi(), eng(en)
{
    //debug
    istanze++;
    cout<<"creating Game ["<<istanze<<"]....OK\n";
    //debug
}

Game::~Game()
{

    //debug
    cout<<"deleting Game ["<<istanze<<"]....";
    istanze--;
    cout<<"OK\n";
    //debug
}

/*
 * metodo che si occupa di effettuare la partita tra 2 giocatori
 * @input giocatore 0 e giocatore 1
 * @output 0 se ha vinto il giocatore 0
 *         1 se ha vinto il giocatore 1
 *         2 in caso di parità
 *        -1 se c'è stato un errore
 */
int Game::run(QVector<PlayerPtr> giocatori, Tree *tree)
{
    QVector<data*> *id; //game vector in tree
    if(tree)
    {
       id = tree->open_game();
    }
    //all'inizio ho il permesso di giocare perche va tutto bene
    engine_control = true;

    //aggiungo i giocatori
    //array nativo
//    QVector<PlayerPtr> giocatori(2);
//    giocatori[0] = g1;
//    giocatori[1] = g2;

    //inizializzo il campo da gioco
    bantumi.inizializza();
    turno = turno_iniziale;

    int mossa, controllo;

    while(!bantumi.fineGioco() && engine_control)
    {
        //calcolo la mossa la esegue e controllo che sia andata a buon fine
        //altrimenti ripeto
        do
        {
            mossa = giocatori[turno]->calcolaMossa(bantumi, turno);
            controllo = bantumi.eseguiMossa(turno, mossa);

            QString mex = QString::number(turno)+
                          QString::number(mossa)+
                          QString::number(controllo);
            emit output(mex);
            //cout << turno << mossa << controllo << " " << flush; //debug

            if(tree)
            {
                id->append(new data(mossa,turno));
            }

            //emetto il segnale di mossa sbagliata
            //TODO mossaerrata metterla nel player e far ritornare true o false
            //(umano sempre true) significato continui comunque anche se sei
            //leso?
            //if(controllo == -1 && eng) emit mossaErrata();


            //if(controllo == 1) emit mossaValida(giocatori[turno]);

            //sistemo il turno e ripeto
            if(controllo == 0)
            {
                //emit mossaValida(giocatori[turno]);
                turno = Table::rival(turno);
            }

            //se la partita ha un engine non richiedo la mossa
        }while(controllo == -1 && eng == 0);

        //termino la partita
        if(controllo == -1 && eng) engine_control = false;
    }

    if(tree)
    {
        tree->add(*id);
    }

    if(!engine_control)
    {
        return -1;
    }
    else {
        return bantumi.calcolaVincitore();
    }
}

void Game::stop()
{
    engine_control = false;
}
