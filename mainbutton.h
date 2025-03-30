#ifndef MODERNBUTTON_H
#define MODERNBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
class MainButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MainButton(QWidget *parent = nullptr);
    ~MainButton();

    void paintEvent(QPaintEvent *event) override;      // 绘制按钮
    void enterEvent(QEnterEvent *event) override;           // 鼠标进入按钮时
    void leaveEvent(QEvent *event) override;           // 鼠标离开按钮时
    void mousePressEvent(QMouseEvent *event) override; // 按钮按下时
    void mouseReleaseEvent(QMouseEvent *event) override; // 按钮释放时
    void setScaleFactor(qreal factor);
    qreal getScaleFactor() const;
    void updateGlowEffect();
    void setYOffset(int offset);
    int getYOffset() const;

protected:
    bool m_isPressed;
    bool m_isHovered;
    int glowAlpha;          // 发光的透明度
    bool glowIncreasing;    // 控制发光透明度是增加还是减少
    qreal scaleFactor;                // 按钮的缩放因子
    int yOffset;                      // 按钮的 Y 轴偏移（用于跳跃）
    QPropertyAnimation *scaleAnimation;  // 控制按钮放大缩小的动画
    QPropertyAnimation *jumpAnimation;   // 控制按钮跳跃的动画

};

#endif
