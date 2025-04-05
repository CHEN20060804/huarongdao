#include "custompage.h"
#include "ui_custompage.h"
#include "mainbutton.h"
CustomPage::CustomPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomPage)
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

CustomPage::~CustomPage()
{
    delete ui;
}
