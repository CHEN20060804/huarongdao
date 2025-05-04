#ifndef GAMEPAGETWO_H
#define GAMEPAGETWO_H

#include <QWidget>
#include "gamelogicone.h"
namespace Ui {
class GamePageTwo;
}

class GamePageTwo : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageTwo(QWidget *parent = nullptr);
    ~GamePageTwo();

signals:
    void mainBtnClicked(int i);
private:
    Ui::GamePageTwo *ui;
};

#endif // GAMEPAGETWO_H
