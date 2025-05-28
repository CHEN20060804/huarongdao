#include "mainstackedwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QColor>
#include <QPainterPath>
#include <QtMath>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include "settingmanager.h"
MainStackedWidget::MainStackedWidget(QWidget *parent)
    : QStackedWidget{parent}
{
    setFixedSize(850, 600);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainStackedWidget::updateTrail);
    timer->start(20);
    qApp->installEventFilter(this);
}
void MainStackedWidget::paintEvent(QPaintEvent *event)
{
    rgb = SettingManager::getInstance()->getRGB();
    QStackedWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 启用平滑的图像缩放
    // 绘制渐变流畅的拖尾效果
    for (int i = 0; i < trailPoints.size(); ++i) {
        float factor = static_cast<float>(i) / trailPoints.size(); // 计算渐变因子，确保靠近鼠标的部分最大
        int alpha = 155 * (1 - factor); // 透明度逐渐降低
        int size = 6 * (1 - factor) + 4; // 从大到小渐变
        QColor color(rgb.r/3, rgb.g/10 + (1 - factor) * 50, rgb.b - (1 - factor) * 100, alpha);
        painter.setBrush(QBrush(color, Qt::SolidPattern));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(trailPoints[i].pos, size, size);
    }

    // 绘制柔和的水波纹效果
    for (const auto &wave : rippleEffects) {
        int alpha = 255 - wave.radius * 2; // 透明度随半径减小
        if (alpha < 0) alpha = 0;
        QColor color(rgb.r/6, rgb.g/3, rgb.b/3, alpha);
        painter.setPen(QPen(color, 5)); // 细边框增强渐变感
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(wave.center, wave.radius, wave.radius);
    }

}
void MainStackedWidget::mouseMoveEvent(QMouseEvent *event) {
    QStackedWidget::mouseMoveEvent(event);
   // qDebug() << "Mouse Move Position: " << event->pos();
    // 添加新的轨迹点，并限制长度，防止拖尾过长
    trailPoints.prepend({event->pos(), 255}); // 新轨迹点插入到前面，保持靠近鼠标处最大
    if (trailPoints.size() > maxTrailLength) {
        trailPoints.pop_back(); // 过长时删除最远端的点
    }
    update();
}
void MainStackedWidget::mousePressEvent(QMouseEvent *event) {
    // 添加水波纹效果，初始半径为 0
    rippleEffects.append({event->pos(), 0});
    update();
}

void MainStackedWidget::updateTrail() {
    // 逐渐减少拖尾点数量，使拖尾变短
    if (!trailPoints.empty()) {
        trailPoints.pop_back(); // 移除最远端的点，使靠近鼠标处更明显
    }
    // 让水波纹扩散并更加丝滑
    for (int i = 0; i < rippleEffects.size(); ++i) {
        rippleEffects[i].radius += 5; // 适当降低增速，确保流畅
        if (rippleEffects[i].radius > rippleMaxSize) {
            rippleEffects.removeAt(i);
            --i;
        }
    }
    update();
}

//绕过了 Qt 的控件层级事件传递机制,终于解决了setMouseTracking(true)没有解决的问题
bool MainStackedWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseMove) {
        if (obj != this && qobject_cast<QWidget*>(obj)) {
            return QStackedWidget::eventFilter(obj, event);
        }
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // 将全局坐标转换为当前控件的本地坐标
        QPoint globalPos = mouseEvent->globalPosition().toPoint();
        QPoint localPos = mapFromGlobal(globalPos);

        // 判断鼠标是否在控件区域内
        if (rect().contains(localPos)) {
            // 直接处理鼠标移动逻辑
            trailPoints.prepend({localPos, 255});
            if (trailPoints.size() > maxTrailLength) {
                trailPoints.pop_back();
            }
            update();
            //qDebug() << "Mouse Move (Global Filter):" << localPos;
        }
    }
    return QStackedWidget::eventFilter(obj, event);
}

void MainStackedWidget::fadeToIndex(int newIndex, int duration) {
    int oldIndex = currentIndex();
    if (oldIndex == newIndex) return;

    QWidget *oldPage = widget(oldIndex);
    QWidget *newPage = widget(newIndex);

    // 1. 将旧页面渲染到 pixmap
    QPixmap snapshot(oldPage->size());
    oldPage->render(&snapshot);

    // 2. 切换到新页面
    setCurrentIndex(newIndex);

    // 3. 在新页面之上创建一个 QLabel 作为遮罩
    QLabel *overlay = new QLabel(this);
    overlay->setPixmap(snapshot);
    overlay->setFixedSize(snapshot.size());
    overlay->move(0, 0);
    overlay->show();

    // 4. 给 overlay 添加透明度效果
    auto *effect = new QGraphicsOpacityEffect(overlay);
    overlay->setGraphicsEffect(effect);

    // 5. 创建淡出动画
    auto *anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setDuration(duration);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);

    connect(anim, &QPropertyAnimation::finished, [overlay]() {
        overlay->deleteLater();  // 动画结束后移除遮罩
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

