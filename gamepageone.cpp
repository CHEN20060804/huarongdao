#include "gamepageone.h"
#include "ui_gamepageone.h"
#include "mainbutton.h"

GamePageOne::GamePageOne(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePageOne)
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

GamePageOne::~GamePageOne()
{
    delete ui;
}
