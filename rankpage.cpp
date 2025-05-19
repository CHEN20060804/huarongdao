#include "rankpage.h"
#include "ui_rankpage.h"
#include "mainbutton.h"
#include <QHeaderView>
#include "gamesessiondata.h"
#include "rankmanager.h"
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


    table = new FancyTableWidget(this);
    table->setRowCount(20);
    table->setColumnCount(5);
    table->setGeometry(125, 100, 600, 400);
    table->setHorizontalHeaderLabels({"关卡", "用时（s）", "步数", "难度系数", "日期"});

    for (int row = 0; row < table->rowCount(); ++row) {
        auto *item = new QTableWidgetItem(QString::number(row + 1));
        item->setTextAlignment(Qt::AlignCenter);  // 居中
        table->setItem(row, 0, item);
    }
}

void RankPage::showRecord()
{
    QVector<GameSessionData> data = RankManager::getInstance()->getLevelBest();
    qDebug() << "show";
    for(int i = 0; i < data.size(); i++)
    {
        GameSessionData d = data[i];

        double secs = d.getSeconds();
        QString secStr = QString::number(secs, 'f', 1);

        auto *item1 = new QTableWidgetItem(secStr);
        item1->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 1, item1);


        // 步数
        auto *item2 = new QTableWidgetItem(QString::number(d.getSteps()));
        item2->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, item2);

        // 难度系数
        double dif = d.getDifficulty();
        QString difStr = QString::number(dif, 'f', 3);

        auto *item3 = new QTableWidgetItem(difStr);
        item3->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, item3);

        // 日期
        auto *item4 = new QTableWidgetItem(d.getBeijingTime());
        item4->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 4, item4);
    }
}

RankPage::~RankPage()
{
    delete ui;
}
