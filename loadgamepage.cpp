#include "customgamemaneger.h"
#include "ui_loadgamepage.h"
#include "loadgamepage.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
LoadGamePage::LoadGamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoadGamePage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);

    ui->sureBtn->setwh(100, 50);
    ui->cancelBtn->setwh(100, 50);
    connect(ui->sureBtn, &QPushButton::clicked, [=](){
        emit sureBtnClicked();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });

    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(50);  // 越大越柔和
    glow->setOffset(0, 0);    // 向四周发散
    glow->setColor(QColor(42, 108, 246, 180));  //制造蓝光晕
    this->setGraphicsEffect(glow);

    listWidget = new QListWidget(this);  // 创建 QListWidget 控件
    listWidget->setSpacing(4);  // 每个 item 之间留 4px
    listWidget->setGeometry(150, 50, 300, 180);  // 参数依次是 x, y, width, height
    listWidget->setStyleSheet(R"(
QListWidget {
    background-color: #2E2E2E;
    border: 1px solid #555555;
    border-radius: 10px;
    padding: 10px;
    color: white;
    font: bold 12pt 'Segoe UI';
    outline: 0;    /* ← 去掉默认的焦点虚线 */
}

/* 普通状态下每个 item 都有一条暗边 */
QListWidget::item {
    background-color: #2D2D2D;
    border: 1px solid #444444;    /* ← 加上边框 */
    border-radius: 5px;
    padding: 8px;
    color: white;
    font: bold 11pt 'Segoe UI';
}

/* 选中状态下，高亮边框和背景 */
QListWidget::item:selected {
    background-color: #2A6CF6;
    border: 1px solid #1E4AB3;
    color: white;
}
)");

    this->setStyleSheet(R"(
QRadioButton {
    spacing: 8px;                   /* 文本与圆点的间距 */
    font: 10pt "Segoe UI";          /* 现代字体 */
    color: #EEEEEE;                 /* 文字颜色 */
}

QRadioButton::indicator {
    width: 18px;                    /* 圆点大小 */
    height: 18px;
    border-radius: 9px;             /* 圆形 */
    border: 2px solid #777777;      /* 灰色边框 */
    background: transparent;
}

QRadioButton::indicator:hover {
    border: 2px solid #2A6CF6;      /* 悬停时边框高亮 */
}

QRadioButton::indicator:checked {
    background-color: #2A6CF6;      /* 选中时填充蓝色 */
    border: 2px solid #2A6CF6;      /* 选中时边框同色 */
}

QRadioButton::indicator:disabled {
    border: 2px solid #444444;
    background: #333333;
}

QRadioButton:disabled {
    color: #555555;
}
)");

    ui->radioBtnOne->setFocusPolicy(Qt::NoFocus);
    ui->radioBtnTwo->setFocusPolicy(Qt::NoFocus);
    ui->radioBtnAI->setFocusPolicy(Qt::NoFocus);

}

void LoadGamePage::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    // 创建绘制对象
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿

    // 绘制背景（黑色，带圆角）
    painter.setBrush(QColor("#1e1e1e"));  // 设置背景色
    painter.setPen(Qt::NoPen);  // 不绘制边框
    painter.drawRoundedRect(rect(), 15, 15);  // 绘制圆角背景

    // 绘制蓝色圆角边框
    QPen pen(QColor(42, 108, 246));  // 蓝色边框
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);  // 不填充，只有边框
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 15, 15); // 圆角边框

}

void LoadGamePage::showOptions()
{
    QStringList items = CustomGameManeger::getInstance()->getallTitles();
    for (int i = 0; i < items.size(); i++)
    {
        listWidget->addItem(items[i]);
    }

}


bool LoadGamePage::loadCustomGame(Level& level)
{
    QListWidgetItem* currentItem = listWidget->currentItem();
    if (!currentItem)
    {
        QMessageBox::warning(this, "提示", "请先选择一个自定义关卡！");
        return false;
    }

    QString selectedTitle = currentItem->text();
    QStringList stream = CustomGameManeger::getInstance()->loadGameFromFile(selectedTitle);

    if (stream.size() != 2)
    {
        QMessageBox::critical(this, "加载错误", "自定义关卡数据格式错误（缺少必要的信息）！");
        return false;
    }

    QStringList domain = stream[0].split(' ', Qt::SkipEmptyParts);
    if (domain.size() != 2)
    {
        QMessageBox::critical(this, "加载错误", "关卡尺寸数据格式错误！");
        return false;
    }

    bool okH = false, okW = false;
    int h = domain[0].toInt(&okH);
    int w = domain[1].toInt(&okW);
    if (!okH || !okW || h <= 0 || w <= 0)
    {
        QMessageBox::critical(this, "加载错误", "关卡尺寸数据非法（必须是正整数）！");
        return false;
    }

    QString content = stream[1];

    // 内容基本合法性检查（这里可以按你的元素规则进一步细化）
    if (content.contains(QRegularExpression("[ \\x{3000}]")))
    {
        QMessageBox::critical(this, "加载错误", "关卡内容中包含非法空格！");
        return false;
    }

    // 成功加载
    if(!level.initForCustom(w, h, content))
    {
        QMessageBox::critical(this, "加载错误", "关卡未能初始化！");
        return false;
    }
    return true;
}

LoadGamePage::~LoadGamePage()
{
    delete ui;
}
