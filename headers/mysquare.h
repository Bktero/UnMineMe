#ifndef MYSQUARE_H
#define MYSQUARE_H

#include <QPushButton>
#include <QMouseEvent>
#include <iostream>

#define SQUARE_DEFAULT_SIZE 30 //en pixels

class mySquare : public QPushButton
{
    Q_OBJECT // this "macro is mandatory for any object that implements signals, slots or properties"

public:
    // Constructeurs
    explicit mySquare(QWidget *parent=0);
    explicit mySquare(QWidget *parent, unsigned int line, unsigned int column);
    explicit mySquare(QWidget *parent, unsigned int line, unsigned int column, bool mined);
    // Destructeur
    ~mySquare(void);

    // Accesseurs et modificateurs
    unsigned int getColumnNumber(void) const;
    void setColumnNumber(unsigned int column);

    bool getIsMined(void) const;
    void setIsMined(bool);

    bool getIsOpened(void) const;
    void setIsOpened(bool);

    unsigned int getLineNumber(void) const;
    void setLineNumber(unsigned int line);

    unsigned int getMinedNeighbours(void) const;
    void setMinedNeighbours(unsigned int number);

    // Methode pour afficher les infos sur une case
    //void operator<< (void);
    void displaySquare(void);

private:

    // Redefinition pour gerer le clic-droit
    void mousePressEvent(QMouseEvent* evt);

    // Positions de la case
    // Informations pertinentes ?
    unsigned int lineNumber;
    unsigned int columnNumber;

    // Etat de la case
    bool isOpened;
    bool isMined;
    bool isMarked;

    // Information sur les voisins (nombre a afficher en cas d'ouverture non explosive)
    unsigned int minedNeighbours;

signals:
    void openedSquare(unsigned int l, unsigned int c);
    void markedSquare(unsigned int l, unsigned int c, bool checked);
    void rightClicked(void);

public slots:
    void openingSquare(void);
    void markingSquare(void);


};

#endif // MYSQUARE_H
