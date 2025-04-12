#include "gamepageone.h"
#include "ui_gamepageone.h"
#include "mainbutton.h"
#include <QMessageBox>
#include <QScreen>

GamePageOne::GamePageOne(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageOne)
    , logic(nullptr)
    , centralWidget(nullptr)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));
    connect(backBtn, &QPushButton::clicked, [=](){
        emit mainBtnClicked(1);
    });
}

void GamePageOne::updateUI() {
    QVector<QString> board = logic->getBoard(); // 改为一维
    qDebug() << "Current board:";
    for (const auto& cell : board) qDebug() << cell;

    for (int i = 0; i < board.size(); ++i) {
        QPushButton* btn = tiles[i];
        QString val = board[i];
        btn->setText(val);
        btn->setStyleSheet(val.trimmed().isEmpty()
                               ? "background-color: lightgray;"
                               : "font-size: 24px; background-color: white; color: black;");
    }
}

void GamePageOne::tryMove(int i, int j) {
    if (logic->tryMove(i, j)) {
        updateUI();
        if (logic->isSolved()) {
            QMessageBox::information(this, "胜利", "你赢了！");
            logic->shuffle();
            updateUI();
        }
    }
}

void GamePageOne::loadLevel(Level level) {
    int w = level.w;
    int h = level.h;

    if (logic) {
        delete logic;
        logic = nullptr;
    }
    logic = new GameLogicOne(w, h, level.element);

    if (centralWidget) {
        delete centralWidget;
        centralWidget = nullptr;
    }
    centralWidget = new QWidget(this);

    centralWidget->move(300, 150);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(5);
    centralWidget->setLayout(gridLayout);

    centralWidget->setStyleSheet("background-color: rgba(0, 255, 0, 50); border: 5px solid black;");

    tiles = QVector<QPushButton*>(w * h, nullptr); // 生成一维数组

    // 创建按钮
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            int index = i * h + j; // 计算一维数组的索引
            tiles[index] = new QPushButton(centralWidget);
            tiles[index]->setFixedSize(80, 80);
            tiles[index]->setStyleSheet("font-size: 24px;");
            gridLayout->addWidget(tiles[index], i, j);
            connect(tiles[index], &QPushButton::clicked, this, [=]() {
                this->tryMove(i, j);
            });
        }
    }

    logic->shuffle();
    qDebug() << "logic board = " << logic->getBoard();
    updateUI();
    centralWidget->show();
}

GamePageOne::~GamePageOne() {
    delete logic;
    delete centralWidget;
    delete ui;
}
