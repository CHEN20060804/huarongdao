#include "settingbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QTimer>

SettingButton::SettingButton(QWidget *parent)
    : MainButton(parent)
{
    setFixedSize(50, 50);  // 固定为正方形
}

SettingButton::~SettingButton() {}

void SettingButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // **修正绘制偏移，确保跳跃动画生效**
    painter.save();
    painter.translate(width() / 2, height() / 2 + yOffset);
    painter.scale(scaleFactor, scaleFactor);
    painter.translate(-width() / 2, -height() / 2);

    int radius = 10;
    QRect buttonRect = rect().adjusted(4, 4, -4, -4);

    // **1. 圆角正方形背景**
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(60, 60, 80));
    gradient.setColorAt(0.5, QColor(50, 50, 70));
    gradient.setColorAt(1, QColor(40, 40, 60));

    QPainterPath buttonPath;
    buttonPath.addRoundedRect(buttonRect, radius, radius);
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawPath(buttonPath);

    // **2. 内发光**
    QRadialGradient innerGlow(rect().center(), width() * 0.6);
    int adjustedAlpha = m_isHovered ? glowAlpha + 20 : glowAlpha;
    innerGlow.setColorAt(0.3, QColor(150, 180, 255, adjustedAlpha / 2));
    innerGlow.setColorAt(0.6, QColor(120, 140, 255, adjustedAlpha / 3));
    innerGlow.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setBrush(innerGlow);
    painter.drawPath(buttonPath);

    // **3. 细腻边缘光**
    QPainterPath borderPath;
    borderPath.addRoundedRect(rect().adjusted(-1, -1, 1, 1), radius, radius);
    painter.setPen(QPen(QColor(130, 170, 255, adjustedAlpha + 40), 2));
    painter.drawPath(borderPath);

    // **4. 齿轮图标**
    int gearSize = width() / 2;
    int centerX = width() / 2;
    int centerY = height() / 2;
    int teethCount = 8;
    int outerRadius = gearSize / 2;
    int innerRadius = outerRadius * 0.6;

    QPainterPath gearPath;
    for (int i = 0; i < teethCount; i++)
    {
        qreal angle = (360.0 / teethCount) * i;
        qreal rad = qDegreesToRadians(angle);
        qreal x1 = centerX + qCos(rad) * outerRadius;
        qreal y1 = centerY + qSin(rad) * outerRadius;

        qreal x2 = centerX + qCos(rad) * innerRadius;
        qreal y2 = centerY + qSin(rad) * innerRadius;

        if (i == 0)
            gearPath.moveTo(x1, y1);
        else
            gearPath.lineTo(x1, y1);

        gearPath.lineTo(x2, y2);
    }
    gearPath.closeSubpath();

    painter.setBrush(QColor(220, 220, 220));
    painter.setPen(QPen(Qt::black, 1));
    painter.drawPath(gearPath);

    // **5. 齿轮中心点**
    painter.setBrush(QColor(180, 180, 180));
    painter.drawEllipse(QPoint(centerX, centerY), gearSize / 6, gearSize / 6);

    painter.restore();
}
