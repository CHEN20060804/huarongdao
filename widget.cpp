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
#include "settingmanager.h"
#include "poptips.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
Widget::Widget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Widget),
    setting(nullptr),
    creating(nullptr),
    loading(nullptr),
    deleting(nullptr),
    music(new QMediaPlayer(this)),
    audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);

    //先切一次关卡选择页面，避免加载卡顿
    ui->stackedWidget->setCurrentIndex(1); // 切到关卡页
    QApplication::processEvents();
    ui->stackedWidget->setCurrentIndex(0); // 切回主页

    setWindowTitle("活字华容道");//设置标题
    setFixedSize(850, 600);
    setMouseTracking(true);


    QString bgph = SettingManager::getInstance()->getBGPH();
    QString phpath = QString(":/video/res/%1.jpg").arg(bgph);
    qDebug() << phpath;

    music->setAudioOutput(audioOutput);
    QString bgm = SettingManager::getInstance()->getBGM();
    qDebug() << bgm;

    QTimer::singleShot(0, this, [=]() {
        playResourceMusic(bgm);
    });


    connect(music, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia) {
            music->setPosition(0); // 回到开头
            music->play();         // 再播一次
        }
    });

    bgPixmap = QPixmap(phpath);

    ui->stackedWidget->setCurrentIndex(0);

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

    connect(setting, &SettingPage::changeBKPH, this, [=](QString ph)
    {
        QString path = QString(":/video/res/%1.jpg").arg(ph);
        bgPixmap = QPixmap(path);
        update();
    });
    connect(setting, &SettingPage::changeBGM, this, &Widget::playResourceMusic);
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
void Widget::playResourceMusic(const QString &bgm)
{
    if(bgm == "wu")
    {
        music->stop();
        music->setSource(QUrl());
        return;
    };
    QString gpath = QString(":/video/res/%1.mp3").arg(bgm);
    QFile resFile(gpath);
    if (!resFile.open(QIODevice::ReadOnly)) {
        qWarning() << "打开资源失败:" << resFile.fileName();
        return;
    }
    QByteArray data = resFile.readAll();
    resFile.close();

    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (!QDir().exists(tempDir))
        QDir().mkpath(tempDir);

    QString tempFilePath = tempDir + "/temp_music.mp3";

    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        qWarning() << "写入临时文件失败:" << tempFilePath;
        return;
    }
    tempFile.write(data);
    tempFile.close();

    music->setAudioOutput(audioOutput);
    music->setSource(QUrl::fromLocalFile(tempFilePath));

    QTimer::singleShot(0, this, [=]() {
        music->stop();
        music->play();
    });
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
    SettingManager::getInstance()->saveSettings();
    PopTips::GoodCenter(dialog, "设置保存成功");
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
