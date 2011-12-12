#include "myboard.h"
#include "functions.h"
#include <QtDebug>

#include <QString>


/**
  * Definitions constructeurs et du destructeur
  *
  */
myBoard::myBoard(void) : QWidget(0)
{
    this->boardBreadth=0;
    this->boardHeight=0;
    this->totalNumberMines=0;
    this->isGameRunning=false;
    //this->squareMatrix= ???

    // Variables de gestion de partie
    this->currentNumberMines=this->totalNumberMines;
    this->currentClosedSquares=this->boardBreadth*this->boardHeight;
}

myBoard::myBoard(unsigned int height, unsigned int breadth, unsigned int mines) : QWidget(0)
{
    // Memorisation des parametres
    this->boardBreadth=breadth;
    this->boardHeight=height;
    this->totalNumberMines=mines;
    this->isGameRunning=false;

    // Variables de gestion de partie
    this->currentNumberMines=this->totalNumberMines;
    this->currentClosedSquares=this->boardBreadth*this->boardHeight;

    /* On rajoute un tour de cases qui ne feront pas partie du jeu.
    *  Elles ne contiendront pas de mines et ne seront pas visibles (non accessibles au joueur).
    *  Cela permet de simplifier les traitements en ne faisant pas de traitement particulier pour les cases du bord.
    *  Les boucles commenceront a 1 et iront à SIZE_OF_BOARD.
    *  Les cases d'indices 0 et SIZE_OF_BOARD+1 seront ces cases dummy.
    *
    *    ?????? Est ce une bonne idee ??????
    *
    * D'ailleurs, les boucles correspondantes sont commentees.
    */

    // Remplissage du vector de vector de cases
    //for(unsigned int l=0;l<=height+1;l++)
    for(unsigned int l=0;l<height;l++)
    {
        // Ligne courante
        std::vector<mySquare*> currentLine;

        // Construction de la ligne
        //for(unsigned int c=0;c<=breadth+1;c++)
        for(unsigned int c=0;c<breadth;c++)
        {
            mySquare* currentSquare = new mySquare(this,l,c);
            currentLine.push_back(currentSquare);
        }

        // Une fois la ligne construite, on la rajoute a la matrice
        squareMatrix.push_back(currentLine);
    }



    // Initialisation du layout d'affichage
    boardLayout = new QGridLayout;

    // Pour coller les cases
    //boardLayout->setSpacing(0);
    //boardLayout->setHorizontalSpacing(0);
    //boardLayout->setVerticalSpacing(0);

    // On associe le layout au widget plateau
    this->setLayout(boardLayout);

    // Affectation dans le layout des cases utiles (= sans le tour de dummy squares)
    //for(unsigned int l=1;l<=height;l++)
    for(unsigned int l=0;l<height;l++)
    {
        // Construction de la ligne
        //for(unsigned int c=1;c<=breadth;c++)
        for(unsigned int c=0;c<breadth;c++)
        {
            boardLayout->addWidget(squareMatrix[l][c],l,c);
        }
    }

}

myBoard::~myBoard(void)
{
    // TODO :Le vecteur contient des pointeurs sur objet, il faut les supprimer un par un
    // Voir : http://cpp.developpez.com/faq/cpp/index.php?page=STL#STL_vector_delete

    //delete[] squareMatrix;
}


/**
  * Accesseurs aux membres prives
  */
unsigned int myBoard::getCurrentNumberMines()
{
    return this->currentNumberMines;
}

void myBoard::setCurrentNumberMines(unsigned int mines)
{
    this->currentNumberMines=mines;
}

/*
myBoard::getBoardSize();
myBoard::setBoardSize();

myBoard::getTotalNumberMines();
myBoard::setTotalNumberMines();

myBoard::getboardLayout();
myBoard::setboardLayout();
*/


/**
  * Definitions des fonctions publiques de la classe
  *
  */

