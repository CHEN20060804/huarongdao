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
    qDebug() << order;

    begin = board;
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
            qDebug() << board;
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
int GameLogicOne::getRows() const
{
    return rows;
}
QVector<QString> GameLogicOne::getBoard() const
{
    return board;
}

double GameLogicOne::computeDifficulty() const
{
    auto getManhattan = [&](int idx1, int idx2){
        int row1 = idx1 / cols;
        int col1 = idx1 % cols;
        int row2 = idx2 / cols;
        int col2 = idx2 % cols;
        return std::abs(row1 - row2) + std::abs(col1 - col2);
    };

    int total = rows * cols;
    double totalDistance = 0.0;
    int maxDistance = 0;

    for (int i = 0; i < total; ++i)
    {
        const QString& tile = begin[i];
        if (tile == "") continue;

        int targetIndex = -1;
        for (int j = 0; j < total; ++j)
        {
            if (target[j] == tile) {
                bool alreadyMatched = false;
                for (int k = 0; k < i; ++k) {
                    if (begin[k] == tile && getManhattan(k, j) == 0) {
                        alreadyMatched = true;
                        break;
                    }
                }
                if (!alreadyMatched) {
                    targetIndex = j;
                    break;
                }
            }
        }

        if (targetIndex != -1) {
            totalDistance += getManhattan(i, targetIndex);
        }
    }

    maxDistance = (rows + cols - 2) * (total - 1); // 除去空格

    double difficulty = totalDistance / maxDistance;
    return std::round(difficulty * 1000) / 1000.0; // 保留三位小数
}

void GameLogicOne::setOrder(const GameLogicOne& logic)
{
    board = logic.board;
}
