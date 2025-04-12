#include "mainbutton.h"
#include <QPainter>
#include <QEvent>
#include <QEasingCurve>
#include <QTimer>
#include <QFont>
MainButton::MainButton(QWidget *parent, int w, int h)
    : QPushButton(parent),
    shadowEffect(new QGraphicsDropShadowEffect(this)),
    hoverGlowAnim(new QPropertyAnimation(this, "glowStrength")),
    hoverScaleAnim(new QPropertyAnimation(this, "scaleFactor")),
    m_glowStrength(0.0),
    m_scaleFactor(1.0),
    m_w(w),
    m_h(h)
{
    setupUI();

    // 启动呼吸灯动画
    breathingTimer = new QTimer(this);
    connect(breathingTimer, &QTimer::timeout, this, &MainButton::startBreathingEffect);

    // 立即启动一次呼吸灯效果
    startBreathingEffect();

    // 设置定时器，每5秒进行一次呼吸灯效果
    breathingTimer->start(5000);

    // 设置按钮字体大小
    setFixedSize(m_w, m_h);
    QFont font = this->font();
    font.setPixelSize(m_h/3);
    setFont(font);

    // 设置样式表
    setStyleSheet(R"(
    QPushButton {
        background-color: #292759;
        font-family: "楷体";
        color: white;
        border-radius: 12px;
        padding: 10px 20px;
        border: 2px solid #15707C;
        text-align: center;
        font-weight: bold;
    }
)");



}


MainButton::~MainButton()
{
    delete shadowEffect;
    delete hoverGlowAnim;
    delete hoverScaleAnim;
    delete breathingTimer;
}

void MainButton::setupUI()
{
    shadowEffect->setBlurRadius(25);
    shadowEffect->setColor(QColor(30, 100, 255, 100));
    shadowEffect->setOffset(0, 0);
    setGraphicsEffect(shadowEffect);
}

void MainButton::startBreathingEffect()
{
    // 呼吸灯效果的动画
    QPropertyAnimation *fadeIn = new QPropertyAnimation(this, "glowStrength");
    fadeIn->setDuration(2500);
    fadeIn->setStartValue(0.1);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "glowStrength");
    fadeOut->setDuration(2500);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.1);
    fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

    QSequentialAnimationGroup *breathingGroup = new QSequentialAnimationGroup(this);
    breathingGroup->addAnimation(fadeIn);
    breathingGroup->addAnimation(fadeOut);
    breathingGroup->setLoopCount(-1); // 无限循环
    breathingGroup->start();
}

void MainButton::enterEvent(QEnterEvent *event)
{
    // 鼠标悬停时的动画：放大和光效变化
    if (hoverGlowAnim->state() == QAbstractAnimation::Running) {
        hoverGlowAnim->stop();
    }
    if (hoverScaleAnim->state() == QAbstractAnimation::Running) {
        hoverScaleAnim->stop();
    }

    hoverGlowAnim->setDuration(300);
    hoverGlowAnim->setStartValue(m_glowStrength);
    hoverGlowAnim->setEndValue(1.5);
    hoverGlowAnim->setEasingCurve(QEasingCurve::OutQuad);
    hoverGlowAnim->start();

    hoverScaleAnim->setDuration(300);
    hoverScaleAnim->setStartValue(m_scaleFactor);
    hoverScaleAnim->setEndValue(1.08);  // 增大按钮
    hoverScaleAnim->setEasingCurve(QEasingCurve::OutQuad);
    hoverScaleAnim->start();

    QPushButton::enterEvent(event);
}

void MainButton::leaveEvent(QEvent *event)
{
    // 鼠标离开时的动画：恢复原状态
    if (hoverGlowAnim->state() == QAbstractAnimation::Running) {
        hoverGlowAnim->stop();
    }
    if (hoverScaleAnim->state() == QAbstractAnimation::Running) {
        hoverScaleAnim->stop();
    }

    hoverGlowAnim->setDuration(300);
    hoverGlowAnim->setStartValue(m_glowStrength);
    hoverGlowAnim->setEndValue(0.5);
    hoverGlowAnim->setEasingCurve(QEasingCurve::InQuad);
    hoverGlowAnim->start();

    hoverScaleAnim->setDuration(300);
    hoverScaleAnim->setStartValue(m_scaleFactor);
    hoverScaleAnim->setEndValue(1.0);  // 恢复原始大小
    hoverScaleAnim->setEasingCurve(QEasingCurve::InQuad);
    hoverScaleAnim->start();

    QPushButton::leaveEvent(event);
}

void MainButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    QRadialGradient gradient(w / 2, h / 2, w / 2);
    gradient.setColorAt(0.0, QColor(50, 200, 220, static_cast<int>(m_glowStrength * 60))); // 中心：青蓝亮色
    gradient.setColorAt(1.0, QColor(10, 40, 50, 0)); // 边缘：深青灰


    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 12, 12);
}

double MainButton::glowStrength() const
{
    return m_glowStrength;
}

void MainButton::setGlowStrength(double strength)
{
    m_glowStrength = strength;
    update();
}

double MainButton::scaleFactor() const
{
    return m_scaleFactor;
}

void MainButton::setScaleFactor(double factor)
{
    m_scaleFactor = factor;
    setFixedSize(m_w * m_scaleFactor, m_h * m_scaleFactor);
}

void MainButton::setwh(int i, int j)
{
    m_w = i;
    m_h = j;
    setFixedSize(m_w, m_h);  // 设置按钮大小

    setFixedSize(m_w, m_h);
    QFont font = this->font();
    font.setPixelSize(m_h/3);
    setFont(font);

    update();
}
