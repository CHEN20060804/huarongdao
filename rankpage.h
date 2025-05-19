#ifndef RANKPAGE_H
#define RANKPAGE_H

#include <QWidget>
#include "FancyTableWidget.h"
namespace Ui {
class RankPage;
}

class RankPage : public QWidget
{
    Q_OBJECT

public:
    explicit RankPage(QWidget *parent = nullptr);
    void showRecord();
    ~RankPage();
signals:
    void mainBtnClicked(int i);
private:
    Ui::RankPage *ui;
    FancyTableWidget* table;
};

#endif // RANKPAGE_H
