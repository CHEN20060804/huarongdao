#include "gamepagewithai.h"
#include "ui_gamepagewithai.h"
#include "mainbutton.h"
#include <QFrame>
#include <QtMath>
#include <climits>
#include <QtConcurrent>
#include "settingmanager.h"
#include <QtAlgorithms>

GamePageWithAI::GamePageWithAI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageWithAI)
    , player(nullptr)
    , ai(nullptr)
    ,playerWidget(nullptr)
    ,AIWidget(nullptr)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));
    connect(backBtn, &QPushButton::clicked, [=]{
        //resetRecord();
        ai->timer->stop();
        ai->isExit = true;
        emit mainBtnClicked(2);
        //delete ai;
        ai->deleteLater();
        ai = nullptr;
    });

    MainButton* reloadBtn = new MainButton(this, 50, 50);
    reloadBtn->move(150, 30);
    reloadBtn->setIcon(QIcon(":/video/res/reload.png"));
    reloadBtn->setIconSize(QSize(30, 30));

    connect(reloadBtn, &QPushButton::clicked, [=](){
        ai->timer->stop();
        ai->isExit = true;
        loadLevel(level);
    });

    QFrame* dividerLine = new QFrame(this);
    dividerLine->setFrameShape(QFrame::VLine);
    dividerLine->setFrameShadow(QFrame::Sunken);     // 凹陷视觉
    dividerLine->setLineWidth(2);                    // 线宽
    dividerLine->setMidLineWidth(1);
    dividerLine->setGeometry(850 / 2 - 1, 186, 2, 500);
    dividerLine->show();
}
void GamePageWithAI::updateUI(Man* man) {
    QVector<TileButton*> tiles = man->getTiles();
    QVector<QString> board = player->logic->getBoard();

    for (int i = 0; i < board.size(); ++i) {
        if (TileButton* btn = tiles[i]) {
            QString val = board[i];
            if (val.trimmed().isEmpty()) {
                btn->hide();  // 完全隐藏
            } else {
                btn->setText(val);
                btn->setEnabled(true);
                btn->show();  // 显示
            }
        }
    }
}

void Player::tryMove(int i, int j) {
    QVector<QString> oldBoard = logic->getBoard(); // 保存旧状态

    if (logic->tryMove(i, j)) {
        //updateStepdisplay();
        //if(currentSteps=="1")startRecord(level);
        QVector<QString> newBoard = logic->getBoard();

        emit playerMoved();
        // 查找移动的方块
        int fromIndex = -1, toIndex = -1;
        for (int i = 0; i < oldBoard.size(); ++i) {
            if (!oldBoard[i].isEmpty() && newBoard[i].isEmpty())
                fromIndex = i;
            if (oldBoard[i].isEmpty() && !newBoard[i].isEmpty())
                toIndex = i;
        }

        if (fromIndex != -1 && toIndex != -1) {
            TileButton* movedBtn = tiles[fromIndex];

            // 计算目标位置
            int toRow = toIndex / logic->getCols();
            int toCol = toIndex % logic->getCols();
            QPoint targetPos(
                toCol * buttonWidth ,
                toRow * buttonHeight
                );

            // 创建动画
            QPropertyAnimation* anim = new QPropertyAnimation(movedBtn, "pos");
            anim->setDuration(200);
            anim->setStartValue(movedBtn->pos());
            anim->setEndValue(targetPos);
            anim->start(QAbstractAnimation::DeleteWhenStopped);

            // 更新按钮位置信息
            movedBtn->setRowCol(toRow, toCol);
            std::swap(tiles[fromIndex], tiles[toIndex]);

            QTimer::singleShot(160, this, [=](){//稍微延迟，碰撞音效
                movesound->play();
            });
        }

        if (logic->isSolved()) {
              //stopRecord();
            winsound->play();
            emit over(this);
        }
    }
}

// void AI::tryMove(int i, int j) {
//     QVector<QString> oldBoard = logic->getBoard(); // 保存旧状态

//     if (logic->tryMove(i, j)) {
//         //updateStepdisplay();
//         //if(currentSteps=="1")startRecord(level);
//         QVector<QString> newBoard = logic->getBoard();

//         // 查找移动的方块
//         int fromIndex = -1, toIndex = -1;
//         for (int i = 0; i < oldBoard.size(); ++i) {
//             if (!oldBoard[i].isEmpty() && newBoard[i].isEmpty())
//                 fromIndex = i;
//             if (oldBoard[i].isEmpty() && !newBoard[i].isEmpty())
//                 toIndex = i;
//         }

