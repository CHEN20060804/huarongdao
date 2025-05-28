#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>
#include <QMap>
namespace Ui {
class SettingPage;
}

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage();
protected:
    void paintEvent(QPaintEvent* event);
signals:
    void saveBtnClicked();
    void cancelBtnClicked();
    void changeBKPH(QString s);
    void changeBGM(QString s);


private:
    Ui::SettingPage *ui;
    QMap<QString, QString> bgphMap;
    QMap<QString, QString> bgmMap;

};

#endif // SETTINGPAGE_H
