#include "man.h"
#include "slidingsidebar.h"
#include "settingmanager.h"
void Man::startRecord()
{
    isStartRecord = true;
    session = GameSessionData();
    session.start();
    if(!gameTimer)
    {
        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &Man::updateTimeDisplay);
    }
    gameTimer->start(100);
   // updateStepdisplay();
}


void Man::updateTimeDisplay()
{
    double seconds = session.getElapsedSeconds();
    QString currentSeconds = QString::number(seconds, 'f', 1);
    QString showTime = "用时：" + currentSeconds + " s";
    recordBar->setLineText(0, showTime);
}
void Man::updateStepdisplay()
{
    session.addStep();
    int steps = session.getSteps();
    QString currentSteps = QString::number(steps);
    QString showSteps = "步数：" + currentSteps;
    recordBar->setLineText(1, showSteps);
}

void Man::resetRecord()
{
    session.reset();
    QString currentSeconds = "0";
    QString showTime = "用时：" + currentSeconds + " s";
    recordBar->setLineText(0, showTime);
    QString currentSteps = "0";
    QString showSteps = "步数：" + currentSteps;
    recordBar->setLineText(1, showSteps);
}

void Man::stopRecord()
{
    gameTimer->stop();
    double seconds = session.getElapsedSeconds();
    QString currentSeconds = QString::number(seconds, 'f', 1);
}

void Man::initRecordSlidingSidebar(QPoint point, SlidingSidebar::SlideDirection direction, QWidget* parent)
{
    if(recordBar)
    {
        recordBar->deleteLater();
        recordBar = nullptr;
    }
    qDebug() << "new";
    recordBar = new SlidingSidebar(parent,
                                   200,                     // 宽度
                                   80,                     // 高度
                                   point, // 初始位置
                                   "#FFF0F5",               // 背景颜色
                                   "楷体",                  // 字体
                                   16,                      // 字号
                                   5,
                                   direction);
    if(gameTimer)
    {
        delete gameTimer;
        gameTimer = nullptr;//重置计时器
    }
    recordBar->setLineText(0, "用时：0 s");
    recordBar->setLineText(1, "步数：0 ");
    recordBar->show();
}


QVector<TileButton*> Man::getTiles() {
    return tiles;
}

Man::~Man()
{
}
