#ifndef MAINBUTTON_H
#define MAINBUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

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
    QGraphicsDropShadowEffect *shadowEffect;  // 外发光
    QPropertyAnimation *hoverGlowAnim;  // 鼠标悬停时的光效动画
    QPropertyAnimation *hoverScaleAnim; // 悬停时放大效果
    QPropertyAnimation *breathingAnim;  // 呼吸灯效果
    QTimer *breathingTimer;

    double m_glowStrength;
    double m_scaleFactor;
    int m_h;
    int m_w;
    QPixmap m_cachedIcon;

    void setupUI();
    void startBreathingEffect();
};

#endif // MAINBUTTON_H
