#include "game.h"
#include <iostream>
#include "sleeper.h"

const int Game::turno_iniziale = 0;

Game::Game(Engine *en): engine_control(true),
    bantumi(), eng(en)
{
    std::cout << "[debug] creo partita" << std::endl; //debug

    if(eng)
    {
        connect(this, SIGNAL(mossaErrata()), eng, SLOT(mossaErrata()));

        connect(this, SIGNAL(mossaValida(Player*)),
                eng, SLOT(mossaValida(Player*)));

        connect(this, SIGNAL(vittoria(Player*)),
                eng, SLOT(vincitore(Player*)));

        connect(this, SIGNAL(pareggio(Player*,Player*)),
                eng, SLOT(pareggio(Player*,Player*)));
    }
}

QVector<Player *> Game::run(Player *g1, Player *g2)
{
    //all'inizio ho il permesso di giocare perche va tutto bene
    engine_control = true;

    //aggiungo i giocatori
    QVector<Player *> giocatori;
    giocatori.append(g1);
    giocatori.append(g2);

    //inizializzo il campo da gioco
    bantumi.inizializza();
    turno = turno_iniziale;

    int mossa, controllo, stato_finale;

    while(!bantumi.fineGioco() && engine_control)
    {
        //calcolo la mossa la esegue e controllo che sia andata a buon fine
        //altrimenti ripeto
        do
        {
            mossa = giocatori[turno]->calcolaMossa(bantumi, turno);
            controllo = bantumi.eseguiMossa(turno, mossa);

            std::cout << turno << mossa << controllo << " "; //debug

            //emetto il segnale di mossa sbagliata
            if(controllo == -1 && eng) emit mossaErrata();


            if(controllo == 1) emit mossaValida(giocatori[turno]);

            //sistemo il turno e ripeto
            if(controllo == 0)
            {
                emit mossaValida(giocatori[turno]);
                turno = Table::avversario(turno);
            }

            //se la partita ha un engine non richiedo la mossa
        }while(controllo == -1 && eng == 0);

        //termino la partita
        if(controllo == -1 && eng) engine_control = false;
    }

    if(!engine_control)
    {
        giocatori.resize(0);
        return giocatori;
    }

    stato_finale = bantumi.calcolaVincitore();
    if (stato_finale == 2)
    {
        //cout << "PARITA'" << endl; //debug
        emit pareggio(giocatori[0], giocatori[1]);
        return giocatori;
    }
    else
    {
        //cout << "HA VINTO IL GIOCATORE " << stato_finale << endl; //debug
        //rimuovo il giocatore perdente
        giocatori.remove(Table::avversario(stato_finale));
        emit vittoria(giocatori[0]);
        return giocatori;
    }

}

void Game::stop()
{
    engine_control = false;
}
