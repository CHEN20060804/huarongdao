#include "gamelogicone.h"
#include <random>
#include <numeric>
#include <QMap>
#include <QDebug>
GameLogicOne::GameLogicOne(int cols, int rows, const QVector<QString>& initBoard)
    : rows(rows), cols(cols), board(initBoard)
{
    target = initBoard; // 目标顺序与初始棋盘相同
}
// 检查拼图是否可解
bool GameLogicOne::isSolvable(const QVector<int>& order) const {
    Q_ASSERT(order.size() == rows * cols);
    Q_ASSERT(order.last() == (rows * cols - 1)); // 空格在最后

    int inv = 0;
    for (int i = 0; i < order.size() - 1; ++i) {
        for (int j = i + 1; j < order.size() - 1; ++j) {
            if (order[i] > order[j]) ++inv;
        }
    }

    if (cols % 2 == 1) {
        return inv % 2 == 0;
    } else {
        // 空格在最后，行数从底数起为 1
        return (inv + 1) % 2 == 0;
    }
}


// 打乱拼图
void GameLogicOne::shuffle() {
    QVector<int> order(rows * cols);
    std::iota(order.begin(), order.end(), 0); // 0~8

    std::random_device rd;
    std::mt19937 g(rd());

    do {
        std::shuffle(order.begin(), order.end() - 1, g); // 只打乱前 8 个
        order[order.size() - 1] = rows * cols - 1;       // 空格始终在最后（编号最大）
    } while (!isSolvable(order));

    for (int i = 0; i < order.size(); ++i)
        board[i] = target[order[i]];
}

bool GameLogicOne::tryMove(int i, int j)
{
    int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
    int currentIndex = i * cols + j;
    for (int d = 0; d < 4; ++d) {
        int ni = i + dx[d], nj = j + dy[d];
        int neighborIndex = ni * cols + nj;
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && board[neighborIndex] == "") {
            std::swap(board[currentIndex], board[neighborIndex]);
            return true;
        }
    }
    return false;
}

bool GameLogicOne::isSolved() const
{
    return board == target;
}

int GameLogicOne::getCols() const
{
    return cols;
}

QVector<QString> GameLogicOne::getBoard() const
{
    return board;
}
