#ifndef GAMEPAGEONE_H
#define GAMEPAGEONE_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include "gamelogicone.h"
#include "levelmaneger.h"

namespace Ui {
class GamePageOne;
}

class GamePageOne : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageOne(QWidget *parent = nullptr);
    ~GamePageOne();

    void loadLevel(Level level);

signals:
    void mainBtnClicked(int);

private:
    void updateUI();
    void tryMove(int i, int j);

    Ui::GamePageOne *ui;
    GameLogicOne* logic;
    QWidget* centralWidget;
    QGridLayout* gridLayout;
    QVector<QPushButton*> tiles;
};

#endif // GAMEPAGEONE_H
