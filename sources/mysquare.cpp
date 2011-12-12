#include "mysquare.h"
#include <QDebug>


// Constructeur par defaut
mySquare::mySquare(QWidget *parent) : QPushButton(parent)
{
    // Mise a zero des variables de classes
    this->lineNumber=0;
    this->columnNumber=0;
    this->minedNeighbours=0;
    this->isMined=false;
    this->isOpened=false;
    this->isMarked=false;

    // Visuel
    this->setFixedSize(SQUARE_DEFAULT_SIZE,SQUARE_DEFAULT_SIZE);

    // Association d'une action avec cette case
    // Le signal "clicked()" est herite de la classe mere
    // Le slot openingSquare est un slot personnalise
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(openingSquare()));
    QObject::connect(this, SIGNAL(rightClicked()), this, SLOT(markingSquare()));

    QObject::connect(this, SIGNAL(openedSquare(uint,uint)), this->parent(), SLOT(openingSquare(uint,uint)));
    QObject::connect(this, SIGNAL(markedSquare(uint,uint,bool)), this->parent(), SLOT(markingSquare(uint,uint,bool)));
}

// Constructeur a utiliser en regle generale
mySquare::mySquare(QWidget *parent, unsigned int line, unsigned int column) : QPushButton(parent)
{
    // Recuperation des arguments
    this->lineNumber=line;
    this->columnNumber=column;

    // Mise a une valeur par defaut des autres membres de la classe
    this->minedNeighbours=0;
    this->isMined=false;
    this->isOpened=false;
    this->isMarked=false;

    // Visuel
    this->setFixedSize(SQUARE_DEFAULT_SIZE,SQUARE_DEFAULT_SIZE);

    // Pour debug
    //this->setText(QString::number(line)+QString::number(column));

    // Association d'une action avec cette case
    // Le signal "clicked()" est herite de la classe mere
    // Le slot openingSquare est un slot personnalise
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(openingSquare()));
    QObject::connect(this, SIGNAL(rightClicked()), this, SLOT(markingSquare()));

    QObject::connect(this, SIGNAL(openedSquare(uint,uint)), this->parent(), SLOT(openingOneSquare(uint,uint)));
    QObject::connect(this, SIGNAL(markedSquare(uint,uint,bool)), this->parent(), SLOT(markingOneSquare(uint,uint,bool)));
}

// Constructeur plus evolue
mySquare::mySquare(QWidget *parent, unsigned int line, unsigned int column, bool mined) : QPushButton(parent)
{
    // Recuperation des arguments
    this->lineNumber=line;
    this->columnNumber=column;
    this->isMined=mined;

    // Mise a une valeur par defaut des autres membres de la classe
    this->minedNeighbours=0;
    this->isOpened=false;
    this->isMarked=false;

    // Visuel
    this->setFixedSize(SQUARE_DEFAULT_SIZE,SQUARE_DEFAULT_SIZE);

    // Pour debug
    //this->setText(QString::number(line)+QString::number(column));

    // Association d'une action avec cette case
    // Le signal "clicked()" est herite de la classe mere
    // Le slot openingSquare est un slot personnalise
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(openingSquare()));
    QObject::connect(this, SIGNAL(rightClicked()), this, SLOT(markingSquare()));

    QObject::connect(this, SIGNAL(openedSquare(uint,uint)), this->parent(), SLOT(openingOneSquare(uint,uint)));
    QObject::connect(this, SIGNAL(markedSquare(uint,uint,bool)), this->parent(), SLOT(markingOneSquare(uint,uint,bool)));
}

// Destructeur
mySquare::~mySquare(void)
{
    // Il n'y a rien a faire car le destructeur de la classe mere est appele automatiquement
    // Voir : http://cpp.developpez.com/faq/cpp/?page=destructeur#DESTRUCTEUR_explicite_base
}

