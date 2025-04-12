#ifndef LEVELPAGE_H
#define LEVELPAGE_H

#include <QWidget>
#include "levelmaneger.h"

namespace Ui {
class LevelPage;
}

class LevelPage : public QWidget
{
    Q_OBJECT

public:
    explicit LevelPage(QWidget *parent = nullptr);
    const LevelManeger& callLevelManger() const;
    ~LevelPage();

signals:
    void mainBtnClicked(int i);
    void levelSelected(int page, int index);
private:
    Ui::LevelPage *ui;

    LevelManeger levelmaneger;
};

#endif
