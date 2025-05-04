#include "deletegamepage.h"
#include "customgamemaneger.h"
#include "ui_deletegamepage.h"
#include "poptips.h"
#include <QPainter>
DeleteGamePage::DeleteGamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeleteGamePage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);

    ui->cancelBtn->setwh(50, 50);
    ui->cancelBtn->setIcon(QIcon(":/video/res/close.png"));
    ui->cancelBtn->setIconSize(QSize(30, 30));

    ui->deleteBtn->setwh(160, 50);
    ui->deleteBtn->setIcon(QIcon(":/video/res/delete.png"));
    ui->deleteBtn->setIconSize(QSize(30, 30));

    ui->selectAllBtn->setwh(80, 50);
    connect(ui->deleteBtn, &QPushButton::clicked, [=](){
        emit deleteBtnClicked();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });
    connect(ui->selectAllBtn, &QPushButton::clicked, [=](){
        for (int i = 0; i < listWidget->count(); ++i) {
            listWidget->item(i)->setSelected(true);
        }
    });

    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(50);  // 越大越柔和
    glow->setOffset(0, 0);    // 向四周发散
    glow->setColor(QColor(42, 108, 246, 180));  //制造蓝光晕
    this->setGraphicsEffect(glow);

    listWidget = new QListWidget(this);  // 创建 QListWidget 控件
    listWidget->setSpacing(4);  // 每个 item 之间留 4px
    listWidget->setGeometry(150, 30, 300, 260);  // 参数依次是 x, y, width, height
    listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//支持多选
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

}

void DeleteGamePage::showOptions()
{
    listWidget->clear();
    QStringList items = CustomGameManeger::getInstance()->getallTitles();
    for (int i = 0; i < items.size(); i++)
    {
        listWidget->addItem(items[i]);
    }
}

bool DeleteGamePage::deleteCustomGame()
{
    QList<QListWidgetItem*> selectedItems = listWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        PopTips::Bad(this, listWidget, "请选择要删除的棋盘");
        return false;
    }

    bool allSuccess = true;

    for (QListWidgetItem* item : selectedItems) {
        QString title = item->text();
        if (!CustomGameManeger::getInstance()->deleteGameFromFile(title)) {
            allSuccess = false;
            break;
        }
    }

    if (allSuccess) {
        PopTips::Good(this, listWidget, "删除成功");
    } else {
        PopTips::Bad(this, listWidget, "部分或全部删除失败");
    }

    return allSuccess;
}

void DeleteGamePage::paintEvent(QPaintEvent* event)
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

DeleteGamePage::~DeleteGamePage()
{
    delete ui;
}
