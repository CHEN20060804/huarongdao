#ifndef WIDGET_H
#define WIDGET_H
#include <QList>
#include <QPoint>
#include <QWidget>
#include <QTimer>
#include <QDialog>
#include "settingpage.h"
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

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void mouseMoveEvent(QMouseEvent *event) override; // 处理鼠标移动事件
    void mousePressEvent(QMouseEvent *event) override; // 处理鼠标点击事件
    void paintEvent(QPaintEvent *event) override; // 处理绘制事件

private slots:
    void updateTrail(); // 更新拖尾和水波的定时器槽函数
    void changePage(int i);
    void popSettingDialog();//弹出设置对话框
    void saveSetting();

private:
    Ui::Widget *ui;
    QTimer *timer; // 定时器，用于定期更新动画
    QList<TrailPoint> trailPoints; // 存储拖尾轨迹点
    QList<RippleEffect> rippleEffects; // 存储水波纹效果
    int maxTrailLength = 80; // 限制拖尾最大长度，防止过长
    int rippleMaxSize = 100; // 增加水波纹的最大扩散范围
    SettingPage* setting;
    QDialog* settingDialog;

    void fadeOutAndClose(QDialog* dlg) const;
};
#endif // WIDGET_H
