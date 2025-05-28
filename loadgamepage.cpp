#include "customgamemaneger.h"
#include "ui_loadgamepage.h"
#include "loadgamepage.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QVBoxLayout>
#include <QRegularExpression>
#include "poptips.h"

LoadGamePage::LoadGamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoadGamePage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);

    ui->sureBtn->setwh(160, 50);
    ui->sureBtn->setIcon(QIcon(":/video/res/loading.png"));
    ui->sureBtn->setIconSize(QSize(40, 40));


    ui->cancelBtn->setwh(50, 50);
    ui->cancelBtn->setIcon(QIcon(":/video/res/close.png"));
    ui->cancelBtn->setIconSize(QSize(30, 30));
    connect(ui->sureBtn, &QPushButton::clicked, [=](){
        emit sureBtnClicked();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });

    listWidget = new QListWidget(this);  // 创建 QListWidget 控件
    listWidget->setSpacing(4);  // 每个 item 之间留 4px
    listWidget->setGeometry(150, 30, 300, 215);  // 参数依次是 x, y, width, height
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
    spacing: 8px;
    font: 10pt "Segoe UI";
    color: #EEEEEE;
}

QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border-radius: 9px;
    border: 2px solid #777777;
    background: transparent;
}

QRadioButton::indicator:hover {
    border: 2px solid #2A6CF6;
}

QRadioButton::indicator:checked {
    background-color: #2A6CF6;
    border: 2px solid #2A6CF6;
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
    QPen pen(QColor(246, 246, 246));  // 蓝色边框
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


bool LoadGamePage::loadCustomGame(Level& level, Model& model)
{
    QListWidgetItem* currentItem = listWidget->currentItem();
    if (!currentItem)
    {
        PopTips::Bad(this, listWidget, "请先选择一个自定义棋盘");
        return false;
    }

    QString selectedTitle = currentItem->text();
    QStringList stream = CustomGameManeger::getInstance()->loadGameFromFile(selectedTitle);

    if (stream.size() != 2 || stream[0].isEmpty() || stream[1].isEmpty())
    {
        PopTips::Bad(this, listWidget, "自定义关卡数据格式错误（缺少必要的信息）");

        return false;
    }

    QStringList domain = stream[0].split(' ', Qt::SkipEmptyParts);
    if (domain.size() != 2)
    {
        PopTips::Bad(this, listWidget, "关卡尺寸数据格式错误");
        return false;
    }

    bool okH = false, okW = false;
    int h = domain[0].toInt(&okH);
    int w = domain[1].toInt(&okW);
    if (!okH || !okW || h < 3 || w < 3 || h > 6 || w > 6)
    {
        PopTips::Bad(this, listWidget, "关卡尺寸数据非法");
        return false;
    }

    QString content = stream[1];

    // 检查非法空格
    static QRegularExpression badSpaceRe("[ \\x{3000}]");
    if (content.contains(badSpaceRe))
    {
        PopTips::Bad(this, listWidget, "关卡包含非法空格");

        return false;
    }

    // 检查 {} 配对，并计算真实元素数量
    int realElementCount = 0;
    for (int i = 0; i < content.size(); ) {
        if (content[i] == '{') {
            int end = content.indexOf('}', i);
            if (end != -1) {
                QString inner = content.mid(i + 1, end - i - 1);
                if (inner.isEmpty()) {
                    PopTips::Bad(this, listWidget, "关卡内容格式错误（空的 {} 不合法）");

                    return false;
                }
                if (inner.contains('{') || inner.contains('}')) {
                    PopTips::Bad(this, listWidget, "关卡内容格式错误（嵌套 {} 不合法）");

                    return false;
                }
                realElementCount++;
                i = end + 1;
            } else {
                PopTips::Bad(this, listWidget, "关卡内容格式错误（缺少 } ）");

                return false;
            }
        } else if (content[i] == '}') {
            PopTips::Bad(this, listWidget, "关卡内容格式错误（孤立的 } ）");
            return false;
        } else {
            realElementCount++;
            ++i;
        }
    }

    // 检查元素数量
    if (realElementCount != h * w - 1)
    {
        PopTips::Bad(this, listWidget,  QString("关卡内容元素数量错误（需要 %1 个元素，实际 %2 个）").arg(h*w-1).arg(realElementCount));

        return false;
    }

    if (!level.initForCustom(w, h, content))
    {
        PopTips::Bad(this, listWidget, "关卡未能初始化！");
        return false;
    }
    // 成功加载
    if(ui->radioBtnOne->isChecked())
    {
        model = ONE;
    }
    else if(ui->radioBtnTwo->isChecked())
    {
        model = TWO;
    }
    else if(ui->radioBtnAI->isChecked())
    {
        model = AI;
    }
    else
    {
        PopTips::Bad(this, listWidget, "请选择模式");
        return false;
    }
    return true;
}

LoadGamePage::~LoadGamePage()
{
    delete ui;
}
