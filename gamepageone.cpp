#include "gamepageone.h"
#include "ui_gamepageone.h"
#include "mainbutton.h"
#include "leveloriginandautor.h"
#include <QMessageBox>
#include <QScreen>
#include <QDebug>
#include "rankmanager.h"
GamePageOne::GamePageOne(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageOne)
    , logic(nullptr)
    , centralWidget(nullptr)
    , recordBar(nullptr)
    , targetBar(nullptr)
    , customBrowser(nullptr)
    ,gameTimer(nullptr)
    ,difficultyBar(nullptr)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));

    MainButton* reloadBtn = new MainButton(this, 50, 50);
    reloadBtn->move(150, 30);
    reloadBtn->setIcon(QIcon(":/video/res/reload.png"));
    reloadBtn->setIconSize(QSize(30, 30));

    connect(backBtn, &QPushButton::clicked, [=](){
        resetRecord();
        if(isCustom==false)
        {
            emit mainBtnClicked(1);
        }
        else
        {
            emit mainBtnClicked(2);
        }
    });

    connect(reloadBtn, &QPushButton::clicked, [=](){
        resetRecord();
        loadLevel(level, isCustom);
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
            youWin();//胜利效果
            if(!isCustom)
            {
                session.setDifficult(hardness);
                RankManager::getInstance()->writeRecord(session);
                qDebug() << "ok";
            }
        }
    }
}

void GamePageOne::loadLevel(const Level& level, bool isCustom) {
    this->level = level;
    this->isCustom = isCustom;
    int cols = level.getw();
    int rows = level.geth();
    int ID = level.getId();

    if(isCustom==false)
    {
        setTitle(ID);
        //初始化滑动框
        if(customBrowser)
        {
            customBrowser->deleteLater();
            customBrowser = nullptr;
        }
        if(targetBar)
        {
            targetBar->deleteLater();
            targetBar = nullptr;
        }
        QString targetText = LevelOriginAndAutor::getTargetText(ID);
        showTargetText(targetText);
    }
    else
    {
        ui->label->hide();
        if(customBrowser)
        {
            customBrowser->deleteLater();
            customBrowser = nullptr;
        }
        if(targetBar)
        {
            targetBar->deleteLater();
            targetBar = nullptr;
        }
        createCustomTargetDisplay(this, level.getElement());
    }

    initRecordSlidingSidebar();

    logic = std::make_unique<GameLogicOne>(cols, rows, level.getElement());

    if (centralWidget) {
        centralWidget->deleteLater();
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
    hardness = logic->computeDifficulty();
    qDebug() << hardness;

    if(difficultyBar)
    {
        difficultyBar->deleteLater();
        difficultyBar = nullptr;
    }
    difficultyBar = new DifficultyLabel(this, 0, 100);
    difficultyBar->setDifficulty(hardness);
    difficultyBar->show();

    updateUI();

    centralWidget->show();
}

void GamePageOne::initRecordSlidingSidebar()
{
    if(recordBar)
    {
        recordBar->deleteLater();
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

void GamePageOne::showTargetText(const QString& targetText)
{
    targetBar = new SlidingSidebar(this,
                                   260,                     // 宽度
                                   250,                     // 高度
                                   QPoint(0 , 152), // 初始位置
                                   "#FFF0F5",               // 背景颜色
                                   "楷体",                  // 字体
                                   16,                      // 字号
                                   5,
                                   SlidingSidebar::SlideLeft); // 向右收起
    targetBar->setLineText(0, targetText);
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
    session.stop();

}

void GamePageOne::setTitle(int ID)
{
    QFont font;
    font.setPointSize(36);
    font.setItalic(false);
    font.setFamily("Raleway");
    ui->label->setFont(font);
    ui->label->setStyleSheet("border-radius: 10px; border: 2px solid white;");
    ui->label->setAlignment(Qt::AlignCenter);  // 设置居中对齐
    ui->label->setText(QString::number(ID + 1));
}
void GamePageOne::youWin()
{
    for(int i = 0;i< tiles.size();i++)
    {
        tiles[i]->setEnabled(false);
    }
    auto* glowEffect = new QGraphicsDropShadowEffect(this);
    glowEffect->setColor(QColor(255, 215, 0)); // 金黄色
    glowEffect->setOffset(0, 0);               // 无偏移，仅环绕边缘
    glowEffect->setBlurRadius(60);             // 柔和模糊程度
    centralWidget->setGraphicsEffect(glowEffect);

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
void GamePageOne::createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry)
{

    customBrowser = new QTextBrowser(parent);
    customBrowser->setGeometry(geometry);

    // 设置简洁适配的半透明浅黄色背景
    QPalette palette = customBrowser->palette();
    palette.setColor(QPalette::Base, QColor(241, 245, 202)); // 浅黄色 (LightYellow)，透明度180
    palette.setColor(QPalette::Text, QColor(0, 0, 0));             // 黑色文字
    customBrowser->setPalette(palette);

    // 设置字体和样式
    customBrowser->setStyleSheet(R"(
        QTextBrowser {
            border: 1px solid white;
            border-radius: 8px;
            padding: 8px;
            font-family: "楷体";
            font-size: 24px;
            background-clip: padding;
        }
    )");

    // 拼接内容 —— 每个目标之间空一格
    QString content = "<b>目标顺序：</b><br><br>";
    for (const QString& item : targetList) {
        content += item + " ";
    }

    customBrowser->setText(content.trimmed());

    customBrowser->setReadOnly(true);
    customBrowser->show();
}

GamePageOne::~GamePageOne() {
    delete centralWidget;
    delete ui;
}