//         if (fromIndex != -1 && toIndex != -1) {
//             TileButton* movedBtn = tiles[fromIndex];

//             // 计算目标位置
//             int toRow = toIndex / logic->getCols();
//             int toCol = toIndex % logic->getCols();
//             QPoint targetPos(
//                 toCol * buttonWidth ,
//                 toRow * buttonHeight
//                 );

//             // 创建动画
//             QPropertyAnimation* anim = new QPropertyAnimation(movedBtn, "pos");
//             anim->setDuration(200);
//             anim->setStartValue(movedBtn->pos());
//             anim->setEndValue(targetPos);
//             anim->start(QAbstractAnimation::DeleteWhenStopped);

//             // 更新按钮位置信息
//             movedBtn->setRowCol(toRow, toCol);
//             std::swap(tiles[fromIndex], tiles[toIndex]);
//         }

//         if (logic->isSolved()) {
//             //stopRecord();
//             emit over(this);

//         }
//     }
// }

void GamePageWithAI::loadLevel(const Level& level) {
    this->level = level;

    // 显示目标字符
    createCustomTargetDisplay(this, level.getElement());


    // 清理旧控件和对象
    if (playerWidget) {
        playerWidget->deleteLater();
        playerWidget = nullptr;
    }
    if (AIWidget) {
        AIWidget->deleteLater();
        AIWidget = nullptr;
    }

    if (player) {
        player->deleteLater();
        player = nullptr;
    }

    if (ai) {
        qDebug() <<"n";
        delete ai;
        //ai->deleteLater();
        qDebug() <<"m";

        ai = nullptr;
    }

    // 创建 playerWidget
    playerWidget = new QWidget(this);
    playerWidget->setObjectName("playerFrame");
    playerWidget->setStyleSheet(R"(
        #playerFrame {
            background: transparent;
            border: 10px solid rgba(0, 255, 0, 50);
        }
    )");
    playerWidget->move(50, 200);  // 左边

    // 创建 aiWidget
    AIWidget = new QWidget(this);
    AIWidget->setObjectName("aiFrame");
    AIWidget->setStyleSheet(R"(
        #aiFrame {
            background: transparent;
            border: 10px solid rgba(255, 0, 0, 50);
        }
    )");

    qDebug() <<"0";

    // 创建新对象
    player = new Player(this);
    ai = new AI(this);

    // 初始化棋盘
    player->initBoard(level, playerWidget);
    ai->initBoard(level, AIWidget);
    player->logic->shuffle();
    ai->logic->setOrder(*player->logic);
    // 调整 aiWidget 位置
    AIWidget->adjustSize();
    int x = this->width() - 50 - AIWidget->width();
    AIWidget->move(x, 200);
    qDebug() <<"2";
    aiReady = false;
    playerMoved = false;

    QtConcurrent::run([&,this]() {
        ai->startSolving();
    });

    connect(player, &Player::playerMoved, player, [&,this]() {

        playerMoved = true;
        if (aiReady)
        {
            qDebug() <<"start1";

            ai->startAnimation();

            qDebug() <<"start3";

        }
    }, Qt::SingleShotConnection);

    connect(ai, &AI::solveReady, ai, [&,this]() {

        aiReady = true;
        if (playerMoved)
        {
            qDebug() <<"start2";

            ai->startAnimation();
        }
    }, Qt::SingleShotConnection);


    qDebug() <<"3";

    // 绑定胜利信号
    connect(player, &Player::over, this, &GamePageWithAI::winEffect);
    connect(ai, &AI::over, this, &GamePageWithAI::winEffect);

    // 更新 UI
    updateUI(player);
    qDebug() << ai->tiles.size();
    updateUI(ai);
    qDebug() <<"7";

    playerWidget->show();
    AIWidget->show();
}



