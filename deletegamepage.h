#ifndef DELETEGAMEPAGE_H
#define DELETEGAMEPAGE_H

#include <QWidget>
#include <QListWidget>
namespace Ui {
class DeleteGamePage;
}

class DeleteGamePage : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteGamePage(QWidget *parent = nullptr);
    bool deleteCustomGame();
    void showOptions();

    ~DeleteGamePage();

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void deleteBtnClicked();
    void cancelBtnClicked();
private:
    Ui::DeleteGamePage *ui;
    QListWidget* listWidget;
};

#endif // DELETEGAMEPAGE_H
