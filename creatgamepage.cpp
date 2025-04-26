#include "creatgamepage.h"
#include "ui_creatgamepage.h"
#include "mainbutton.h"
#include <QPainter>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include "customgamemaneger.h"
CreatGamePage::CreatGamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreatGamePage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);
    ui->saveBtn->setwh(100, 50);
    ui->cancelBtn->setwh(100, 50);
    connect(ui->saveBtn, &QPushButton::clicked, [=](){
        emit saveBtnClicked();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });

    //QApplication::setStyle("Fusion");
    QString styleSheet = R"(
    QLabel {
        font-family: 'Segoe UI', serif;
        font-size: 14px;
        font-weight: bold;
        color: white;
    }

    QLineEdit, QTextEdit , QComboBox{
        background-color: #444444;
        border: 1px solid #555555;
        border-radius: 5px;
        padding: 5px;
        color: white;
        font-size: 14px;
    }

    QLineEdit {
        padding-left: 8px;
        letter-spacing: 1px;
        border: 1px solid #666666;
        background-color: #3a3a3a;
    }

    QLineEdit:focus {
        border: 1px solid #3498db;
        background-color: #555555;
    }

    QLineEdit::placeholder {
        color: #aaaaaa;
        font-style: italic;
    }

    QLineEdit:hover {
        background-color: #4a4a4a;
    }

    QTextEdit:focus, QSpinBox:focus {
        border: 1px solid #3498db;
        background-color: #555555;
    }

    QTextEdit {
        border: 1px solid #555555;
        border-radius: 5px;
    }
)";
    setStyleSheet(styleSheet);

    // 加阴影效果
    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(50);  // 越大越柔和
    glow->setOffset(0, 0);    // 向四周发散
    glow->setColor(QColor(42, 108, 246, 180));  //制造蓝光晕
    this->setGraphicsEffect(glow);


    ui->comboBox->addItem("3×3");
    ui->comboBox->addItem("3×4");
    ui->comboBox->addItem("3×5");
    ui->comboBox->addItem("4×4");
    ui->comboBox->addItem("4×5");
    ui->comboBox->addItem("4×6");
    ui->comboBox->addItem("5×5");
    ui->comboBox->addItem("5×6");
    ui->comboBox->addItem("6×6");



}

void CreatGamePage::paintEvent(QPaintEvent* event)
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

bool CreatGamePage::saveCustomGame()
{
    auto showTip = [this](QWidget* widget, const QString& message) {
        QLabel *tip = new QLabel(this);
        tip->setText(message);
        tip->setStyleSheet("background-color: black; color: white; border: 1px solid #ffa07a; padding: 5px; border-radius: 6px;");
        tip->adjustSize();
        tip->move(widget->x() + (widget->width() - tip->width()) / 2 + 20, widget->y() + widget->height() + 5);
        tip->show();
        QTimer::singleShot(3000, tip, &QLabel::deleteLater);
        widget->setFocus();
    };

    int h[9] = {3, 3, 3, 4, 4, 4, 5, 5, 6};
    int w[9] = {3, 4, 5, 4, 5, 6, 5, 6, 6};
    int selectedIndex = ui->comboBox->currentIndex();
    int rows = h[selectedIndex];
    int cols = w[selectedIndex];

    QString title = ui->lineEdit->text();
    QRegularExpression re("^[\u4e00-\u9fa5a-zA-Z0-9._-]+$");
    if (!re.match(title).hasMatch()) {
        showTip(ui->lineEdit, "标题名称不合法");
        ui->lineEdit->selectAll();
        return false;
    }

    QString content = ui->textEdit->toPlainText();
    if (content.contains(QRegularExpression("[\\x09\\x20\\x{3000}]"))) {
        showTip(ui->textEdit, "棋盘内容不合法（不能包含空格或Tab）");
        return false;
    }


    int realElementCount = 0;
    for (int i = 0; i < content.size(); ) {
        if (content[i] == '{') {
            int end = content.indexOf('}', i);
            if (end != -1)
            {
                QString inner = content.mid(i + 1, end - i - 1);
                if (inner.isEmpty())
                {
                    showTip(ui->textEdit, "棋盘内容格式错误（空的 {} 不合法）");
                    return false;
                }
                if (inner.contains('{') || inner.contains('}'))
                {
                    showTip(ui->textEdit, "棋盘内容格式错误（不允许嵌套 {} ）");
                    return false;
                }
                realElementCount++;
                i = end + 1;
            }
            else
            {
                showTip(ui->textEdit, "棋盘内容格式错误（缺少 } ）");
                return false;
            }
        } else {
            realElementCount++;
            ++i;
        }
    }


    if (realElementCount != rows * cols - 1) {
        showTip(ui->textEdit, QString("棋盘元素数量错误（需要 %1 个元素）").arg(rows * cols - 1));
        return false;
    }

    QString stream = QString::number(rows) + " " + QString::number(cols) + "\n" + content;
    if (!CustomGameManeger::getInstance()->saveGameToFile(title, stream))
    {
        return false;
    }

    return true;
}


CreatGamePage::~CreatGamePage()
{
    delete ui;
}