void Player::initBoard(const Level& level, QWidget* parent) {
    int cols = level.getw();
    int rows = level.geth();

    double val = SettingManager::getInstance()->getSoundVolume()/100.0;
    movesound->setVolume(val);
    winsound->setVolume(val);

    logic = std::make_unique<GameLogicOne>(cols, rows, level.getElement());

    // 清空旧控件
    if (parent->layout()) {
        QLayoutItem* item;
        while ((item = parent->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete parent->layout();
    }

    auto *outerLayout = new QVBoxLayout(parent);
    outerLayout->setContentsMargins(10, 10, 10, 10);
    outerLayout->setSpacing(0);

    QWidget *boardBg = new QWidget(parent);
    boardBg->setObjectName("boardBg");
    boardBg->setStyleSheet("background-color: lightgray;");
    outerLayout->addWidget(boardBg);

    auto *grid = new QGridLayout(boardBg);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    buttonWidth  = 70 - (cols - 3) * 10;
    buttonHeight = 70 - (rows - 3) * 10;
    tiles.resize(rows * cols);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            auto *btn = new TileButton(boardBg, buttonWidth, buttonHeight);
            grid->addWidget(btn, row, col);
            btn->setRowCol(row, col);
            connect(btn, &TileButton::clicked, parent, [this, btn]() {
                tryMove(btn->row(), btn->col());
            });
            tiles[index] = btn;
        }
    }
}
void AI::initBoard(const Level& level, QWidget* parent) {
    int cols = level.getw();
    int rows = level.geth();

    isExit = false;

    double val = SettingManager::getInstance()->getSoundVolume()/100.0;
    movesound->setVolume(val);
    winsound->setVolume(val);

    logic = std::make_unique<GameLogicOne>(cols, rows, level.getElement());
    targetBoard = logic->getTarget();
    width = logic->getCols();

    int size = targetBoard.size();

    // 1. 构建 targetMap: 把每个 tile (QString) 映射到它在 targetBoard 中的唯一索引
    targetMap.clear();
    for (int i = 0; i < size; ++i) {
        targetMap.insert(targetBoard[i], i);
    }

    // 2. 缓存每个索引的“目标行/列”以便后续快速查询
    targetRow.resize(size);
    targetCol.resize(size);
    for (int idx = 0; idx < size; ++idx) {
        targetRow[idx] = idx / width;
        targetCol[idx] = idx % width;
    }


    // 清空旧控件
    if (parent->layout()) {
        QLayoutItem* item;
        while ((item = parent->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete parent->layout();
    }

    auto *outerLayout = new QVBoxLayout(parent);
    outerLayout->setContentsMargins(10, 10, 10, 10);
    outerLayout->setSpacing(0);

    QWidget *boardBg = new QWidget(parent);
    boardBg->setObjectName("boardBg");
    boardBg->setStyleSheet("background-color: lightgray;");
    outerLayout->addWidget(boardBg);

    auto *grid = new QGridLayout(boardBg);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    buttonWidth  = 70 - (cols - 3) * 10;
    buttonHeight = 70 - (rows - 3) * 10;
    tiles.resize(rows * cols);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            auto *btn = new TileButton(boardBg, buttonWidth, buttonHeight);
            grid->addWidget(btn, row, col);
            btn->setRowCol(row, col);
            connect(btn, &TileButton::clicked, parent, [this, btn]() {
                tryMove(btn->row(), btn->col());
            });
            tiles[index] = btn;
        }
    }
}

void GamePageWithAI::winEffect(Man* man)
{
    QVector<TileButton*> tiles;
    QWidget* widget;
    if(dynamic_cast<Player*>(man))
    {
        tiles = player->tiles;
        widget = playerWidget;
    }
    else
    {
        tiles = ai->tiles;
        widget = AIWidget;
    }
    for(int i = 0;i< tiles.size();i++)
    {
        tiles[i]->setEnabled(false);
    }

    aiReady = false;
    playerMoved = false;

    auto* glowEffect = new QGraphicsDropShadowEffect(this);
    glowEffect->setColor(QColor(255, 215, 0)); // 金黄色
    glowEffect->setOffset(0, 0);               // 无偏移，仅环绕边缘
    glowEffect->setBlurRadius(60);             // 柔和模糊程度
    widget->setGraphicsEffect(glowEffect);

    // 创建动画：改变 glowEffect 的透明度（通过 color alpha）
    auto* anim = new QPropertyAnimation(glowEffect, "color");
    anim->setDuration(2000); // 一个周期2秒
    anim->setLoopCount(-1);  // 无限循环

    // 设置颜色变化：从浅金 → 深金 → 浅金
    anim->setKeyValueAt(0.0, QColor(255, 215, 0, 80));   // 开始：淡金
    anim->setKeyValueAt(0.5, QColor(255, 215, 0, 255));  // 中间：亮金
    anim->setKeyValueAt(1.0, QColor(255, 215, 0, 80));   // 结束：淡金

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void GamePageWithAI::createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry)
{

    targetText = new QTextBrowser(parent);
    targetText->setGeometry(geometry);

    // 设置简洁适配的半透明浅黄色背景
    QPalette palette = targetText->palette();
    palette.setColor(QPalette::Base, QColor(241, 245, 202)); // 浅黄色 (LightYellow)，透明度180
    palette.setColor(QPalette::Text, QColor(0, 0, 0));             // 黑色文字
    targetText->setPalette(palette);

    // 设置字体和样式
    targetText->setStyleSheet(R"(
        QTextBrowser {
            border: 1px solid white;
            border-radius: 8px;
            padding: 8px;
            font-family: "楷体";
            font-size: 18px;
            background-clip: padding;
        }
    )");

    // 拼接内容 —— 每个目标之间空一格
    QString content = "<div align=\"center\"><b>目标顺序：</b></div><br>";
    for (const QString& item : targetList) {
        content += item + " ";
    }

    targetText->setText(content.trimmed());

    targetText->setReadOnly(true);
    targetText->show();
}

