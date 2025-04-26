#ifndef CUSTOMPAGE_H
#define CUSTOMPAGE_H

#include <QWidget>
#include "creatgamepage.h"
namespace Ui {
class CustomPage;
}

class CustomPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomPage(QWidget *parent = nullptr);
    ~CustomPage();
signals:
    void mainBtnClicked(int i);
    void creatBtnClicked();
    void loadBtnClicked();
private:
    Ui::CustomPage *ui;
};

#endif // CUSTOMPAGE_H
