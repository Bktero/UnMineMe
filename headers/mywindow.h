#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>

#include "myboard.h"
#include "functions.h"

#include <QDebug>


class myWindow : public QWidget
{
    Q_OBJECT
public:
    explicit myWindow(QWidget *parent = 0);
    ~myWindow(void);
    void createBoard(void);

private:
    myBoard *localBoard;
    QGridLayout *layout;
    QPushButton *restartGameButton;
    QLCDNumber *LCDMines;

    QLCDNumber *LCDTime;
    QTimer *elapsedTimer;
    unsigned int elapsedTime;

signals:
    void restartedGame(void);

public slots:
    void LCDTimeUpdate(void);
    void LCDTimeStart(void);
    void restartingGame(void);
    void finishingGame(bool won);
    void countingMines(bool checked);



};

#endif // MYWINDOW_H