AI::AI(QObject* parent) : Man(parent), timer(new QTimer(this)) {}

AI::~AI() {
    qDebug() << "[AI] 析构中";
    if (timer->isActive()) {
        qDebug() << "[AI] Timer 活跃，尝试 stop";
        timer->stop();
    }
    qDebug() << "[AI] 尝试 disconnect timer";
    timer->disconnect();
    qDebug() << "[AI] 析构结束";
}

int AI::calculateRowConflict(const QVector<QString>& board, int row) {
    int conflict = 0;
    int baseIdx = row * width;
    int height  = board.size() / width;
    QVector<bool> used(targetBoard.size(), false);
    QVector<QPair<int,int>> tiles;
    tiles.reserve(width);

    for (int col = 0; col < width; ++col) {
        int idx = baseIdx + col;
        const QString& val = board[idx];
        if (val.isEmpty()) continue;

        // 候选目标索引列表
        QList<int> cands = targetMap.values(val);
        int chosen = -1;
        for (int tIdx : cands) {
            if (used[tIdx]) continue;
            if (tIdx / width != row) continue;
            chosen = tIdx;
            break;
        }
        if (chosen >= 0) {
            used[chosen] = true;
            int targetCol = chosen % width;
            tiles.append({col, targetCol});
        }
    }

    int m = tiles.size();
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if ((tiles[i].first < tiles[j].first) !=
                (tiles[i].second < tiles[j].second)) {
                conflict += 2;
            }
        }
    }
    return conflict;
}

int AI::calculateColConflict(const QVector<QString>& board, int col) {
    int conflict = 0;
    int height = board.size() / width;
    QVector<bool> used(targetBoard.size(), false);
    QVector<QPair<int,int>> tiles;
    tiles.reserve(height);

    for (int row = 0; row < height; ++row) {
        int idx = row * width + col;
        const QString& val = board[idx];
        if (val.isEmpty()) continue;

        QList<int> cands = targetMap.values(val);
        int chosen = -1;
        for (int tIdx : cands) {
            if (used[tIdx]) continue;
            if (tIdx % width != col) continue;
            chosen = tIdx;
            break;
        }
        if (chosen >= 0) {
            used[chosen] = true;
            int targetRowIdx = chosen / width;
            tiles.append({ row, targetRowIdx });
        }
    }

    int m = tiles.size();
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if ((tiles[i].first < tiles[j].first) !=
                (tiles[i].second < tiles[j].second)) {
                conflict += 2;
            }
        }
    }
    return conflict;
}

int AI::calculateLinearConflict(const QVector<QString>& board) {
    int total = 0;
    int height = board.size() / width;
    for (int r = 0; r < height; ++r) {
        total += calculateRowConflict(board, r);
    }
    for (int c = 0; c < width; ++c) {
        total += calculateColConflict(board, c);
    }
    return total;
}

// int AI::heuristic(const QVector<QString>& board) {
//     int size = board.size();
//     int manhattanSum = 0;

//     // 1. 计算全盘曼哈顿距离之和
//     for (int i = 0; i < size; ++i) {
//         const QString& val = board[i];
//         if (val.isEmpty()) continue;  // 空格不算
//         int tIdx = targetMap.value(val);       // 目标索引
//         int xi = i / width, yi = i % width;     // 当前坐标
//         int xt = targetRow[tIdx], yt = targetCol[tIdx]; // 目标坐标
//         manhattanSum += std::abs(xi - xt) + std::abs(yi - yt);
//     }