void myBoard::mineMyBoard(void)
{
    // Nombre de mines deja posees
    int dropedMines=0;

    // Premier passage pour miner le terrain
     for(unsigned int i=0;i<this->boardHeight;i++)
    {
        for(unsigned int j=0;j<this->boardBreadth;j++)
        {
            // Minage de facon aleatoire
            if( mineMe(this->boardHeight, this->boardBreadth, this->totalNumberMines) == 1
                    && dropedMines!=this->totalNumberMines )
            {
                // Le tirage au sort a decide que cette case sera minee
                this->squareMatrix[i][j]->setIsMined(true);
                dropedMines+=1;
            }
        }
    }

    // Si ce premier passage n'a pas suffit, on boucle a nouveau
    // mais on parcourt le plateau differemment pour renforcer le placement aleatoire
    while(dropedMines != this->totalNumberMines)
    {
         for(unsigned int j=0;j<this->boardBreadth;j++)
        {
            for(unsigned int i=0;i<this->boardHeight;i++)
            {
                // Minage de facon aleatoire les cases non minees
                if( false == this->squareMatrix[j][i]->getIsMined()
                        && dropedMines!=this->totalNumberMines)
                {
                    if( mineMe(this->boardHeight, this->boardBreadth, this->totalNumberMines) == 1 )
                    {
                        // Le tirage au sort a decide que cette case sera minee
                        this->squareMatrix[j][i]->setIsMined(true);
                        dropedMines+=1;
                    }
                }
            }
        }
    }
}//mineMyBoard


