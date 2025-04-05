#include "mainbutton.h"
#include <QPainter>
#include <QEvent>
#include <QEasingCurve>

MainButton::MainButton(QWidget *parent, int w, int h)
    : QPushButton(parent),
    shadowEffect(new QGraphicsDropShadowEffect(this)),
    hoverGlowAnim(new QPropertyAnimation(this, "glowStrength")),  // 设置动画目标为当前对象的 glowStrength
    hoverScaleAnim(new QPropertyAnimation(this, "scaleFactor")),   // 设置动画目标为当前对象的 scaleFactor
    breathingAnim(new QPropertyAnimation(this, "glowStrength")),  // 呼吸效果也使用 glowStrength
    breathingTimer(new QTimer(this)),
    m_glowStrength(0.0),
    m_scaleFactor(1.0),
    m_w(w),
    m_h(h)
{
    setupUI();

    // 设置 breathingAnim 的目标属性为 glowStrength
    breathingAnim->setTargetObject(this);
    breathingAnim->setPropertyName("glowStrength");

    // 连接定时器超时信号和 breathingEffect 方法
    connect(breathingTimer, &QTimer::timeout, this, &MainButton::startBreathingEffect);

    // 启动定时器，每5秒触发一次呼吸效果
    breathingTimer->start(5000);

    // 设置按钮大小
    setFixedSize(m_w, m_h);

    // 设置样式表
    setStyleSheet(R"(
    QPushButton {
        background-color: #0F1C3F;  /* 深蓝色 */
        font-family: "楷体";
        color: white;
        border-radius: 12px;
        font-size: 18px;
        padding: 10px 20px;
        border: 2px solid #183A5E;
        text-align: center;
        font-weight: bold; /* 加粗 */
    }
)");
}


MainButton::~MainButton()
{
    delete shadowEffect;
    delete hoverGlowAnim;
    delete hoverScaleAnim;
    delete breathingAnim;
    delete breathingTimer;
}

void MainButton::setupUI()
{

    shadowEffect->setBlurRadius(25);
    shadowEffect->setColor(QColor(30, 100, 255, 100));
    shadowEffect->setOffset(0, 0);
    setGraphicsEffect(shadowEffect);
}

void MainButton::startBreathingEffect() {
    qDebug() << "startBreathingEffect triggered";
    if (breathingAnim->state() == QAbstractAnimation::Running) {
        breathingAnim->stop();  // 停止当前的动画
    }
    breathingAnim->setDuration(5000);  // 增加动画时间，使呼吸效果更为柔和
    breathingAnim->setStartValue(0.3);
    breathingAnim->setEndValue(1.0);
    breathingAnim->setEasingCurve(QEasingCurve::InOutQuad);  // 更平滑的动画曲线
    breathingAnim->setLoopCount(-1);
    breathingAnim->start();
}


void MainButton::enterEvent(QEnterEvent *event)
{
    // 如果动画正在运行，则先停止它们
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
    // 如果动画正在运行，则先停止它们
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
    gradient.setColorAt(0, QColor(50, 100, 255, static_cast<int>(m_glowStrength * 50)));
    gradient.setColorAt(1, QColor(10, 30, 70, 0));

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
    setFixedSize(m_w * m_scaleFactor, m_h* m_scaleFactor);
}


void MainButton::setwh(int i, int j)
{
    m_w = i;
    m_h = j;
    setFixedSize(m_w, m_h);  // 设置按钮大小
    update();
}
