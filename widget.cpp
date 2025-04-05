#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QColor>
#include <QPainterPath>
#include <QtMath>
#include <QDebug>
Widget::Widget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("活字华容道");//设置标题
    setFixedSize(850, 600);
    setMouseTracking(true);

    ui->stackedWidget->setCurrentIndex(0);
    // 设置定时器，每 16ms 更新一次画面，使动画更流畅
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::updateTrail);
    timer->start(16);

    connect(ui->startpage, &StartPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->levelpage, &LevelPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->custompage, &CustomPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->rankpage, &RankPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->gamepageone, &GamePageOne::mainBtnClicked, this, &Widget::changePage);

    connect(ui->startpage, &StartPage::closeBtnClicked,this, [=](){
        close();
    });

}

void Widget::paintEvent(QPaintEvent *event)
{

    // 创建 QPixmap 对象，加载图片
    QPixmap pixmap(":/video/res/bk1.jpg");
    // 创建 QPainter 对象，准备在窗口上绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 启用平滑的图像缩放
    // 将 QPixmap 绘制到窗口的指定位置（例如左上角）
    painter.drawPixmap(0, 0, width(), height(), pixmap);

    // 绘制渐变流畅的拖尾效果
    for (int i = 0; i < trailPoints.size(); ++i) {
        float factor = static_cast<float>(i) / trailPoints.size(); // 计算渐变因子，确保靠近鼠标的部分最大
        int alpha = 155 * (1 - factor); // 透明度逐渐降低
        int size = 6 * (1 - factor) + 4; // 从大到小渐变
        QColor color(0, 50 + (1 - factor) * 50, 255 - (1 - factor) * 100, alpha);
        painter.setBrush(QBrush(color, Qt::SolidPattern));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(trailPoints[i].pos, size, size);
    }

    // 绘制柔和的水波纹效果
    for (const auto &wave : rippleEffects) {
        int alpha = 255 - wave.radius * 2; // 透明度随半径减小
        if (alpha < 0) alpha = 0;
        QColor color(0, 50, 255, alpha);
        painter.setPen(QPen(color, 5)); // 细边框增强渐变感
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(wave.center, wave.radius, wave.radius);
    }

}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    // 添加新的轨迹点，并限制长度
    trailPoints.prepend({event->pos(), 255}); // 新轨迹点插入到前面，保持靠近鼠标处最大
    if (trailPoints.size() > maxTrailLength) {
        trailPoints.pop_back(); // 过长时删除最远端的点
    }
    update();
}

void Widget::mousePressEvent(QMouseEvent *event) {
    // 添加水波纹效果，初始半径为 0
    rippleEffects.append({event->pos(), 0});
    update();
}

void Widget::updateTrail() {
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

void Widget::changePage(int i)
{
    ui->stackedWidget->setCurrentIndex(i);
}

Widget::~Widget()
{
    delete ui;
}
