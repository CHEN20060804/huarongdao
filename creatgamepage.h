#ifndef CREATGAMEPAGE_H
#define CREATGAMEPAGE_H

#include <QWidget>

namespace Ui {
class CreatGamePage;
}

class CreatGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit CreatGamePage(QWidget *parent = nullptr);
    bool saveCustomGame();

    ~CreatGamePage();
signals:
    void saveBtnClicked();
    void cancelBtnClicked();

protected:
    void paintEvent(QPaintEvent*) override;


private:
    Ui::CreatGamePage *ui;

};

#endif // CREATGAMEPAGE_H
