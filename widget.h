#ifndef WIDGET_H
#define WIDGET_H

#include <QList>
#include <QPoint>
#include <QWidget>
#include <QTimer>
#include <QDialog>
#include <QPixmap>
#include "settingpage.h"
#include "creatgamepage.h"
#include "loadgamepage.h"
#include "deletegamepage.h"
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}

QT_END_NAMESPACE
// 结构体：表示拖尾点，包含位置和透明度
struct TrailPoint {
    QPoint pos;
    int alpha; // 透明度，决定点的消失速度
};

// 结构体：表示水波效果，包含中心位置和半径
struct RippleEffect {
    QPoint center;
    int radius; // 水波扩散半径
};

//所有的页面切换，弹窗处理都交给这个类
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:

    void paintEvent(QPaintEvent *event) override; // 处理绘制事件

private slots:
   // void updateTrail(); // 更新拖尾和水波的定时器槽函数
    void changePage(int i) const;
    void popSettingDialog();//弹出设置对话框
    void popCreatingDialog();//弹出创建对话框
    void popLoadingDialog();//弹出载入对话框
    void popDeletingDialog();//弹出删除对话框
    void saveSetting();
    void initGame(int page, int index);
    void saveCustomGame();
    void loadCustomGame();
    void deleteCustomGame();
    void showRank();
    void playResourceMusic(const QString &qrcPath);

private:
    Ui::Widget *ui;
    QMediaPlayer* music;
    QAudioOutput* audioOutput;
    SettingPage* setting;
    CreatGamePage* creating;
    LoadGamePage* loading;
    DeleteGamePage* deleting;
    QDialog* dialog;



    QPixmap bgPixmap;

    void fadeOutAndClose(QDialog* dlg) const;//淡出对话框并关闭
};
#endif // WIDGET_H
