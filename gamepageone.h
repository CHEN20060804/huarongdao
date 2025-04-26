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

    void loadLevel(const Level& level, bool isCustom);

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
    bool isCustom;

    void initRecordSlidingSidebar();
    void showTargetText(const QString& text);
    void updateStepdisplay();
    void startRecord(Level level);
    void resetRecord();
    void stopRecord();
    void setTitle(int ID);
    void youWin();
};

#endif // GAMEPAGEONE_H
