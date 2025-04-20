#ifndef GAMEPAGEONE_H
#define GAMEPAGEONE_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QTimer>
#include "gamelogicone.h"
#include "levelmaneger.h"
#include "tilebutton.h"
#include "slidingsidebar.h"
#include "gamesessiondata.h"
namespace Ui {
class GamePageOne;
}

class GamePageOne : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageOne(QWidget *parent = nullptr);
    ~GamePageOne();

    void loadLevel(const Level& level);

signals:
    void mainBtnClicked(int);
private slots:
    void updateTimeDisplay();
private:
    void updateUI();
    void tryMove(int i, int j);

    int m_buttonWidth;  // 新增成员变量存储按钮尺寸
    int m_buttonHeight;
    Ui::GamePageOne *ui;
    GameLogicOne* logic;
    QWidget* centralWidget;
    QGridLayout* gridLayout;
    QVector<TileButton*> tiles;
    GameSessionData session;
    QTimer* gameTimer;
    QString currentSeconds;
    QString currentSteps;
    SlidingSidebar* recordBar;
    SlidingSidebar* targetBar;
    Level level;

    void initRecordSlidingSidebar();
    void showTargetText(const QString& text, const QString& origin, const QString& autor);
    void updateStepdisplay();
    void startRecord(Level level);
    void resetRecord();
    void stopRecord();

};

#endif // GAMEPAGEONE_H
