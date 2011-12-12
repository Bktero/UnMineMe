#include "mywindow.h"

#define NBR_MINES 15


/**
  * Constructeurs et destructeur
  *
  */
myWindow::myWindow(QWidget *parent) : QWidget(parent)
{
    // Creation du plateau de jeu
    myWindow::createBoard();

    // Initialisation pour les tirages aleatoires
    mineMeInit();

    // Creation du bouton et connexion au slot servant a commencer une nouvelle partie
    restartGameButton = new QPushButton("Lets (re)Mine...");
    QObject::connect( restartGameButton, SIGNAL(clicked()), this, SLOT(restartingGame()) );

    // Creation du LCD du nombre de mines
    LCDMines = new QLCDNumber(this);
    LCDMines->display( (int) localBoard->getCurrentNumberMines() );

    // Creation du chronometre du temps ecoule
    elapsedTimer = new QTimer(this);
    connect(elapsedTimer, SIGNAL(timeout()), this, SLOT(LCDTimeUpdate()));
    // Le chrono sera demarre par le slot LCDTimeStart

    // Creation du LCD affichant ce temps
    LCDTime = new QLCDNumber(this);
    LCDTime->display(0);

    // Mise en place dans le layout
    layout = new QGridLayout;
    layout->addWidget(LCDMines,1,1);
    layout->addWidget(restartGameButton,1,2);
    layout->addWidget(LCDTime,1,3);
    layout->addWidget(localBoard,2,1,1,3);
    this->setLayout(layout);
}

myWindow::~myWindow(void)
{
    // Rien de special a faire
}

/**
  * Definitions des fonctions de la classe
  *
  */
void myWindow::createBoard(void)
{
    // Creation de l'objet myBoard
    localBoard = new myBoard(10,10,NBR_MINES);

    // Minage du plateau de jeu
    localBoard->mineMyBoard();
    localBoard->countMinedNeighbours();

    // Connexion des signaux que le plateau peut emettre
    QObject::connect( localBoard, SIGNAL(finishedGame(bool)), this, SLOT(finishingGame(bool)) );
    QObject::connect( localBoard, SIGNAL(markedOneSquare(bool)), this, SLOT(countingMines(bool)) );
    QObject::connect( localBoard, SIGNAL(startedGame()), this, SLOT(LCDTimeStart()) );
}


/**
  * Definitions des slots personnalises
  *
  */
void myWindow::LCDTimeUpdate(void)
{
    elapsedTime+=1;
    qDebug() << "elapsedTime" << elapsedTime;
    LCDTime->display((int) elapsedTime);
}

void myWindow::LCDTimeStart(void)
{
    qDebug() << "LCDTimeStart";
    elapsedTimer->start(1000);
}

void myWindow::restartingGame(void)
{
    qDebug() << "Debut du slot myWindow::restartingGame";

    // On supprime le plateau actuel.
    delete localBoard;

    // On cree le nouveau plateau.
    myWindow::createBoard();
    // En supprimant l'objet, il a automatiquement ete enleve du layout. On le replace.
    layout->addWidget(localBoard,2,1,1,3);

    // Remettre le compteur de mines a sa valeur initiale
    LCDMines->display( (int) localBoard->getCurrentNumberMines() );

    // Remettre le temps affiche a zero
    elapsedTimer->stop();
    elapsedTime=0;
    LCDTime->display((int) elapsedTime);
    // Le chrono sera demarre par le slot LCDTimeStart

    // Remettre le bouton dans son etat initial
    restartGameButton->setText("Let (re)Mine");

    qDebug() << "Fin du slot myWindow::restartingGame";
}

void myWindow::finishingGame(bool won)
{
    qDebug() << "Debut du slot myWindow::finishingGame (" << won << ")";

    // Arreter le temps
    elapsedTimer->stop();

    // Afficher l'issue de la parti
    if(true==won)
    {
        restartGameButton->setText("Win !");
    }
    else
    {
        restartGameButton->setText("Epic Fail !");
    }

    qDebug() << "Fin du slot myWindow::finishingGame";
}

void myWindow::countingMines(bool checked)
{
    qDebug() << "Debut du slot myWindow::countingMines (" << checked << ")";

    if(true==checked)
    {
        // On decremente le compteur
        this->LCDMines->display(this->LCDMines->value()-1);
    }
    else
    {
        // On decremente le compteur
        this->LCDMines->display(this->LCDMines->value()+1);
    }

    qDebug() << "Fin du slot myWindow::countingMines";
}
