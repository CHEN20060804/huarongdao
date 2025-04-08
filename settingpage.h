#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>

namespace Ui {
class SettingPage;
}

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage();
signals:
    void saveBtnClicked();
    void cancelBtnClicked();

private:
    Ui::SettingPage *ui;
};

#endif // SETTINGPAGE_H
