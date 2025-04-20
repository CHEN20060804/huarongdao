#include "gamepageone.h"
#include "ui_gamepageone.h"
#include "mainbutton.h"
#include "leveloriginandautor.h"
#include <QMessageBox>
#include <QScreen>
#include <QDebug>
GamePageOne::GamePageOne(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageOne)
    , logic(nullptr)
    , centralWidget(nullptr)
    , recordBar(nullptr)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));
    connect(backBtn, &QPushButton::clicked, [=](){
        resetRecord();
        emit mainBtnClicked(1);
    });
}

void GamePageOne::updateUI() {
    QVector<QString> board = logic->getBoard();
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

void GamePageOne::tryMove(int i, int j) {
    QVector<QString> oldBoard = logic->getBoard(); // 保存旧状态

    if (logic->tryMove(i, j)) {
        updateStepdisplay();
        if(currentSteps=="1")startRecord(level);
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
                toCol * m_buttonWidth ,
                toRow * m_buttonHeight
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
            stopRecord();
            QMessageBox::information(this, "胜利", "你赢了！");
            resetRecord();
            logic->shuffle();
            qDebug() << "logic board = " << logic->getBoard();
            updateUI();
        }
    }
}

void GamePageOne::loadLevel(const Level& level) {
    this->level = level;
    int cols = level.getw();
    int rows = level.geth();

    //初始化滑动框
    initRecordSlidingSidebar();
    QString targetText = LevelOriginAndAutor::getTargetText(level.getId());
    QString origin = LevelOriginAndAutor::getOrigin(level.getId());
    QString autor = LevelOriginAndAutor::getAutor(level.getId());
    showTargetText(targetText, origin, autor);

    // ——— 1. 清理旧逻辑和界面 ———
    if (logic) {
        delete logic;
        logic = nullptr;
    }
    logic = new GameLogicOne(cols, rows, level.getElement());

    if (centralWidget) {
        delete centralWidget;
        centralWidget = nullptr;
    }

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("borderFrame");
    centralWidget->setStyleSheet(R"(
        #borderFrame {
            background: transparent;                     /* 底色全透明 */
            border: 10px solid rgba(0, 255, 0, 50);      /* 半透明绿框 (alpha=128) */
        }
    )");
    centralWidget->move(300 - (cols - 3) * 10, 150);

    auto *outerLayout = new QVBoxLayout(centralWidget);
    outerLayout->setContentsMargins(10, 10, 10, 10);
    outerLayout->setSpacing(0);

    QWidget *boardBg = new QWidget(centralWidget);
    boardBg->setObjectName("boardBg");
    boardBg->setStyleSheet("background-color: lightgray;");  // 棋盘底色
    outerLayout->addWidget(boardBg);

    auto *grid = new QGridLayout(boardBg);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    m_buttonWidth  = 80 - (cols - 3) * 10;
    m_buttonHeight = 80 - (rows - 3) * 10;
    tiles.resize(rows * cols);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            auto *btn = new TileButton(boardBg, m_buttonWidth, m_buttonHeight);
            grid->addWidget(btn, row, col);
            btn->setRowCol(row, col);
            connect(btn, &TileButton::clicked, this, [this, btn]() {
                tryMove(btn->row(), btn->col());
            });

            tiles[index] = btn;
        }
    }

    logic->shuffle();
    updateUI();
    centralWidget->show();
}

void GamePageOne::initRecordSlidingSidebar()
{
    if(recordBar)
    {
        delete recordBar;
        recordBar = nullptr;
    }
    recordBar = new SlidingSidebar(this,
                                   200,                     // 宽度
                                   80,                     // 高度
                                   QPoint(width() - 200 , 50), // 初始位置
                                   "#FFF0F5",               // 背景颜色
                                   "楷体",                  // 字体
                                   16,                      // 字号
                                   5,
                                   SlidingSidebar::SlideRight); // 向右收起
    if(gameTimer)
    {
        delete gameTimer;
        gameTimer = nullptr;//重置计时器
    }
    recordBar->setLineText(0, "用时：0 s");
    recordBar->setLineText(1, "步数：0 ");
    recordBar->show();
}

void GamePageOne::showTargetText(const QString& targetText, const QString& origin, const QString& autor)
{
    if(targetBar)
    {
        delete targetBar;
        targetBar = nullptr;
    }
    targetBar = new SlidingSidebar(this,
                                   200,                     // 宽度
                                   400,                     // 高度
                                   QPoint(0 , 100), // 初始位置
                                   "#FFF0F5",               // 背景颜色
                                   "楷体",                  // 字体
                                   16,                      // 字号
                                   5,
                                   SlidingSidebar::SlideLeft); // 向右收起
    targetBar->setLineText(0, targetText);
    targetBar->setLineText(1, origin);
    targetBar->setLineText(2, autor);
    targetBar->show();
}

void GamePageOne::startRecord(Level level)
{
    session = GameSessionData(level.getId());
    session.start();
    if(!gameTimer)
    {
        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &GamePageOne::updateTimeDisplay);
    }
    gameTimer->start(100);
    updateStepdisplay();
}


void GamePageOne::updateTimeDisplay()
{
    double seconds = session.getElapsedSeconds();
    currentSeconds = QString::number(seconds, 'f', 1);
    QString showTime = "用时：" + currentSeconds + " s";
    recordBar->setLineText(0, showTime);
}
void GamePageOne::updateStepdisplay()
{
    session.addStep();
    int steps = session.getSteps();
    currentSteps = QString::number(steps);
    QString showSteps = "步数：" + currentSteps;
    recordBar->setLineText(1, showSteps);
}

void GamePageOne::resetRecord()
{
    session.reset();
    currentSeconds = "0";
    QString showTime = "用时：" + currentSeconds + " s";
    recordBar->setLineText(0, showTime);
    currentSteps = "0";
    QString showSteps = "步数：" + currentSteps;
    recordBar->setLineText(1, showSteps);
}

void GamePageOne::stopRecord()
{
    gameTimer->stop();
    double seconds = session.getElapsedSeconds();
    currentSeconds = QString::number(seconds, 'f', 1);
}

GamePageOne::~GamePageOne() {
    delete logic;
    delete centralWidget;
    delete ui;
}
