#ifndef GAMEPAGEONE_H
#define GAMEPAGEONE_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include <QTimer>
#include <QTextBrowser>
#include "gamelogicone.h"
#include "levelmaneger.h"
#include "tilebutton.h"
#include "slidingsidebar.h"
#include "gamesessiondata.h"
#include <memory>
#include "man.h"



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

    Ui::GamePageOne *ui;
    int m_buttonWidth;
    int m_buttonHeight;
    std::unique_ptr<GameLogicOne> logic;
    QWidget* centralWidget;
    QGridLayout* gridLayout;
    QVector<TileButton*> tiles;
    GameSessionData session;
    QTimer* gameTimer;
    QString currentSeconds;
    QString currentSteps;
    SlidingSidebar* recordBar;
    SlidingSidebar* targetBar;
    QTextBrowser* customBrowser;
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
    void createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry = {0, 152, 160, 250});

};

#endif // GAMEPAGEONE_H
