#ifndef STARTPAGE_H
#define STARTPAGE_H
#include <QWidget>

namespace Ui {
class StartPage;
}

class StartPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();

signals:
    void mainBtnClicked(int i);
    void closeBtnClicked();
    void settingBtnClicked();
private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H
