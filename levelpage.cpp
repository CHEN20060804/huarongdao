#include "levelpage.h"
#include "ui_levelpage.h"
#include "mainbutton.h"

LevelPage::LevelPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelPage)
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

    for(int i = 0; i < levelmaneger.getSize(); i++)
    {
        MainButton* choseLevelBtn = new MainButton(this, 70, 60);
        choseLevelBtn->setText(QString::number(i+1));
        choseLevelBtn->move(i%5*150+100, i/5*100+150);
        connect(choseLevelBtn, &QPushButton::clicked, [=](){
            emit levelSelected(4, i);
        });
    }

}

const LevelManeger& LevelPage::callLevelManger() const
{
    return levelmaneger;
}

LevelPage::~LevelPage()
{
    delete ui;
}
