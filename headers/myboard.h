#ifndef MYBOARD_H
#define MYBOARD_H

#include "mysquare.h"
#include <QPushButton>
#include <QLCDNumber>
#include <QGridLayout>

#include <vector>

class myBoard : public QWidget
{
   Q_OBJECT

public:
    // Constructeurs
    myBoard(); //par defaut
    myBoard(unsigned int boardHeight, unsigned int boardBreadth, unsigned int totalNumberOfMines);

    // Destructeur
    ~myBoard();

    // Fcontions a utiliser a l'exterieur
    void mineMyBoard(void);
    void countMinedNeighbours(void);

    // Accesseurs aux membres prives
    unsigned int getCurrentNumberMines(void);
    void setCurrentNumberMines(unsigned int mines);

private:

    // Fonctions internes
    void loosingGame(void);
    void winningGameOrNot();

    // Hauteur et largeur du plateau
    unsigned int boardHeight;
    unsigned int boardBreadth;

    // Nombre total de mines sur le plateau
    unsigned int totalNumberMines;
    // Nombre de cases marquees comme minées à un instant donne
    int currentNumberMines;
    // Nombre de cases non ouvertes
    int currentClosedSquares;

    // Plateau de cases
    std::vector< std::vector<mySquare*> > squareMatrix;
    QGridLayout* boardLayout;

    // Indicateur de partie en cours
    bool isGameRunning;

signals :
    void startedGame(void);
    void markedOneSquare(bool checked);
    void finishedGame(bool won);

public slots :
    void openingOneSquare(unsigned int l, unsigned int c);
    void markingOneSquare(unsigned int l, unsigned int c, bool checked);

};

#endif // MYBOARD_H
