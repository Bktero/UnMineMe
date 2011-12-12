#include <QTime>
//#include <QDebug>

void mineMeInit(void)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}


int mineMe(unsigned int boardHeigh, unsigned int boardWidth, unsigned int numberOfMines)
{
    int proba=-1;
    int result=0;

    // Tirage aleatoire
    proba = qrand() % boardHeigh*boardWidth/numberOfMines;

    // Si le tirage est concluant, le resultat vaut 1
    if(proba==0)
    {
        result=1;
    }

    //qDebug() << "MineMe = " << result;
    return result;
}
