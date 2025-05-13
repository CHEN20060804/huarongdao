#ifndef GAMEPAGEWITHAI_H
#define GAMEPAGEWITHAI_H

#include <QWidget>
#include "slidingsidebar.h"
#include "tilebutton.h"
#include "gamelogicone.h"
#include "leveldata.h"
#include "gamesessiondata.h"
#include <memory>
#include <QTextBrowser>
#include <QVector>
#include <QSet>
#include <QPointer>
#include <QTimer>
#include <QPair>
#include <queue>
#include "man.h"

class GamePageWithAI;

namespace Ui {
class GamePageWithAI;
}


class Player : public Man {
    Q_OBJECT
    friend class GamePageWithAI;
public:
    explicit Player(QObject* parent = nullptr) : Man(parent) {}

private:
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;
};
class AI : public Man {
    Q_OBJECT
    friend class GamePageWithAI;

public:
    explicit AI(QObject* parent = nullptr) : Man(parent) {}

private:
    // override
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;

    // --- 数据成员 ---
    int width;                                       // 棋盘宽度
    QVector<QString> targetBoard;                    // 目标状态一维数组
    QTimer timer;                                    // 定时器控制动画速度

    struct State {
        QVector<QString> board;                      // 当前局面
        int g;                                       // 已走步数
        int h;                                       // 启发值
        QVector<QPair<int, int>> path;              // 移动路径
    };

    QSet<QVector<QString>> visited;                 // 当前 DFS 路径中的判重（可用于剪枝）

    int threshold;                                   // 当前 f(n) 阈值
    bool solved = false;                             // 是否已找到解
    QVector<QPair<int, int>> solutionPath;           // 最终解路径（用于动画播放）

    // --- 成员函数 ---
    int heuristic(const QVector<QString>& board);    // 启发函数 h(n)
    int dfs(State& state, int bound);                // 递归搜索函数，返回新阈值或成功标志
    void startSolving();                             // 启动 IDA* 搜索流程
    void solveStep();                                // 用 timer 动画播放 solutionPath
    QVector<State> generateNeighbors(const State& s);// 生成邻居状态
};

class GamePageWithAI : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageWithAI(QWidget *parent = nullptr);
    void loadLevel(const Level& level);
    ~GamePageWithAI();
signals:
    void mainBtnClicked(int i);
private:

    Ui::GamePageWithAI *ui;


    QTextBrowser* targetText;
    Level level;
    Player* player;
    AI* ai;
    QWidget* playerWidget;
    QWidget* AIWidget;

    void winEffect(Man* man);
    void updateUI(Man* man);
    void createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry = {290, 20, 270, 165});
};

#endif // GAMEPAGEWITHAI_H