//     // 2. 计算线性冲突惩罚
//     int linear = calculateLinearConflict(board);

//     return manhattanSum + linear;
// }

quint64 AI::hashBoard(const QVector<QString>& board) const {
    quint64 hash = 0xcbf29ce484222325ULL; // 64位 FNV 初始值
    for (const QString& tile : board) {
        quint64 hTile = static_cast<quint64>(qHash(tile));
        hash ^= hTile + 0x9e3779b97f4a7c15ULL + (hash << 12) + (hash >> 4);
    }
    return hash;
}

QVector<AI::State> AI::generateNeighbors(const State& current) {
    QVector<State> neighbors;
    int row = current.emptyPos / width;
    int col = current.emptyPos % width;
    const QVector<QPair<int,int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};

    // 只拷贝一次
    QVector<QString> boardCopy = current.board;

    for (int dir = 0; dir < 4; ++dir) {
        int dr = directions[dir].first;
        int dc = directions[dir].second;
        int newRow = row + dr;
        int newCol = col + dc;
        if (newRow < 0 || newRow >= boardCopy.size()/width || newCol < 0 || newCol >= width)
            continue;
        int newIndex = newRow * width + newCol;

        // —— 1. 增量计算曼哈顿 ——
        int newManhattan = current.manhattan;
        const QString& movedVal = boardCopy[newIndex];
        if (!movedVal.isEmpty()) {
            QList<int> cands = targetMap.values(movedVal);
            int bestOldDist = INT_MAX, bestNewDist = INT_MAX;
            for (int tIdx : cands) {
                // “旧位置 newIndex”到目标 tIdx 的距离
                int ox = newIndex / width, oy = newIndex % width;
                int xt = targetRow[tIdx], yt = targetCol[tIdx];
                int dOld = std::abs(ox - xt) + std::abs(oy - yt);
                if (dOld < bestOldDist) bestOldDist = dOld;

                // “新位置 emptyPos”到目标 tIdx 的距离
                int nx = current.emptyPos / width, ny = current.emptyPos % width;
                int dNew = std::abs(nx - xt) + std::abs(ny - yt);
                if (dNew < bestNewDist) bestNewDist = dNew;
            }
            newManhattan = current.manhattan - bestOldDist + bestNewDist;
        }

        // —— 2. 记录“交换前”受影响行/列的线性冲突惩罚之和 ——
        int oldPart = 0;
        if (dr == 0) {
            // 水平移动：这时仅受影响 “行 row” 和 “列 col、新Col”
            oldPart += calculateRowConflict(boardCopy, row);
            oldPart += calculateColConflict(boardCopy, col);
            oldPart += calculateColConflict(boardCopy, newCol);
        } else {
            // 垂直移动：仅受影响 “列 col” 和 “行 row、新Row”
            oldPart += calculateColConflict(boardCopy, col);
            oldPart += calculateRowConflict(boardCopy, row);
            oldPart += calculateRowConflict(boardCopy, newRow);
        }

        // —— 3. 真正 swap 空格与 tile ——
        std::swap(boardCopy[current.emptyPos], boardCopy[newIndex]);

        // —— 4. 计算“交换后”受影响行/列的线性冲突之和 ——
        int newPart = 0;
        if (dr == 0) {
            newPart += calculateRowConflict(boardCopy, row);
            newPart += calculateColConflict(boardCopy, col);
            newPart += calculateColConflict(boardCopy, newCol);
        } else {
            newPart += calculateColConflict(boardCopy, col);
            newPart += calculateRowConflict(boardCopy, row);
            newPart += calculateRowConflict(boardCopy, newRow);
        }
        int newLinear = current.linearConflict - oldPart + newPart;

        // —— 5. 构造邻居 State ——
        State neighbor;
        neighbor.board          = boardCopy;
        neighbor.emptyPos       = newIndex;
        neighbor.g              = current.g + 1;
        neighbor.manhattan      = newManhattan;
        neighbor.linearConflict = newLinear;
        neighbor.h              = newManhattan + newLinear;
        neighbor.code           = hashBoard(boardCopy);
        neighbor.path           = current.path;
        neighbor.path.append({ newRow, newCol });

        neighbors.append(neighbor);

        // —— 6. 恢复原来盘面 ——
        std::swap(boardCopy[current.emptyPos], boardCopy[newIndex]);
    }

    return neighbors;
}
void AI::startSolving() {
    qDebug() << "start";
    qDebug() << targetBoard;
    openG.clear();
    closedG.clear();
    while (!openPQ.empty()) openPQ.pop();

    State start;
    start.board    = logic->getBoard();
    start.emptyPos = start.board.indexOf("");
    start.g        = 0;

    // —— 全盘计算曼哈顿距离之和 ——
    int size = start.board.size();
    int manhSum = 0;
    QVector<bool> used(size, false);
    for (int i = 0; i < size; ++i) {
        const QString& val = start.board[i];
        if (val.isEmpty()) continue;
        QList<int> cands = targetMap.values(val);
        int bestDist = INT_MAX;
        int bestIdx  = -1;
        for (int tIdx : cands) {
            if (used[tIdx]) continue;
            int xi = i / width, yi = i % width;
            int xt = targetRow[tIdx], yt = targetCol[tIdx];
            int d = std::abs(xi - xt) + std::abs(yi - yt);
            if (d < bestDist) {
                bestDist = d;
                bestIdx  = tIdx;
            }
        }
        if (bestIdx >= 0) {
            used[bestIdx] = true;
            manhSum += bestDist;
        }
    }
    start.manhattan = manhSum;

    // —— 全盘计算线性冲突惩罚之和 ——
    int totalLin = 0;
    int height = size / width;
    for (int r = 0; r < height; ++r) {
        totalLin += calculateRowConflict(start.board, r);
    }
    for (int c = 0; c < width; ++c) {
        totalLin += calculateColConflict(start.board, c);
    }
    start.linearConflict = totalLin;

    // —— 合成向量 h, 计算哈希, 推入 openPQ/openG ——
    start.h    = start.manhattan + start.linearConflict;
    start.code = hashBoard(start.board);
    openPQ.push(start);
    openG[start.code] = 0;
    // … 之后的循环不变，只是 generateNeighbors 里会用增量曼哈顿 …
    while (!openPQ.empty() && !isExit) {
        State curr = openPQ.top();
        openPQ.pop();
        if (closedG.contains(curr.code)) continue;
        if (openG.contains(curr.code) && openG[curr.code] < curr.g) continue;

        if (curr.board == targetBoard) {
            solutionPath = curr.path;
            solved = true;
            emit solveReady();
            qDebug() << "[A*] 目标已找到";
            return;
        }

        closedG[curr.code] = curr.g;
        QVector<State> neighbors = generateNeighbors(curr);
        for (State& neighbor : neighbors) {
            if (closedG.contains(neighbor.code)) continue;
            int newG = neighbor.g;
            if (openG.contains(neighbor.code) && openG[neighbor.code] <= newG) continue;
            openG[neighbor.code] = newG;
            openPQ.push(neighbor);
        }
    }

    qDebug() << "[A*] 未找到解";
    solved = false;
}

