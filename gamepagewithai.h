#ifndef GAMEPAGEWITHAI_H
#define GAMEPAGEWITHAI_H

#include <QWidget>
#include "slidingsidebar.h"
#include "tilebutton.h"
#include "gamelogicone.h"
#include "leveldata.h"
#include "gamesessiondata.h"
#include <memory>
#include <QTextBrowser>


class GamePageWithAI;

namespace Ui {
class GamePageWithAI;
}

class Man : public QObject {
    Q_OBJECT
    friend class GamePageWithAI;

protected:
    int buttonWidth = 0;
    int buttonHeight = 0;

    std::unique_ptr<GameLogicOne> logic;
    QVector<TileButton*> tiles;
    SlidingSidebar* recordBar;

    explicit Man(QObject* parent = nullptr) : QObject(parent), logic(nullptr), recordBar(nullptr)
    {}

    virtual void initBoard(const Level& level, QWidget* parentWidget) = 0;
    virtual void tryMove(int i, int j) = 0;

signals:
    void over(Man* man);
};

class Player : public Man {
    Q_OBJECT
    friend class GamePageWithAI;
public:
    explicit Player(QObject* parent = nullptr) : Man(parent) {}

private:
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;
};

class AI : public Man {
    Q_OBJECT
    friend class GamePageWithAI;
public:
    explicit AI(QObject* parent = nullptr) : Man(parent) {}

private:
    void initBoard(const Level& level, QWidget* parentWidget) override;
    void tryMove(int i, int j) override;
};

class GamePageWithAI : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageWithAI(QWidget *parent = nullptr);
    void loadLevel(const Level& level);
    ~GamePageWithAI();
signals:
    void mainBtnClicked(int i);
private:

    Ui::GamePageWithAI *ui;


    QTextBrowser* targetText;
    GameSessionData session;
    QTimer* gameTimer;
    Level level;
    Player* player;
     AI* ai;
    QWidget* playerWidget;
    QWidget* AIWidget;

    void winEffect(Man* man);
    void updateUI(Man* man);
    void createCustomTargetDisplay(QWidget* parent, const QStringList& targetList, const QRect& geometry = {290, 20, 270, 165});
};

#endif // GAMEPAGEWITHAI_H
