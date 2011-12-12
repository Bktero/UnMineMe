//
//  main.cpp
//  UnMineMe
//
//  Created by Pierre Gradot on 13/11/11.
//


#include <QApplication> 

#include "myboard.h"
#include "functions.h"

#include "mywindow.h"

int main(int argc, char *argv[])
{
    // Creation de l'application Qt
    QApplication app(argc, argv);
/*
    // Creation du plateau de jeux
    myBoard localBoard(10,10,15);

    // Initialisation pour les tirages aleatoires
    mineMeInit();

    // Minage du plateau de jeu
    localBoard.mineMyBoard();
    localBoard.countMinedNeighbours();

    // Titre et affichage du plateau
    localBoard.setWindowTitle("UnMineMe I'm Famous");
    localBoard.show();
*/
    myWindow maFenetre(0);
    maFenetre.show();

    // Demarrage de l'application
    return app.exec();
}