// Accesseurs (avec des const car ils ne doivent rien modifier) et modificateurs pour les variables de classe
// columnNumber
unsigned int mySquare::getColumnNumber(void) const
{
    return this->lineNumber;
}
void mySquare::setColumnNumber(unsigned int column)
{
    this->columnNumber=column;
}
// lineNumber
unsigned int  mySquare::getLineNumber(void) const
{
    return this->columnNumber;
}
void mySquare::setLineNumber(unsigned int line)
{
    this->lineNumber=line;
}
// isMined
bool mySquare::getIsMined(void) const
{
    return this->isMined;
}
void mySquare::setIsMined(bool mined)
{
    this->isMined=mined;
}
// isOpened
bool mySquare::getIsOpened(void) const
{
    return this->isOpened;
}
void mySquare::setIsOpened(bool opened)
{
    this->isOpened=opened;
}

unsigned int mySquare::getMinedNeighbours(void) const
{
    return this->minedNeighbours;
}
void mySquare::setMinedNeighbours(unsigned int number)
{
    this->minedNeighbours=number;
}


// Methode pour afficher les infos sur une case
void mySquare::displaySquare(void)
{
    qDebug() << "Affichage d'une case";
    qDebug() << "Position : ligne = " << lineNumber << "; colonne = " << columnNumber;
    qDebug() << "Etat de la case : mine=" << isMined << "ouverte=" << isOpened;
    qDebug() << "Nombre de voisins mines = " << minedNeighbours;
    qDebug() << "--------------------------\n";
}




void mySquare::mousePressEvent(QMouseEvent* evt)
{
    // On teste le boutton de la souris et on emet le signal correspondant
    if(evt->button() == Qt::LeftButton)
    {
        //qDebug() << "Clic gauche";
        emit clicked();
    }
    else if(evt->button() == Qt::RightButton)
    {
        //qDebug() << "Clic droit";
        emit rightClicked();
    }
    else
    {
        qDebug() << "Autre action a la souris";
    }
}



// Definitions des slots personnalises
void mySquare::openingSquare(void)
{
    // Ouverture de la case
    if(false==this->isOpened)
    {
        // Si la case est fermee, on l'ouvre
        this->isOpened=true;

        // Quoi qu'il arrive, la case n'est plus marqee
        this->isMarked=false;

        // Estompe l'aspect "bouton" de la case
        //this->setFlat(true); // empeche laffichage des stylesheets /!\

        // La case est-elle minee ?
        if(true==this->isMined)
        {
            // On affiche un dessin de mine
            // TODO :dessin de mine
            // En attendant, un texte
            this->setText("(M)");
            this->setStyleSheet("background-color:red; color:white");
        }
        else
        {
            // On affiche le nombre de voisins mines
            this->setText(QString::number(this->minedNeighbours));

            // En fonctions de ce nombre, on change la couleur du texte
            if(0 == this->minedNeighbours)
            {
                this->setFlat(true);
                this->setText(NULL);
            }
            else if(1 == this->minedNeighbours)
            {
                this->setStyleSheet("color:green");
            }
            else if(2 == this->minedNeighbours)
            {
                this->setStyleSheet("color:blue");
            }
            else if(3 == this->minedNeighbours)
            {
                this->setStyleSheet("color:orange");
            }
            else if(4 == this->minedNeighbours)
            {
                this->setStyleSheet("color:red");
            }
            else
            {
                this->setStyleSheet("color:darkred");
            }
        }

        // Enfin, une fois qu'on a fait tous les traitements possibles sur la case
        // on en avertit le parent (probablement une instance de myBoard)
        emit openedSquare(this->lineNumber, this->columnNumber);
    }
    else
    {
        // Le joueur a clique sur une case deja ouverte
        // --> rien a faire
    }
}


void mySquare::markingSquare(void)
{
    if(true==this->isOpened)
    {
        qDebug() << "On ne peut pas marquer une case ouverte";
    }
    else
    {
        // Si non marquee, on marque
        if(false==isMarked)
        {
            this->setText("M");
            this->isMarked=true;
            emit markedSquare(this->lineNumber, this->columnNumber, true);
        }
        else //sinon, on demarque
        {
            this->setText(NULL);
            this->isMarked=false;
            emit markedSquare(this->lineNumber, this->columnNumber, false);
        }
    }
}
