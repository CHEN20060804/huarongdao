#include "gamepagewithai.h"
#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QColor>
#include <QPainterPath>
#include <QtMath>
#include <QDebug>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Widget::Widget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Widget),
    setting(nullptr),
    loading(nullptr),
    creating(nullptr),
    deleting(nullptr)
{
    ui->setupUi(this);

    //先切一次关卡选择页面，避免加载卡顿
    ui->stackedWidget->setCurrentIndex(1); // 切到关卡页
    QApplication::processEvents();         // 强制处理 paint/layout/init
    ui->stackedWidget->setCurrentIndex(0); // 切回主页

    setWindowTitle("活字华容道");//设置标题
    setFixedSize(850, 600);
    setMouseTracking(true);

    bgPixmap = QPixmap(":/video/res/bk7.jpg");  // widget.h 里定义 QPixmap bgPixmap;


    ui->stackedWidget->setCurrentIndex(0);
    // 设置定时器，每 16ms 更新一次画面，使动画更流畅
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::updateTrail);
    timer->start(16);

    //处理页面切换和弹窗
    connect(ui->startpage, &StartPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->startpage, &StartPage::rankBtnClicked, this, &Widget::showRank);

    connect(ui->levelpage, &LevelPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->levelpage, &LevelPage::levelSelected, this, &Widget::initGame);

    connect(ui->custompage, &CustomPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->rankpage, &RankPage::mainBtnClicked, this, &Widget::changePage);

    connect(ui->gamepageone, &GamePageOne::mainBtnClicked, this, &Widget::changePage);

    connect(ui->gamepagetwo, &GamePageTwo::mainBtnClicked, this, &Widget::changePage);

    connect(ui->gamepagewithai, &GamePageWithAI::mainBtnClicked, this, &Widget::changePage);

    connect(ui->startpage, &StartPage::closeBtnClicked,this, [=](){close();});

    connect(ui->startpage, &StartPage::settingBtnClicked, this, &Widget::popSettingDialog);

    connect(ui->custompage, &CustomPage::creatBtnClicked, this, &Widget::popCreatingDialog);

    connect(ui->custompage, &CustomPage::loadBtnClicked, this, &Widget::popLoadingDialog);

    connect(ui->custompage, &CustomPage::deleteBtnClicked, this, &Widget::popDeletingDialog);

}

void Widget::paintEvent(QPaintEvent *event)
{
    // 创建 QPainter 对象，准备在窗口上绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 启用平滑的图像缩放

    painter.drawPixmap(0, 0, width(), height(), bgPixmap);

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

void Widget::popSettingDialog()
{
    dialog = new QDialog(this, Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_TranslucentBackground, true);
    dialog->setFixedSize(600, 400);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    setting = new SettingPage(this);

    // 创建QVBoxLayout，并设置为QDialog的布局
    QVBoxLayout* layout = new QVBoxLayout(dialog);  // 让layout的父对象是settingDialog
    layout->addWidget(setting);  // 把SettingPage加到layout里
    layout->setContentsMargins(0, 0, 0, 0); // 去除内边距
    layout->setSpacing(0); // 去除控件之间的间距

    connect(setting, &SettingPage::saveBtnClicked, this, &Widget::saveSetting);
    connect(setting, &SettingPage::cancelBtnClicked, this, [=]() {
        fadeOutAndClose(dialog);
        setting = nullptr;
    });

    // 添加淡入动画
    dialog->setWindowOpacity(0.0);
    QPropertyAnimation* fadeInAnim = new QPropertyAnimation(dialog, "windowOpacity");
    fadeInAnim->setDuration(500);
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);
    fadeInAnim->start(QAbstractAnimation::DeleteWhenStopped);

    dialog->exec();
}

void Widget::popCreatingDialog()
{
    dialog = new QDialog(this, Qt::FramelessWindowHint);
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_TranslucentBackground, true);
    dialog->setFixedSize(600, 400);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    creating = new CreatGamePage(this);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(creating);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    connect(creating, &CreatGamePage::saveBtnClicked, this, &Widget::saveCustomGame);
    connect(creating, &CreatGamePage::cancelBtnClicked, this, [=]() {
        fadeOutAndClose(dialog);
        creating = nullptr;
    });

    dialog->setWindowOpacity(0.0);
    QPropertyAnimation* fadeInAnim = new QPropertyAnimation(dialog, "windowOpacity");
    fadeInAnim->setDuration(500);
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);
    fadeInAnim->start(QAbstractAnimation::DeleteWhenStopped);

    dialog->exec();
}


