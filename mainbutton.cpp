#include "MainButton.h"
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPainterPath>
#include <QTimer>

MainButton::MainButton(QWidget *parent)
    : QPushButton(parent), m_isPressed(false), m_isHovered(false),
    glowAlpha(100), glowIncreasing(true), scaleFactor(1.0), yOffset(0)
{
    setStyleSheet("border: none;");

    // 呼吸灯动画
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainButton::updateGlowEffect);
    timer->start(50);

    // **缩放动画**
    scaleAnimation = new QPropertyAnimation(this, "scaleFactor");
    scaleAnimation->setDuration(150);
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // **跳跃动画**
    jumpAnimation = new QPropertyAnimation(this, "yOffset");
    jumpAnimation->setDuration(200);
    jumpAnimation->setEasingCurve(QEasingCurve::OutQuad);
}

MainButton::~MainButton() {}

void MainButton::setScaleFactor(qreal factor)
{
    scaleFactor = factor;
    update();
}

qreal MainButton::getScaleFactor() const
{
    return scaleFactor;
}

void MainButton::setYOffset(int offset)
{
    yOffset = offset;
    update();
}

int MainButton::getYOffset() const
{
    return yOffset;
}

void MainButton::updateGlowEffect()
{
    glowAlpha += glowIncreasing ? 2 : -2;
    if (glowAlpha >= 140) {
        glowAlpha = 140;
        glowIncreasing = false;
    } else if (glowAlpha <= 80) {
        glowAlpha = 80;
        glowIncreasing = true;
    }
    update();
}

void MainButton::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;

    // **放大 & 跳跃**
    scaleAnimation->setStartValue(scaleFactor);
    scaleAnimation->setEndValue(1.5);
    scaleAnimation->start();

    jumpAnimation->setStartValue(yOffset);
    jumpAnimation->setEndValue(-10); // ✨ 让按钮跳跃得更明显
    jumpAnimation->start();

    update();
    QPushButton::enterEvent(event);
}

void MainButton::leaveEvent(QEvent *event)
{
    m_isHovered = false;

    // **恢复原大小 & 回到原位**
    scaleAnimation->setStartValue(scaleFactor);
    scaleAnimation->setEndValue(1.0);
    scaleAnimation->start();

    jumpAnimation->setStartValue(yOffset);
    jumpAnimation->setEndValue(0);
    jumpAnimation->start();

    update();
    QPushButton::leaveEvent(event);
}

void MainButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // **修正绘制偏移，确保跳跃生效**
    painter.save();
    painter.translate(width() / 2, height() / 2 + yOffset);
    painter.scale(scaleFactor, scaleFactor);
    painter.translate(-width() / 2, -height() / 2);

    int radius = 12;
    QRect buttonRect = rect().adjusted(4, 4, -4, -4);

    // **1. 按钮背景**
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(50, 50, 65));
    gradient.setColorAt(0.5, QColor(40, 40, 55));
    gradient.setColorAt(1, QColor(30, 30, 50));

    QPainterPath buttonPath;
    buttonPath.addRoundedRect(buttonRect, radius, radius);
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawPath(buttonPath);

    // **2. 内发光**
    QRadialGradient innerGlow(rect().center(), width() * 0.6);
    int adjustedAlpha = m_isHovered ? glowAlpha + 20 : glowAlpha;
    innerGlow.setColorAt(0.3, QColor(120, 160, 255, adjustedAlpha / 2));
    innerGlow.setColorAt(0.6, QColor(80, 120, 255, adjustedAlpha / 3));
    innerGlow.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setBrush(innerGlow);
    painter.drawPath(buttonPath);

    // **3. 细腻边缘光**
    QPainterPath borderPath;
    borderPath.addRoundedRect(rect().adjusted(-1, -1, 1, 1), radius, radius);
    painter.setPen(QPen(QColor(100, 150, 255, adjustedAlpha + 40), 2));
    painter.drawPath(borderPath);

    // **4. 按钮文本**
    painter.setPen(QPen(Qt::white));
    painter.setFont(QFont("楷体", 16, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, text());

    painter.restore();
}

void MainButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void MainButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}
