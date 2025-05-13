#include "gamepagewithai.h"
#include "ui_gamepagewithai.h"
#include "mainbutton.h"
#include <QFrame>
#include <QtMath>
#include <climits>
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
        emit mainBtnClicked(2);
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
        }

        if (logic->isSolved()) {
              //stopRecord();
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
        delete player;
        player = nullptr;
    }
    if (ai) {
        delete ai;
        ai = nullptr;
    }
    //qDebug() <<"lll";

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

    // 绑定胜利信号
    connect(player, &Player::over, this, &GamePageWithAI::winEffect);
    connect(ai, &AI::over, this, &GamePageWithAI::winEffect);

    // 更新 UI
    updateUI(player);
    qDebug() << ai->tiles.size();
    updateUI(ai);

    playerWidget->show();
    AIWidget->show();
}



void Player::initBoard(const Level& level, QWidget* parent) {
    int cols = level.getw();
    int rows = level.geth();

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



// void GamePageWithAI::initRecordSlidingSidebar()
// {
//     if(recordBar)
//     {
//         recordBar->deleteLater();
//         recordBar = nullptr;
//     }
//     recordBar = new SlidingSidebar(this,
//                                    200,                     // 宽度
//                                    80,                     // 高度
//                                    QPoint(width() - 200 , 50), // 初始位置
//                                    "#FFF0F5",               // 背景颜色
//                                    "楷体",                  // 字体
//                                    16,                      // 字号
//                                    5,
//                                    SlidingSidebar::SlideRight); // 向右收起
//     if(gameTimer)
//     {
//         delete gameTimer;
//         gameTimer = nullptr;//重置计时器
//     }
//     recordBar->setLineText(0, "用时：0 s");
//     recordBar->setLineText(1, "步数：0 ");
//     recordBar->show();
// }


// void GamePageWithAI::startRecord(Level level)
// {
//     session.start();
//     if(!gameTimer)
//     {
//         gameTimer = new QTimer(this);
//         connect(gameTimer, &QTimer::timeout, this, &GamePageOne::updateTimeDisplay);
//     }
//     gameTimer->start(100);
//     updateStepdisplay();
// }


// void GamePageWithAI::updateTimeDisplay()
// {
//     double seconds = session.getElapsedSeconds();
//     currentSeconds = QString::number(seconds, 'f', 1);
//     QString showTime = "用时：" + currentSeconds + " s";
//     recordBar->setLineText(0, showTime);
// }
// void GamePageWithAI::updateStepdisplay()
// {
//     session.addStep();
//     int steps = session.getSteps();
//     currentSteps = QString::number(steps);
//     QString showSteps = "步数：" + currentSteps;
//     recordBar->setLineText(1, showSteps);
// }

// void GamePageWithAI::resetRecord()
// {
//     session.reset();
//     currentSeconds = "0";
//     QString showTime = "用时：" + currentSeconds + " s";
//     recordBar->setLineText(0, showTime);
//     currentSteps = "0";
//     QString showSteps = "步数：" + currentSteps;
//     recordBar->setLineText(1, showSteps);
// }

// void GamePageWithAI::stopRecord()
// {
//     gameTimer->stop();
//     double seconds = session.getElapsedSeconds();
//     currentSeconds = QString::number(seconds, 'f', 1);
// }

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

GamePageWithAI::~GamePageWithAI() {
    delete ai;
    delete player;
    delete ui;
}

void AI::initBoard(const Level& level, QWidget* parent) {
    int cols = level.getw();
    int rows = level.geth();
    qDebug() <<"c" << cols << " " << "r" << rows;


    logic = std::make_unique<GameLogicOne>(cols, rows, level.getElement());


    targetBoard = level.getElement();

    // clear old widgets
    if (parent->layout()) {
        QLayoutItem* item;
        while ((item = parent->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete parent->layout();
    }

    auto *outerLayout = new QVBoxLayout(parent);
    outerLayout->setContentsMargins(10,10,10,10);
    outerLayout->setSpacing(0);

    QWidget* boardBg = new QWidget(parent);
    boardBg->setObjectName("boardBg");
    boardBg->setStyleSheet("background-color: lightgray;");
    outerLayout->addWidget(boardBg);

    auto *grid = new QGridLayout(boardBg);
    grid->setContentsMargins(0,0,0,0);
    grid->setSpacing(0);

    buttonWidth  = 70 - (cols - 3) * 10;
    buttonHeight = 70 - (rows - 3) * 10;
    tiles.resize(rows * cols);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int idx = r * cols + c;
            TileButton* btn = new TileButton(boardBg, buttonWidth, buttonHeight);
            grid->addWidget(btn, r, c);
            btn->setRowCol(r, c);
            tiles[idx] = btn;
        }
    }

    qDebug() << tiles.size();

   qDebug() << "lll";

}

void AI::tryMove(int i, int j) {
    qDebug() << "AI::tryMove got" << i << j;
    QVector<QString> oldBoard = logic->getBoard();
    if (!logic->tryMove(i, j)) return;

    QVector<QString> newBoard = logic->getBoard();
    int from = -1, to = -1;
    for (int k = 0; k < oldBoard.size(); ++k) {
        if (oldBoard[k].isEmpty() && !newBoard[k].isEmpty()) to = k;
        if (!oldBoard[k].isEmpty() && newBoard[k].isEmpty()) from = k;
    }

    if (from >= 0 && to >= 0) {
        TileButton* btn = tiles[from];
        btn->raise();
        int tr = to / logic->getCols(), tc = to % logic->getCols();
        QPoint endP(tc * buttonWidth, tr * buttonHeight);
        QPropertyAnimation* anim = new QPropertyAnimation(btn, "pos");
        anim->setDuration(200);
        anim->setStartValue(btn->pos());
        anim->setEndValue(endP);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        tiles.swapItemsAt(from, to);
        tiles[to]->setRowCol(tr, tc);
    }

    // 刷新棋盘标签
    for (int k = 0; k < tiles.size(); ++k) tiles[k]->setText(logic->getBoard()[k]);

    if (logic->isSolved()) emit over(this);
}


