#ifndef GAMELOGICONE_H
#define GAMELOGICONE_H

#include <QString>
#include <QVector>

class GameLogicOne
{
public:
    GameLogicOne(int cols, int rows, const QVector<QString>& initBoard);

    bool isSolvable(const QVector<int>&) const;
    void shuffle();
    bool tryMove(int i, int j);
    bool isSolved() const;
    int getCols() const;
    int getRows() const;
    void setOrder(const GameLogicOne& logic);
    QVector<QString> getBoard() const;
    QVector<QString> getTarget() const;
    double computeDifficulty() const;

private:
    int rows, cols;
    QVector<QString> begin;
    QVector<QString> board;
    QVector<QString> target;
};

#endif // GAMELOGICONE_H