// void AI::startSolving() {
//     qDebug() << "start";
//     qDebug() << targetBoard;
//     openG.clear();
//     closedG.clear();
//     while (!openPQ.empty()) openPQ.pop();

//     State start;
//     start.board = logic->getBoard();
//     start.emptyPos = start.board.indexOf("");
//     start.g = 0;
//     start.h = heuristic(start.board);
//     start.path = {};
//     start.code = hashBoard(start.board);

//     openPQ.push(start);
//     openG[start.code] = 0;

//     while (!openPQ.empty()&&!isExit) {
//         State curr = openPQ.top();
//         openPQ.pop();
//         // 如果状态已在 closed 集合，并且 board 真实重复，跳过
//         if (closedG.contains(curr.code)) continue;

//         // 如果 openG 有更优路径，跳过
//         if (openG.contains(curr.code) && openG[curr.code] < curr.g) continue;


//         if (curr.board == targetBoard) {
//             solutionPath = curr.path;
//             solved = true;
//             emit solveReady();
//             qDebug() << "[A*] 目标已找到";
//             return;
//         }

//         closedG[curr.code] = curr.g;
//         QVector<State> neighbors = generateNeighbors(curr);

//         for (State& neighbor : neighbors) {
//             if (closedG.contains(neighbor.code)) continue;
//             int newG = neighbor.g;

