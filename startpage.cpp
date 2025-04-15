#include "startpage.h"
#include "ui_startpage.h"
#include "mainbutton.h"
#include <QPainterPath>
#include <QPainter>
StartPage::StartPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartPage)
{
    ui->setupUi(this);
    setFixedSize(850, 600);
    //创建边角按钮
    MainButton *settingBtn = new MainButton(this, 50, 50);
    MainButton *exitBtn = new MainButton(this, 50, 50);
    int x0 = width() - settingBtn->width() -50 ;
    int y0 = 30;
    settingBtn->move(x0, y0);  // 设置按钮的位置
    settingBtn->setIcon(QIcon(":/video/res/setting.png"));
    settingBtn->setIconSize(QSize(30, 30));

    titleFont = QFont("华文行楷", 52, QFont::Normal);
    titleFont.setItalic(true);
    titleText = "活字华容道";
    titlePosition = QPoint(250, 150);


    int x1 =width() - exitBtn->width() - 50;
    int y1 = 100;
    exitBtn->move(x1, y1);
    exitBtn->setIcon(QIcon(":/video/res/exit.png"));
    exitBtn->setIconSize(QSize(30, 30));

    connect(exitBtn,&QPushButton::clicked,[=]{
        close();
    });

    connect(ui->btnLevel,&QPushButton::clicked, [=](){
        emit mainBtnClicked(1);//发出信号，要求切换到索引为1的界面
    });

    connect(ui->btnCustom,&QPushButton::clicked, [=](){
        emit mainBtnClicked(2);
    });

    connect(ui->btnRank,&QPushButton::clicked, [=](){
        emit mainBtnClicked(3);
    });

    connect(exitBtn,&QPushButton::clicked, [=](){
        emit closeBtnClicked();
    });

    connect(settingBtn, &QPushButton::clicked, [=](){
        emit settingBtnClicked();
    });

}
void StartPage::paintEvent(QPaintEvent* ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); // 文本抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 缩放更平滑

    QPainterPath textPath;
    textPath.addText(titlePosition, titleFont, titleText);

    // 1. 外发光（光晕）
    QPainterPath glowPath = textPath;
    painter.setPen(Qt::NoPen);
    for (int i = 6; i >= 1; --i) {
        QColor glowColor(0, 150, 255, 15);  // 淡蓝光晕
        painter.setBrush(glowColor);
        QTransform t;
        t.scale(1 + i * 0.015, 1 + i * 0.015);
        QPainterPath scaled = t.map(glowPath);
        painter.drawPath(scaled.translated(2, 2));
    }

    // 2. 阴影
    QPainterPath shadowPath = textPath.translated(4, 4);
    painter.setBrush(QColor(0, 0, 0, 160));
    painter.drawPath(shadowPath);

    // 3. 描边
    //QPen outlinePen(QColor(220, 230, 255), 1.5);  // 更柔和的描边色
    QPen outlinePen(QColor(40, 40, 80), 6); // 稍带紫的深蓝色，更能托住亮色渐变
    painter.setPen(outlinePen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(textPath);

    // 4. 填充渐变
    QLinearGradient gradient(titlePosition, QPoint(titlePosition.x(), titlePosition.y() + 100));
    gradient.setColorAt(0.0, QColor(250, 250, 255));     // 顶部轻微发光
    gradient.setColorAt(0.3, QColor(120, 180, 255));     // 中部亮蓝
    gradient.setColorAt(1.0, QColor(60, 30, 120));       // 底部深紫
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawPath(textPath);
}


StartPage::~StartPage()
{
    delete ui;
}
