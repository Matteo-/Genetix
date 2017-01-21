#include "table.h"
#include <iostream>
//#include <QTextStream>
using namespace std;

const int Table::numero_buche = 7;

Table::Table(): buca(2), fagioli_inizio(3)
{
    cout << "[debug] creo un tavolo" << endl; //debug

    buca[0].resize(numero_buche);
    buca[1].resize(numero_buche);
    inizializza();

}



void Table::inizializza()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 7; j++)
            buca[i][j] = fagioli_inizio;

    buca[0][6] = 0;
    buca[1][6] = 0;
}

//stampo il campo da gioco
void Table::stampa() const
{
    //QTextStream cout(stdout);
    //system("clear"); //debug

    cout << "              bantumi:\n";
    cout << "      5    4    3    2    1    0\n";
    cout << "     ~~   ~~   ~~   ~~   ~~   ~~\n    ";
    for (int i = 6; i > 0; i-- )
        cout << "|" << buca[0][i-1] << ((buca[0][i-1] < 10) ? " " : "") << "| ";
    cout << "\n     ~~   ~~   ~~   ~~   ~~   ~~\n";
    cout << " ~~~~                           ~~~~\n";
    cout << "|    |                         |    |\n";
    cout << "| " << buca[0][6] << ((buca[0][6] < 10) ? " " : "");
    cout << " |                         | ";
    cout << buca[1][6]<< ((buca[1][6] < 10) ? " " : "") << " |\n";
    cout << "|    |                         |    |\n";
    cout << " ~~~~                           ~~~~\n";
    cout << "     ~~   ~~   ~~   ~~   ~~   ~~\n    ";
    for (int i = 0; i < 6; i++ )
        cout << "|" << buca[1][i] << ((buca[1][i] < 10) ? " " : "") << "| ";
    cout << "\n     ~~   ~~   ~~   ~~   ~~   ~~\n";
    cout << "      0    1    2    3    4    5\n";
}

/**
 * @brief Partita::controllo
 * @param turno
 * @param mossa
 * @return -1 se mossa non valida, 0 se fine turno, 1 se ha un altro turno
 */
int Table::eseguiMossa(int turno, int mossa)
{
    //controllo se è una mossa valida
    if (mossa < 0 || mossa > 5 || buca[turno][mossa] == 0)
            return -1;

    //mi salvo il numero di fagioli nella buca e il turno iniziale
    int fagioli = buca[turno][mossa];
    int turno_virtuale = turno;

    //azzero la buca
    buca[turno][mossa] = 0;

    //mi posiziono sulla buca appena svuotata
    int pos = mossa;

    //semino
    for (int i = 0; i < fagioli; i++)
    {
        //incremento la posizione
        pos = (pos+1)%7;
        //se pos == 0 allora ho ricominciato il giro
        //quindi devo cambiare giocatore
        if(pos == 0) turno_virtuale = abs(turno_virtuale-1);

        //se sono nella buca grande avversaria la salto
        //altrimenti semino
        if(pos == 6 && turno_virtuale == abs(turno-1))
            i--;
        else
            buca[turno_virtuale][pos]++;
    }

    //se finisco nella mia buca grande ho diritto ad un altro turno
    if(pos == 6 && turno_virtuale == turno) return 1;

    //cattura
    if(turno_virtuale == turno && buca[turno_virtuale][pos] == 1)
    {
        //cout << "[debug] cattura" << endl;
        buca[turno][pos] = 0;

        //trovo buca avversario
        turno_virtuale = abs(turno-1);
        pos = 5-pos;

        buca[turno][6] += 1+buca[turno_virtuale][pos];
        buca[turno_virtuale][pos] = 0;
    }

    //fine turno
    return 0;
}

/**
 * @brief Tavolo::numeroMosseValide
 * @param turno
 * @return numero delle mosse valide per il giocatore di quel turno
 */
int Table::numMosse(int turno) const
{
    int n = 0;
    for (int i = 0; i < 6; i++)
        if(buca[turno][i] != 0)
            n++;
    return n;
}

/**
 * if the array of playr 0 or player 1 is all empty
 * then the game is finish
 *
 * return true if endgame is reached
 * return false if game can be continued
 */
bool Table::fineGioco() const
{
    if(!numMosse(0) || !numMosse(1)) return true;
    else return false;
}


/**
 * @brief Tavolo::vincitore trova il giocatore che ha vinto la partita
 * @return 0 se ha vinto giocatore 0
 *         1 se ha vinto giocatore 1
 *         2 se pareggio
 */
int Table::calcolaVincitore() const
{
    if ( buca[0][6] == buca[1][6] )
        return 2;
    else
        if ( buca[0][6] > buca[1][6] ) return 0;
        else return 1;
}

/**
 * @brief Tavolo::mosseValide
 * @param turno
 * @return vettore contenente le mosse possibili per il giocatore di turno
 */
QVector<int> Table::mosseValide(int turno) const
{
    QVector<int> mosse;

    for (int i = 0; i < 6; i++)
        if(buca[turno][i] != 0)
            mosse.append(i);
    return mosse;
}

QVector< QVector<int> > Table::get() const
{
    return buca;
}

/**
 * @brief Table::bucheVuote
 * @param turno
 * @return numero di buche vuote per il giocatore di turno
 */
int Table::bucheVuote(int turno) const
{
    return (numero_buche-1)-mosseValide(turno).size();
}

/**
 * @brief Table::differenzaPunti
 * @param turno
 * @return ritorna la differenza punti tra i giocatori
 * < 0 se il giocatore di turno è in svantaggio
 * = 0 se è in parità
 * > 0 se è in vantaggio
 */
int Table::differenzaPunti(int turno) const
{
    return buca[turno][numero_buche-1] - buca[abs(turno-1)][numero_buche-1];
}

int Table::avversario(int turno)
{
    return abs(turno-1);
}
