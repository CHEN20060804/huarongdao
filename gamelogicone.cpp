#include "gamelogicone.h"
#include <random>
#include <numeric>
#include <QMap>

GameLogicOne::GameLogicOne(int cols, int rows, const QVector<QString>& initBoard)
    : rows(rows), cols(cols), board(initBoard)
{
    target = initBoard; // 目标顺序与初始棋盘相同
}

bool GameLogicOne::isSolvable(const QVector<QString>& flat) const {
    // 创建目标顺序的映射表
    QMap<QString, int> targetIndexMap;
    for (int i = 0; i < target.size(); ++i) {
        targetIndexMap[target[i]] = i;
    }

    // 计算逆序数，使用自定义目标顺序的映射
    int inv = 0;
    for (int i = 0; i < flat.size(); ++i) {
        for (int j = i + 1; j < flat.size(); ++j) {
            if (flat[i] != "" && flat[j] != "" && targetIndexMap[flat[i]] > targetIndexMap[flat[j]]) {
                ++inv;
            }
        }
    }

    // 计算空格的索引并确定空格所在的行（从底数起）
    int emptyIndex = flat.indexOf("");
    int emptyRowFromBottom = rows - (emptyIndex / cols) - 1;

    if (cols % 2 == 1) {
        // 对于奇数列，逆序数必须是偶数
        return inv % 2 == 0;
    } else {
        // 对于偶数列，逆序数 + 空格的行数必须是偶数
        return (inv + emptyRowFromBottom) % 2 == 0;
    }
}

void GameLogicOne::shuffle() {
    QVector<QString> flat = target; // n^2 个字符
    flat[flat.size() - 1] = ""; // 空格固定在最后

    QVector<QString> toShuffle = flat.mid(0, flat.size() - 1);

    std::random_device rd;
    std::mt19937 g(rd());

    do {
        std::shuffle(toShuffle.begin(), toShuffle.end(), g);
    } while (!isSolvable(toShuffle));

    // 构造最终 flat
    for (int i = 0; i < toShuffle.size(); ++i)
        flat[i] = toShuffle[i];
    flat[flat.size() - 1] = ""; // 确保空格仍在最后

    board = flat;
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

QVector<QString> GameLogicOne::getBoard() const
{
    return board;
}
