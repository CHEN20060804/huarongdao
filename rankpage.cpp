#include "rankpage.h"
#include "ui_rankpage.h"
#include "mainbutton.h"

RankPage::RankPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RankPage)
{
    ui->setupUi(this);
    setFixedSize(850, 600);

    MainButton* backBtn = new MainButton(this, 50, 50);
    backBtn->move(50, 30);
    backBtn->setIcon(QIcon(":/video/res/back.png"));
    backBtn->setIconSize(QSize(30, 30));

    connect(backBtn, &QPushButton::clicked, [=](){
        emit mainBtnClicked(0);
    });
}

RankPage::~RankPage()
{
    delete ui;
}
