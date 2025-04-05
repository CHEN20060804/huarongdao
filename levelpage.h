#ifndef LEVELPAGE_H
#define LEVELPAGE_H

#include <QWidget>

namespace Ui {
class LevelPage;
}

class LevelPage : public QWidget
{
    Q_OBJECT

public:
    explicit LevelPage(QWidget *parent = nullptr);
    ~LevelPage();

signals:
    void mainBtnClicked(int i);

private:
    Ui::LevelPage *ui;
};

#endif