void myBoard::countMinedNeighbours(void)
{
    unsigned int mineCounter=0;

    // Comptage du centre du plateau
    for(unsigned int l=1;l<this->boardHeight-1;l++)
    {
        for(unsigned int c=1;c<this->boardBreadth-1;c++)
        {
            //qDebug() << "Iteration : l=" << l << " c=" << c;
            // Comptage : Ligne au dessus
            mineCounter  = squareMatrix[l-1][c-1]->getIsMined() ? 1 : 0;
            mineCounter += squareMatrix[l-1][c]->getIsMined() ? 1 : 0;
            mineCounter += squareMatrix[l-1][c+1]->getIsMined() ? 1 : 0;
            //Comptage : Meme ligne
            mineCounter += squareMatrix[l][c-1]->getIsMined() ? 1 : 0;
            mineCounter += squareMatrix[l][c+1]->getIsMined() ? 1 : 0;
            // Comptage: Ligne au dessous
            mineCounter += squareMatrix[l+1][c-1]->getIsMined() ? 1 : 0;
            mineCounter += squareMatrix[l+1][c]->getIsMined() ? 1 : 0;
            mineCounter += squareMatrix[l+1][c+1]->getIsMined() ? 1 : 0;

            // Memorisation dans la case courante
            squareMatrix[l][c]->setMinedNeighbours(mineCounter);
            //DEBUG
            //squareMatrix[l][c]->setText(QString::number(mineCounter));
        }
    }

    // Comptage pour le coin superieur gauche
    mineCounter  = squareMatrix[0][1]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[1][1]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[1][0]->getIsMined() ? 1 : 0;
    squareMatrix[0][0]->setMinedNeighbours(mineCounter);
    //DEBUG
    //squareMatrix[0][0]->setText(QString::number(mineCounter));

    // Comptage pour le coin superieur droit
    mineCounter  = squareMatrix[0][this->boardBreadth-2]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[1][this->boardBreadth-2]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[1][this->boardBreadth-1]->getIsMined() ? 1 : 0;
    squareMatrix[0][this->boardBreadth-1]->setMinedNeighbours(mineCounter);
    //DEBUG
    //squareMatrix[0][this->boardBreadth-1]->setText(QString::number(mineCounter));

    // Comptage pour le coin inferieur gauche
    mineCounter  = squareMatrix[this->boardHeight-2][0]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[this->boardHeight-2][1]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[this->boardHeight-1][1]->getIsMined() ? 1 : 0;
    squareMatrix[this->boardHeight-1][0]->setMinedNeighbours(mineCounter);
    //DEBUG
    //squareMatrix[this->boardHeight-1][0]->setText(QString::number(mineCounter));

    // Comptage pour le coin inferieur droit
    mineCounter  = squareMatrix[this->boardHeight-1][this->boardBreadth-2]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[this->boardHeight-2][this->boardBreadth-2]->getIsMined() ? 1 : 0;
    mineCounter += squareMatrix[this->boardHeight-2][this->boardBreadth-1]->getIsMined() ? 1 : 0;
    squareMatrix[this->boardHeight-1][this->boardBreadth-1]->setMinedNeighbours(mineCounter);
    //DEBUG
    //squareMatrix[this->boardHeight-1][this->boardBreadth-1]->setText(QString::number(mineCounter));

    // Comptage pour la ligne du haut (sans les coins)
    for(unsigned int c=1;c<this->boardBreadth-1;c++)
    {
        // Comptage : Meme ligne
        mineCounter  = squareMatrix[0][c-1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[0][c+1]->getIsMined() ? 1 : 0;
        // Comptage : Ligne au dessous
        mineCounter += squareMatrix[1][c-1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[1][c]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[1][c+1]->getIsMined() ? 1 : 0;

        // Memorisation dans la case courante
        squareMatrix[0][c]->setMinedNeighbours(mineCounter);
        //DEBUG
        //squareMatrix[0][c]->setText(QString::number(mineCounter));
    }

    // Comptage pour la ligne du bas (sans les coins)
    for(unsigned int c=1;c<this->boardBreadth-1;c++)
    {
        // Comptage : Meme ligne
        mineCounter  = squareMatrix[this->boardBreadth-2][c-1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[this->boardBreadth-2][c+1]->getIsMined() ? 1 : 0;
        // Comptage : Ligne au dessous
        mineCounter += squareMatrix[this->boardBreadth-1][c-1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[this->boardBreadth-1][c]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[this->boardBreadth-1][c+1]->getIsMined() ? 1 : 0;

        // Memorisation dans la case courante
        squareMatrix[this->boardBreadth-1][c]->setMinedNeighbours(mineCounter);
        //DEBUG
        //squareMatrix[this->boardBreadth-1][c]->setText(QString::number(mineCounter));
    }

    // Comptage pour la colonne de gauche (sans les coins)
    for(unsigned int l=1;l<this->boardHeight-1;l++)
    {
        // Comptage : Meme colonne
        mineCounter  = squareMatrix[l-1][0]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l+1][0]->getIsMined() ? 1 : 0;
        // Comptage : Colonne a droite
        mineCounter += squareMatrix[l-1][1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l][1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l+1][1]->getIsMined() ? 1 : 0;

        // Memorisation dans la case courante
        squareMatrix[l][0]->setMinedNeighbours(mineCounter);
        //DEBUG
        //squareMatrix[l][0]->setText(QString::number(mineCounter));
    }

    // Comptage pour la colonne de droite (sans les coins)
    for(unsigned int l=1;l<this->boardHeight-1;l++)
    {
        // Comptage : Meme colonne
        mineCounter  = squareMatrix[l-1][this->boardHeight-1]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l+1][this->boardHeight-1]->getIsMined() ? 1 : 0;
        // Comptage : Colonne a gauche
        mineCounter += squareMatrix[l-1][this->boardHeight-2]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l][this->boardHeight-2]->getIsMined() ? 1 : 0;
        mineCounter += squareMatrix[l+1][this->boardHeight-2]->getIsMined() ? 1 : 0;

        // Memorisation dans la case courante
        squareMatrix[l][this->boardHeight-1]->setMinedNeighbours(mineCounter);
        //DEBUG
        //squareMatrix[l][this->boardHeight-1]->setText(QString::number(mineCounter));
    }
}//countMinedNeighbours



/**
  * Definition des fonctions private de la classe
  *
  */

void myBoard::loosingGame(void)
{
    qDebug() <<"PERDU !!!";

    // La partie est terminee
    isGameRunning=false;

    // Desactivation de toutes les cases du plateau
    for(unsigned int l=0;l<boardHeight;l++)
    {
        for(unsigned int c=0;c<boardBreadth;c++)
        {
            squareMatrix[l][c]->setEnabled(false);
        }
    }

    // TODO : si des cases ont ete marquees comme minees a tord,
    // il faut les ouvrir pour que le joueur voient ses erreurs
}

void myBoard::winningGameOrNot()
{
    // Comparaison du nombre de cases avec le nombre de mines "trouvees"
    if(this->totalNumberMines==this->currentClosedSquares)
    {

        qDebug() << "GAGNE !!!";

        // La partie est terminee
        isGameRunning=false;

        // On informe qui voudra que la partie a ete gagnee
        emit finishedGame(true);

        // Desactivation de toutes les cases du plateau
        for(unsigned int l=0;l<boardHeight;l++)
        {
            for(unsigned int c=0;c<boardBreadth;c++)
            {
                squareMatrix[l][c]->setEnabled(false);
            }
        }
    }
}



/**
  * Definitions des slots personnalises
  *
  */

void myBoard::openingOneSquare(unsigned int l, unsigned int c)
{
    //qDebug() << "Execution du slot openingSquare de myBoard";
    //qDebug() <<  "l=" << l << ";c=" << c;

    // Premiere ouverture donc debut de la partie
    if(false==isGameRunning)
    {
        qDebug() << "Ouverture de la premiere case";
        isGameRunning=true;
        // On informe qui voudra que la partie a commence
        emit startedGame();
    }


    // Si ouverture d'une mine ==> partie perdue
    if(true==squareMatrix[l][c]->getIsMined())
    {
        // On termine la partie
        this->loosingGame();

        // On informe qui voudra que la partie a ete perdue
        emit finishedGame(false);

    }
    else // Sinon, la partie continue
    {
        // Ouverture de tous les voisins si la case ne touche aucune mine
        if(0 == squareMatrix[l][c]->getMinedNeighbours())
        {
            if(l>0  && l< boardHeight-1 && c< boardBreadth-1 && c>0)
            {
                // Traitement du centre
                // Ligne au dessus
                squareMatrix[l-1][c-1]->click();
                squareMatrix[l-1][c]->click();
                squareMatrix[l-1][c+1]->click();
                // Meme ligne
                squareMatrix[l][c-1]->click();
                squareMatrix[l][c+1]->click();
                // Ligne au dessous
                squareMatrix[l+1][c-1]->click();
                squareMatrix[l+1][c]->click();
                squareMatrix[l+1][c+1]->click();
            }
            else if(0==l && 0==c)
            {
                // Traitement du coin superieur gauche
                squareMatrix[0][1]->click();
                squareMatrix[1][1]->click();
                squareMatrix[1][0]->click();
            }
            else if(0==l && boardBreadth-1==c)
            {
                // Traitement du coin superieur droit
                squareMatrix[0][boardBreadth-2]->click();
                squareMatrix[1][boardBreadth-2]->click();
                squareMatrix[1][boardBreadth-1]->click();
            }
            else if(boardHeight-1==l && 0==c)
            {
                // Traitement du coin inferieur gauche
                squareMatrix[boardHeight-2][0]->click();
                squareMatrix[boardHeight-2][1]->click();
                squareMatrix[boardHeight-1][1]->click();
            }
            else if(boardHeight-1==l && boardBreadth-1==c)
            {
                // Traitement du coin inferieur droit
                squareMatrix[boardHeight-2][boardBreadth-1]->click();
                squareMatrix[boardHeight-2][boardBreadth-2]->click();
                squareMatrix[boardHeight-1][boardBreadth-2]->click();
            }
            else if(0==l)
            {
                // Traitement de la ligne superieure (sans les coins)
                // Meme ligne
                squareMatrix[0][c-1]->click();
                squareMatrix[0][c+1]->click();
                // Ligne au dessous
                squareMatrix[1][c-1]->click();
                squareMatrix[1][c]->click();
                squareMatrix[1][c+1]->click();
            }
            else if(boardHeight-1==l)
            {
                // Traitement de la ligne inferieure (sans les coins)
                // Meme ligne
                squareMatrix[boardHeight-1][c-1]->click();
                squareMatrix[boardHeight-1][c+1]->click();
                // Ligne au dessus
                squareMatrix[boardHeight-2][c-1]->click();
                squareMatrix[boardHeight-2][c]->click();
                squareMatrix[boardHeight-2][c+1]->click();
            }
            else if(0==c)
            {
                // Traitement de la colonne de gauche (sans les coins)
                // Meme colonne
                squareMatrix[l-1][c]->click();
                squareMatrix[l+1][c]->click();
                // Colonne a droite
                squareMatrix[l-1][c+1]->click();
                squareMatrix[l][c+1]->click();
                squareMatrix[l+1][c+1]->click();
            }
            else if(boardBreadth-1==c)
            {
                // Traitement de la colonne de droite (sans les coins)
                // Meme colonne
                squareMatrix[l-1][boardBreadth-1]->click();
                squareMatrix[l+1][boardBreadth-1]->click();
                // Colonne a gauche
                squareMatrix[l-1][boardBreadth-2]->click();
                squareMatrix[l][boardBreadth-2]->click();
                squareMatrix[l+1][boardBreadth-2]->click();
            }
        }//Ouverture des voisins

        // Comptage du nombre de cases ouvertes
        this->currentClosedSquares--;
        qDebug() << "Il reste " << this->currentClosedSquares << " case a ouvrir";

        // Fonction regardant si on a gagne
        this->winningGameOrNot();
    }
}//openingSquare


void myBoard::markingOneSquare(unsigned int l, unsigned int c, bool checked)
{
    // Si la case est "checked", on decompte
    if(true==checked)
    {
        this->currentNumberMines --;
        emit markedOneSquare(true);
    }
    else
    {
        this->currentNumberMines++;
        emit markedOneSquare(false);
    }

    qDebug() << "Il reste " << this->currentNumberMines << " mines a trouver";
}//markingOneSquare
