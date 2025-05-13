#ifndef MAN_H
#define MAN_H

#include <QObject>
#include "tilebutton.h"
#include <gamelogicone.h>
#include "leveldata.h"
#include "slidingsidebar.h"
#include "gamesessiondata.h"
#include <QTimer>
class Man : public QObject {
    Q_OBJECT

protected:
    int buttonWidth = 0;
    int buttonHeight = 0;

    std::unique_ptr<GameLogicOne> logic;
    QVector<TileButton*> tiles;
    SlidingSidebar* recordBar;
    GameSessionData session;
    QTimer* gameTimer;

    explicit Man(QObject* parent = nullptr) : QObject(parent), logic(nullptr), recordBar(nullptr)
    {}

    virtual void initBoard(const Level& level, QWidget* parentWidget) = 0;
    virtual void tryMove(int i, int j) = 0;
    virtual  void startRecord(Level level);
    virtual  void updateTimeDisplay();
    virtual  void updateStepdisplay();
    virtual  void resetRecord();
    virtual  void stopRecord();
    virtual void initRecordSlidingSidebar();


public:
    virtual QVector<TileButton*> getTiles();
signals:
    void over(Man* man);
};


#endif // MAN_H