void Widget::popLoadingDialog()
{
    dialog = new QDialog(this, Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_TranslucentBackground, true);
    dialog->setFixedSize(600, 400);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    loading = new LoadGamePage(this);  // 创建SettingPage
    loading->showOptions();
    // 创建QVBoxLayout，并设置为QDialog的布局
    QVBoxLayout* layout = new QVBoxLayout(dialog);  // 让layout的父对象是settingDialog
    layout->addWidget(loading);  // 把SettingPage加到layout里
    layout->setContentsMargins(0, 0, 0, 0); // 去除内边距
    layout->setSpacing(0); // 去除控件之间的间距

    connect(loading, &LoadGamePage::sureBtnClicked, this, &Widget::loadCustomGame);
    connect(loading, &LoadGamePage::cancelBtnClicked, this, [=]() {
        fadeOutAndClose(dialog);
        loading = nullptr;
    });

    // 添加淡入动画
    dialog->setWindowOpacity(0.0);
    QPropertyAnimation* fadeInAnim = new QPropertyAnimation(dialog, "windowOpacity");
    fadeInAnim->setDuration(500);
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);
    fadeInAnim->start(QAbstractAnimation::DeleteWhenStopped);

    dialog->exec();
}
void Widget::popDeletingDialog()
{

    dialog = new QDialog(this, Qt::FramelessWindowHint);
    dialog->setAttribute(Qt::WA_TranslucentBackground, true);
    dialog->setFixedSize(600, 400);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    deleting = new DeleteGamePage(this);
    deleting->showOptions();

    // 创建QVBoxLayout，并设置为QDialog的布局
    QVBoxLayout* layout = new QVBoxLayout(dialog);  // 让layout的父对象是settingDialog
    layout->addWidget(deleting);  // 把SettingPage加到layout里
    layout->setContentsMargins(0, 0, 0, 0); // 去除内边距
    layout->setSpacing(0); // 去除控件之间的间距

    connect(deleting, &DeleteGamePage::deleteBtnClicked, this, &Widget::deleteCustomGame);
    connect(deleting, &DeleteGamePage::cancelBtnClicked, this, [=]() {
        fadeOutAndClose(dialog);
    });

    // 添加淡入动画
    dialog->setWindowOpacity(0.0);
    QPropertyAnimation* fadeInAnim = new QPropertyAnimation(dialog, "windowOpacity");
    fadeInAnim->setDuration(500);
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);
    fadeInAnim->start(QAbstractAnimation::DeleteWhenStopped);

    dialog->exec();
}
void Widget::saveCustomGame()
{
    if(!creating->saveCustomGame())
    {
        qDebug() << "未能保存";
        return;
    }
}

void Widget::loadCustomGame()
{
    Level level;
    Model model;
    if(!loading->loadCustomGame(level, model))
    {
        return;
    }
    if(model==ONE)
    {
        changePage(4);
        ui->gamepageone->loadLevel(level, true);
        dialog->close();
    }
    else if(model==TWO)
    {
        changePage(5);
        ui->gamepagetwo->loadLevel(level);
        dialog->close();
    }
    else
    {
        changePage(6);
        ui->gamepagewithai->loadLevel(level);
        dialog->close();
    }

}

void Widget::deleteCustomGame()
{
    if(!deleting->deleteCustomGame())
    {
        return;
    }
    deleting->showOptions();
}
void Widget::changePage(int i) const
{
    ui->stackedWidget->fadeToIndex(i);
}

void Widget::saveSetting()
{

}

void Widget::fadeOutAndClose(QDialog* dlg) const {
    QPropertyAnimation* fadeOutAnim = new QPropertyAnimation(dlg, "windowOpacity");
    fadeOutAnim->setDuration(500);
    fadeOutAnim->setStartValue(dlg->windowOpacity());  // 当前透明度
    fadeOutAnim->setEndValue(0.0);

    connect(fadeOutAnim, &QPropertyAnimation::finished, [dlg]() {
        dlg->close();         // 关闭对话框
    });
    fadeOutAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::initGame(int page, int index)
{
    changePage(page);
    Level level = ui->levelpage->callLevelManger().getLevel(index);
    ui->gamepageone->loadLevel(level, false);
}

void Widget::showRank()
{
    changePage(3);
    ui->rankpage->showRecord();
}

Widget::~Widget()
{
    delete ui;
}
