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

    QVector<QString> getBoard() const;

private:
    int rows, cols;
    QVector<QString> board;
    QVector<QString> target;
    int emptyIndex; // 空格的索引
};

#endif // GAMELOGICONE_H
