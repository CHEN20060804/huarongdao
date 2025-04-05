#include "startpage.h"
#include "ui_startpage.h"
#include "mainbutton.h"

StartPage::StartPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartPage)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    //创建边角按钮
    MainButton *settingBtn = new MainButton(this, 50, 50);
    MainButton *exitBtn = new MainButton(this, 50, 50);
    int x0 = width() - settingBtn->width() -50 ;
    int y0 = 30;
    settingBtn->move(x0, y0);  // 设置按钮的位置
    settingBtn->setIcon(QIcon(":/video/res/setting.png"));
    settingBtn->setIconSize(QSize(30, 30));

    int x1 =width() - exitBtn->width() - 50;
    int y1 = 100;
    exitBtn->move(x1, y1);
    exitBtn->setIcon(QIcon(":/video/res/exit.png"));
    exitBtn->setIconSize(QSize(30, 30));

    connect(exitBtn,&QPushButton::clicked,[=]{
        close();
    });

    connect(ui->btnLevel,&QPushButton::clicked, [=](){
        emit mainBtnClicked(1);//发出信号，要求切换到索引为1的界面
    });

    connect(ui->btnCustom,&QPushButton::clicked, [=](){
        emit mainBtnClicked(2);
    });

    connect(ui->btnRank,&QPushButton::clicked, [=](){
        emit mainBtnClicked(3);
    });

    connect(exitBtn,&QPushButton::clicked, [=](){
        emit closeBtnClicked();
    });

}


StartPage::~StartPage()
{
    delete ui;
}