//             // 确保我们只扩展未扩展过且更优的路径
//             if (openG.contains(neighbor.code) && openG[neighbor.code] <= newG) continue;

//             openG[neighbor.code] = newG;
//             openPQ.push(neighbor);
//         }
//     }

//     qDebug() << "[A*] 未找到解";
//     solved = false;

// }

// 动画播放解路径
void AI::solveStep() {
    if (solutionPath.isEmpty()) {
        timer->stop();
        qDebug() << "timer s";
        return;
    }

    auto [blankRow, blankCol] = solutionPath.takeFirst();

    // 空格即将移动到的位置，所以它的前一个位置是要被点击的方块位置
    int emptyIndex = logic->getBoard().indexOf("");
    int emptyRow = emptyIndex / logic->getCols();
    int emptyCol = emptyIndex % logic->getCols();

    // 要点击的方块，就是 blankRow/blankCol 的邻居
    int deltaRow = blankRow - emptyRow;
    int deltaCol = blankCol - emptyCol;

    // 得到要点击的格子的坐标
    int tileRow = emptyRow + deltaRow;
    int tileCol = emptyCol + deltaCol;

    QVector<QString> oldBoard = logic->getBoard();
    qDebug() << 1;
    if (logic->tryMove(tileRow, tileCol)) {
        QVector<QString> newBoard = logic->getBoard();

        int fromIndex = -1, toIndex = -1;
        for (int idx = 0; idx < oldBoard.size(); ++idx) {
            if (!oldBoard[idx].isEmpty() && newBoard[idx].isEmpty())
                fromIndex = idx;
            if (oldBoard[idx].isEmpty() && !newBoard[idx].isEmpty())
                toIndex = idx;
        }

        if (fromIndex != -1 && toIndex != -1) {
            TileButton* movedBtn = tiles[fromIndex];

            int toRow = toIndex / logic->getCols();
            int toCol = toIndex % logic->getCols();
            QPoint targetPos(toCol * buttonWidth, toRow * buttonHeight);

            // 动画效果
            QPropertyAnimation* anim = new QPropertyAnimation(movedBtn, "pos");
            anim->setDuration(200);
            anim->setStartValue(movedBtn->pos());
            anim->setEndValue(targetPos);
            anim->start(QAbstractAnimation::DeleteWhenStopped);

            // 更新按钮数组
            movedBtn->setRowCol(toRow, toCol);
            std::swap(tiles[fromIndex], tiles[toIndex]);

            QTimer::singleShot(160, this, [this]() {
                movesound->play();
            });
        }

        // 判胜
        if (logic->isSolved()) {
            winsound->play();
            emit over(this);
            timer->stop(); // 防止后续误触
        }
    }
}
void AI::startAnimation() {
    if (!timer->isActive()) {
        qDebug() << "timer";
        connect(timer, &QTimer::timeout, this, &AI::solveStep);
        timer->start(300);
    }
}
GamePageWithAI::~GamePageWithAI() {
    delete ai;
    delete player;
    delete ui;
}



// void AI::tryMove(int i, int j) {
//     qDebug() << "AI::tryMove got" << i << j;
//     QVector<QString> oldBoard = logic->getBoard();
//     if (!logic->tryMove(i, j)) return;

//     QVector<QString> newBoard = logic->getBoard();
//     int from = -1, to = -1;
//     for (int k = 0; k < oldBoard.size(); ++k) {
//         if (oldBoard[k].isEmpty() && !newBoard[k].isEmpty()) to = k;
//         if (!oldBoard[k].isEmpty() && newBoard[k].isEmpty()) from = k;
//     }

//     if (from >= 0 && to >= 0) {
//         TileButton* btn = tiles[from];
//         btn->raise();
//         int tr = to / logic->getCols(), tc = to % logic->getCols();
//         QPoint endP(tc * buttonWidth, tr * buttonHeight);
//         QPropertyAnimation* anim = new QPropertyAnimation(btn, "pos");
//         anim->setDuration(200);
//         anim->setStartValue(btn->pos());
//         anim->setEndValue(endP);
//         anim->start(QAbstractAnimation::DeleteWhenStopped);
//         tiles.swapItemsAt(from, to);
//         tiles[to]->setRowCol(tr, tc);
//     }

//     // 刷新棋盘标签
//     for (int k = 0; k < tiles.size(); ++k) tiles[k]->setText(logic->getBoard()[k]);

//     if (logic->isSolved()) emit over(this);
// }


