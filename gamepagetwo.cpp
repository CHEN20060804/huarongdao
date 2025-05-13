#include "gamepagetwo.h"
#include "ui_gamepagetwo.h"
#include "mainbutton.h"
#include <QFrame>
#include <QStyle>
#include <QParallelAnimationGroup>
//#include <QKeyEvent>
GamePageTwo::GamePageTwo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageTwo)
    ,playerL(nullptr)
    ,playerR(nullptr)
    ,widgetL(nullptr)
    ,widgetR(nullptr)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));
    connect(backBtn, &QPushButton::clicked, [=]{
        emit mainBtnClicked(2);
    });
    QFrame* dividerLine = new QFrame(this);
    dividerLine->setFrameShape(QFrame::VLine);
    dividerLine->setFrameShadow(QFrame::Sunken);     // 凹陷视觉
    dividerLine->setLineWidth(2);                    // 线宽
    dividerLine->setMidLineWidth(1);
    dividerLine->setGeometry(850 / 2 - 1, 186, 2, 500);
    dividerLine->show();

    setFocusPolicy(Qt::StrongFocus);
    this->grabKeyboard();
    setFocus();
}

void GamePageTwo::moveSelection(QPair<int, int> &pos, int dRow, int dCol, Player2* player) {
    int rows = level.geth();
    int cols = level.getw();
    int newRow = pos.first + dRow;
    int newCol = pos.second + dCol;
    QPair<int, int> newpos = {newRow, newCol};


    if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols ) return;

    player->getOneTile(pos)->setSelected(false);
    player->getOneTile(newpos)->setSelected(true);
    pos = newpos;
    qDebug() << pos;
}

void GamePageTwo::triggerSelected(const QPair<int, int>& pos, Player2* player) {
    auto isTileEmpty = [&](){
        return player->getOneTile(pos)->text().isEmpty();
    };
    if(!isTileEmpty())
        emit player->getOneTile(pos)->triggered();
}
void GamePageTwo::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    // 玩家1控制左边棋盘
    case Qt::Key_W:
        if (!playerL->isAnimating) moveSelection(*selectedL, -1, 0, playerL);
        break;
    case Qt::Key_S:
        if (!playerL->isAnimating) moveSelection(*selectedL, 1, 0, playerL);
        break;
    case Qt::Key_A:
        if (!playerL->isAnimating) moveSelection(*selectedL, 0, -1, playerL);
        break;
    case Qt::Key_D:
        if (!playerL->isAnimating) moveSelection(*selectedL, 0, 1, playerL);
        break;
    case Qt::Key_F:
        if (!playerL->isAnimating) triggerSelected(*selectedL, playerL);
        break;

        // 玩家2控制右边棋盘
    case Qt::Key_Up:
        if (!playerR->isAnimating) moveSelection(*selectedR, -1, 0, playerR);
        break;
    case Qt::Key_Down:
        if (!playerR->isAnimating) moveSelection(*selectedR, 1, 0, playerR);
        break;
    case Qt::Key_Left:
        if (!playerR->isAnimating) moveSelection(*selectedR, 0, -1, playerR);
        break;
    case Qt::Key_Right:
        if (!playerR->isAnimating) moveSelection(*selectedR, 0, 1, playerR);
        break;
    case Qt::Key_Control:
        if (!playerR->isAnimating) triggerSelected(*selectedR, playerR);
        break;
    }
}


void GamePageTwo::loadLevel(const Level& level) {

    this->level = level;
    // 显示目标字符
    createCustomTargetDisplay(this, level.getElement());

    // 清理旧控件和对象
    if (widgetL) {
        widgetL->deleteLater();
        widgetL = nullptr;
    }
    if (widgetR) {
        widgetR->deleteLater();
        widgetR = nullptr;
    }
    if (playerL) {
        delete playerL;
        playerL = nullptr;
    }
    if (playerR) {
        delete playerR;
        playerR = nullptr;
    }
    //qDebug() <<"lll";

    // 创建 playerWidget
    widgetL = new QWidget(this);
    widgetL->setObjectName("playerFrame");
    widgetL->setStyleSheet(R"(
        #playerFrame {
            background: transparent;
            border: 10px solid rgba(0, 255, 0, 50);
        }
    )");
    widgetL->move(50, 200);  // 左边

    // 创建 aiWidget
    widgetR = new QWidget(this);
    widgetR->setObjectName("aiFrame");
    widgetR->setStyleSheet(R"(
        #aiFrame {
            background: transparent;
            border: 10px solid rgba(255, 0, 0, 50);
        }
    )");

    // 创建新对象
    playerL = new Player2(this);
    playerR = new Player2(this);


    // 初始化棋盘
    playerL->initBoard(level, widgetL);
    playerR->initBoard(level, widgetR);
    playerL->logic->shuffle();
    playerR->logic->setOrder(*playerL->logic);
    // 调整 aiWidget 位置
    widgetR->adjustSize();
    int x = this->width() - 50 - widgetR->width();
    widgetR->move(x, 200);

    playerL->selected = std::make_shared<QPair<int, int>>(level.geth() - 1, level.getw() - 2);
    playerR->selected = std::make_shared<QPair<int, int>>(level.geth() - 1, level.getw() - 2);
    selectedL = playerL->selected;
    selectedR = playerR->selected;


    // 绑定胜利信号
    connect(playerL, &Player2::over, this, &GamePageTwo::winEffect);
    connect(playerR, &Player2::over, this, &GamePageTwo::winEffect);

    // 更新 UI
    updateUI(playerR);
    updateUI(playerL);

    playerL->getOneTile(*selectedL)->setSelected(true);
    playerR->getOneTile(*selectedR)->setSelected(true);

    widgetL->show();
    widgetR->show();
}


