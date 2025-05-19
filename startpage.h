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
    void rankBtnClicked();
protected:
    void paintEvent(QPaintEvent* ev) override;
private:
    Ui::StartPage *ui;

    QPoint titlePosition;
    QString titleText;
    QFont titleFont;
    QColor titleColor;
};

#endif // STARTPAGE_H
