#ifndef MAINSTACKEDWIDGET_H
#define MAINSTACKEDWIDGET_H

#include <QStackedWidget>
#include <widget.h>
class MainStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainStackedWidget(QWidget *parent = nullptr);
    void fadeToIndex(int index, int duration = 600);

protected:
    void mouseMoveEvent(QMouseEvent *event) override; // 处理鼠标移动事件
    void mousePressEvent(QMouseEvent *event) override; // 处理鼠标点击事件
    void paintEvent(QPaintEvent *event) override; // 处理绘制事件
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void updateTrail(); // 更新拖尾和水波的定时器槽函数
private:
    QTimer *timer; // 定时器，用于定期更新动画
    QList<TrailPoint> trailPoints; // 存储拖尾轨迹点
    QList<RippleEffect> rippleEffects; // 存储水波纹效果
    int maxTrailLength = 80; // 限制拖尾最大长度，防止过长
    int rippleMaxSize = 100; // 增加水波纹的最大扩散范围
};
#endif // MAINSTACKEDWIDGET_H
