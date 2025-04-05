#ifndef RANKPAGE_H
#define RANKPAGE_H

#include <QWidget>

namespace Ui {
class RankPage;
}

class RankPage : public QWidget
{
    Q_OBJECT

public:
    explicit RankPage(QWidget *parent = nullptr);
    ~RankPage();
signals:
    void mainBtnClicked(int i);
private:
    Ui::RankPage *ui;
};

#endif // RANKPAGE_H
