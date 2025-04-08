#ifndef MAINBUTTON_H
#define MAINBUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSequentialAnimationGroup>

class MainButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(double glowStrength READ glowStrength WRITE setGlowStrength)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor)

public:
    explicit MainButton(QWidget *parent = nullptr, int w = 160, int h = 60);
    ~MainButton();

    double glowStrength() const;
    void setGlowStrength(double strength);

    double scaleFactor() const;
    void setScaleFactor(double factor);
    void setwh(int i, int j);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void startBreathingEffect();  // 启动呼吸效果

    QGraphicsDropShadowEffect *shadowEffect;  // 外发光
    QPropertyAnimation *hoverGlowAnim;  // 鼠标悬停时的光效动画
    QPropertyAnimation *hoverScaleAnim; // 悬停时放大效果
    QTimer *breathingTimer;  // 定时器，用于定时触发呼吸效果
    double m_glowStrength;
    double m_scaleFactor;
    int m_h;
    int m_w;

    bool breathingEnabled = true;
};

#endif // MAINBUTTON_H
