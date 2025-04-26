#ifndef LOADGAMEPAGE_H
#define LOADGAMEPAGE_H

#include <QWidget>
#include <QListWidget>
#include "leveldata.h"

namespace Ui {
class LoadGamePage;
}

class LoadGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit LoadGamePage(QWidget *parent = nullptr);
    void showOptions();
    bool loadCustomGame(Level& level);
    ~LoadGamePage();
signals:
    void sureBtnClicked();
    void cancelBtnClicked();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::LoadGamePage *ui;
    QListWidget* listWidget;
};

#endif // LOADGAMEPAGE_H
