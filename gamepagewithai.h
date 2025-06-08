#ifndef GAMEPAGEWITHAI_H
#define GAMEPAGEWITHAI_H

#include <QWidget>
#include "leveldata.h"
#include <QTextBrowser>
#include <QVector>
#include <QSet>
#include <QPointer>
#include <functional>
#include <QTimer>
#include <QPair>
#include <queue>
#include "man.h"
#include "difficultylabel.h"

class GamePageWithAI;

namespace Ui {
class GamePageWithAI;
}


class Player : public Man {
    Q_OBJECT
    friend class GamePageWithAI;
public:
    explicit Player(QObject* parent = nullptr) : Man(parent) {}
signals:
    void playerMoved();
private:
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;
};

class AI : public Man {
    Q_OBJECT
    friend class GamePageWithAI;

public:
    explicit AI(QObject* parent = nullptr);
    void detachHeavyData();
    void RELEASE();
    ~AI();

signals:
    void solveReady(); // 当 A* 求解完成时发出信号
    void move();
    void thought();

private:
    // 初始化棋盘，读取关卡和目标状态
    void initBoard(const Level& level, QWidget* parentWidget) override;

    // 核心状态结构体
    struct State {
        QVector<QString> board;
        int g = 0;                           // 已走步数
        int h = 0;
        int emptyPos = -1;                   // 空格位置
        QVector<QPair<int, int>> path;       // 移动轨迹
        quint64 code = 0;                    // 状态哈希值4
        int manhattan = 0;
        int linearConflict = 0;


        bool operator>(const State& other) const {
            return (g + h*1.5) > (other.g + other.h*1.5);//给h呈上权重
        }
    };

    // 核心数据
    int width = 0;
    QVector<QString> targetBoard;                   // 目标状态
    QVector<QPair<int, int>> solutionPath;          // 求解路径
    QMultiHash<QString,int> targetMap;
    QVector<int> targetRow;            // 索引 → 目标行
    QVector<int> targetCol;            // 索引 → 目标列



    bool solved = false;
    bool isExit = false;

 //   QTimer* timer;                                  // 控制动画

    // A* 用到的容器
    QHash<quint64, int> openG;                      // 状态哈希 -> 最小 g 值
    QHash<quint64, int> closedG;                    // 拓展过的状态
    std::priority_queue<State, std::vector<State>, std::greater<State>> openPQ;

    // 函数
    void startSolving();                            // 启动 A*
    void startAnimation();                          // 启动动画播放
    void solveStep();                               // 一步动画（定时器）

    int calculateRowConflict(const QVector<QString>& board, int row);
    int calculateColConflict(const QVector<QString>& board, int row);

    void scheduleNextStep();
    int intervalTime;

    int heuristic(const QVector<QString>& board);   // 启发函数
    int calculateLinearConflict(const QVector<QString>& board);
    QVector<State> generateNeighbors(const State& current); // 生成可移动邻居
    quint64 hashBoard(const QVector<QString>& board) const; // 哈希函数


    // 覆盖虚函数（空实现）
    void tryMove(int i, int j) override {}

    // 工具
    int toIndex(int row, int col) const { return row * width + col; }
    QPair<int, int> toCoord(int idx) const { return { idx / width, idx % width }; }
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
    void AIcanMove();
private:

    Ui::GamePageWithAI *ui;


    QTextBrowser* targetText;
    Level level;
    Player* player;
    AI* ai;
    QWidget* playerWidget;
    QWidget* AIWidget;
    DifficultyLabel* difficultyBar;
    double hardness;

    bool aiReady = false;
    bool allowAImove = false;
    QElapsedTimer timer;

    void winEffect(Man* man);
    void updateUI(Man* man);
    void createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry = {290, 20, 270, 165});
};

#endif // GAMEPAGEWITHAI_H
