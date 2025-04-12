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

    for(int i = 0; i < 15; i++)
    {
        MainButton* choseLevelBtn = new MainButton(this, 75, 60);
        choseLevelBtn->setText(QString::number(i+1));
        choseLevelBtn->move(i%5*150+100, i/5*150+150);
        connect(choseLevelBtn, &QPushButton::clicked, [=](){
            emit levelSelected(4, i+1);
        });
    }

    QFont font;
    font.setFamily("楷体");  // 设置字体
    font.setPointSize(36);       // 设置字号
    font.setBold(true);          // 设置加粗
    font.setItalic(false);       // 设置斜体
    ui->label->setFont(font);
}

const LevelManeger& LevelPage::callLevelManger() const
{
    return levelmaneger;
}

LevelPage::~LevelPage()
{
    delete ui;
}
