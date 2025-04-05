#ifndef CUSTOMPAGE_H
#define CUSTOMPAGE_H

#include <QWidget>

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
private:
    Ui::CustomPage *ui;
};

#endif // CUSTOMPAGE_H
