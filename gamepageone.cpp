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
    QVector<QString> board = logic->getBoard();

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
        auto b = logic->getBoard();
        qDebug() << "After move:" << b;
        updateUI();
        if (logic->isSolved()) {
            QMessageBox::information(this, "胜利", "你赢了！");
            logic->shuffle();
            qDebug() << "logic board = " << logic->getBoard();
            updateUI();
        }
    }
}

void GamePageOne::loadLevel(Level level) {
    int cols = level.w; // 列数
    int rows = level.h; // 行数
    if (logic) {
        delete logic;
        logic = nullptr;
    }
    logic = new GameLogicOne(cols, rows, level.element);

    if (centralWidget) {
        delete centralWidget;
        centralWidget = nullptr;
    }
    centralWidget = new QWidget(this);

    centralWidget->move(300-(cols-3)*18, 150);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(5);
    centralWidget->setLayout(gridLayout);

    centralWidget->setStyleSheet("background-color: rgba(0, 255, 0, 50); border: 5px solid black;");

    tiles = QVector<QPushButton*>(rows * cols, nullptr);

    // 创建按钮
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col; // 行优先计算索引
            tiles[index] = new QPushButton(centralWidget);
            tiles[index]->setFixedSize(80-(cols-3)*10, 80-(rows-3)*10);
            gridLayout->addWidget(tiles[index], row, col); // 按行、列添加
            connect(tiles[index], &QPushButton::clicked, this, [=]() {
                this->tryMove(row, col); // 传递正确的行、列参数
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