void Player2::initBoard(const Level& level, QWidget* parent) {
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
    boardBg->setStyleSheet("background-color: lightgray;");//lightgray
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
            connect(btn, &TileButton::triggered, parent, [this, btn]() {
                tryMove(btn->row(), btn->col());
            });
            tiles[index] = btn;
        }
    }
}

void Player2::tryMove(int i, int j) {
    QVector<QString> oldBoard = logic->getBoard();
    if (!logic->tryMove(i, j)) return;
    QVector<QString> newBoard = logic->getBoard();

    int fromIndex = -1, toIndex = -1;
    for (int k = 0; k < oldBoard.size(); ++k) {
        if (!oldBoard[k].isEmpty() && newBoard[k].isEmpty()) fromIndex = k;
        if (oldBoard[k].isEmpty()  && !newBoard[k].isEmpty()) toIndex   = k;
    }
    if (fromIndex < 0 || toIndex < 0) return;

    TileButton* movedBtn = tiles[fromIndex];
    TileButton* emptyBtn = tiles[toIndex];

    movedBtn->raise();
    emptyBtn->lower();

    QPoint posMoved = movedBtn->pos();
    QPoint posEmpty = emptyBtn->pos();

    auto *animMoved = new QPropertyAnimation(movedBtn, "pos");
    animMoved->setDuration(180);
    animMoved->setStartValue(posMoved);
    animMoved->setEndValue(posEmpty);

    auto *animEmpty = new QPropertyAnimation(emptyBtn, "pos");
    animEmpty->setDuration(180);
    animEmpty->setStartValue(posEmpty);
    animEmpty->setEndValue(posMoved);

    QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
    group->addAnimation(animMoved);
    group->addAnimation(animEmpty);

    isAnimating = true;
    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        qSwap(tiles[fromIndex], tiles[toIndex]);

        int toRow = toIndex / logic->getCols();
        int toCol = toIndex % logic->getCols();
        int fromRow = fromIndex / logic->getCols();
        int fromCol = fromIndex % logic->getCols();
        movedBtn->setRowCol(toRow, toCol);
        emptyBtn->setRowCol(fromRow, fromCol);

        movedBtn->setText(newBoard[toIndex]);
        emptyBtn->setText("");
        movedBtn->setEmpty(false);
        emptyBtn->setEmpty(true);

        auto oldSel = *selected;
        if (oldSel.first >= 0 && oldSel.second >= 0)
            getOneTile(oldSel)->setSelected(false);

        if (!newBoard[toIndex].isEmpty()) {
            *selected = { toRow, toCol };
            getOneTile(*selected)->setSelected(true);
        } else {
            *selected = { -1, -1 };
        }

        isAnimating = false;

        if (logic->isSolved()) {
            emit over(this);
        }
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}


TileButton* Player2::getOneTile(QPair<int, int> pos) {
    for (TileButton* btn : tiles) {
        if (btn->row() == pos.first && btn->col() == pos.second) {
            return btn;
        }
    }
    return nullptr;
}

void GamePageTwo::updateUI(Man* man) {
    QVector<TileButton*> tiles = man->getTiles();
    QVector<QString> board = playerL->logic->getBoard();
    for (int i = 0; i < board.size(); ++i) {
        if (TileButton* btn = tiles[i]) {
            QString val = board[i];
            bool isEmpty = val.trimmed().isEmpty();
            btn->setEmpty(isEmpty);
            btn->setText(val);
           // btn->setSelected(false);
        }
    }
    update();

}
void GamePageTwo::winEffect(Man* man)
{
    QVector<TileButton*> tiles;
    QWidget* widget;
    if( man == playerL)
    {
        tiles = playerL->tiles;
        widget = widgetL;
    }
    else
    {
        tiles = playerR->tiles;
        widget = widgetR;
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



void GamePageTwo::createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry)
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
GamePageTwo::~GamePageTwo()
{
    this->releaseKeyboard();
    delete ui;
}
