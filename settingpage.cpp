#include "settingpage.h"
#include "ui_settingpage.h"
#include <QDebug>
SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::SettingPage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);

    // 设置样式
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            border: 2px solid #2a6cf6;
            border-radius: 15px;
        }
    )");
    setAttribute(Qt::WA_StyledBackground, true);

    // 设置按钮尺寸
    ui->save->setwh(100, 50);
    ui->cancel->setwh(100, 50);

    // 加阴影效果
    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(50);  // 越大越柔和
    glow->setOffset(0, 0);    // 向四周发散
    glow->setColor(QColor(42, 108, 246, 180));  //制造蓝光晕
    this->setGraphicsEffect(glow);

    //链接事件
    connect(ui->save, &QPushButton::clicked, [=](){
        emit saveBtnClicked();
    });
    connect(ui->cancel, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });
}


SettingPage::~SettingPage()
{
    delete ui;
}
