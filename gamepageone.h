#ifndef GAMEPAGEONE_H
#define GAMEPAGEONE_H

#include <QWidget>

namespace Ui {
class GamePageOne;
}

class GamePageOne : public QWidget
{
    Q_OBJECT

public:
    explicit GamePageOne(QWidget *parent = nullptr);
    ~GamePageOne();
signals:
    void mainBtnClicked(int i);

private:
    Ui::GamePageOne *ui;
};

#endif // GAMEPAGEONE_H
