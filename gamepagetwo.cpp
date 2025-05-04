#include "gamepagetwo.h"
#include "ui_gamepagetwo.h"
#include "mainbutton.h"
GamePageTwo::GamePageTwo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageTwo)
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
}

GamePageTwo::~GamePageTwo()
{
    delete ui;
}
