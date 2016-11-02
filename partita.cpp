#include "partita.h"

#include <iostream>

Partita::Partita(): turno_iniziale(0), bantumi(), giocatori()
{
    cout << "[debug] creo partita" << endl; //debug
}

QVector<Giocatore*> Partita::run(Giocatore* g1, Giocatore *g2)
{
    //aggiungo i giocatori
    giocatori.append(g1);
    giocatori.append(g2);

    //inizializzo il campo da gioco
    bantumi.inizializza();
    turno = turno_iniziale;

    int mossa, controllo, stato_finale;

    while(!bantumi.fineGioco())
    {
        //calcolo la mossa la esegue e controllo che sia andata a buon fine
        //altrimenti ripeto
        do
        {
            mossa = giocatori[turno]->calcolaMossa(bantumi, turno);
            controllo = bantumi.eseguiMossa(turno, mossa);

            cout << turno << mossa << controllo << " ";

            //sistemo il turno e ripeto
            if(controllo == 0) turno = abs(turno-1);

        }while(controllo == -1);
    }

    stato_finale = bantumi.calcolaVincitore();
    if (stato_finale == 2)
    {
        cout << "PARITA'" << endl; //debug
        return giocatori;
    }
    else
    {
        cout << "HA VINTO IL GIOCATORE " << stato_finale << endl; //debug
        //rimuovo il giocatore perdente
        giocatori.remove(abs(stato_finale-1));
        return giocatori;
    }

}
