#include "settingpage.h"
#include "ui_settingpage.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include "settingmanager.h"



SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::SettingPage)
{
    ui->setupUi(this);
    setFixedSize(600, 400);

    // 设置按钮尺寸
    ui->save->setwh(100, 50);
    ui->cancel->setwh(100, 50);

    //链接事件
    connect(ui->save, &QPushButton::clicked, [=](){
        emit saveBtnClicked();
    });
    connect(ui->cancel, &QPushButton::clicked, [=](){
        emit cancelBtnClicked();
    });

    setStyleSheet(R"(
    QComboBox {
        background-color: #444444;
        border: 1px solid #555555;
        border-radius: 5px;
        padding: 5px;
        color: white;
        font-size: 14px;
    }

    QComboBox QAbstractItemView {
        background-color: #444444;
        color: white;
        font-size: 14px;
        border: none;
        outline: 0;
    }

    QComboBox QAbstractItemView::item:hover {
        background-color: #555555;
    }

    QComboBox QAbstractItemView::item:selected {
        background-color: #666666;
        color: white;
    })");

    bgphMap = {
        { "霁蓝", "bk7" },
        { "仰望", "bk_tree" },
        { "日落", "bk_sunset" },
        { "冬日", "bk_snow" },
    };

    bgmMap = {
        { "无" , "wu" },
        { "雨夜", "yuye" },
        { "春日", "chunri" },
        { "夏日", "xiari" },
    };


    auto keyByValue = [=](const QMap<QString, QString> &map, const QString &value) {
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (it.value() == value) return it.key();
        }
        return QString();
    };//由value找key


    for (const QString& name : bgphMap.keys()) {
        ui->comboBoxBGPH->addItem(name);
    }

    QStringList orderedKeys = {"无", "春日", "夏日", "雨夜"};
    for (const QString& name : orderedKeys) {
        if (bgmMap.contains(name)) {
            ui->comboBoxBGM->addItem(name);
        }
    }
    connect(ui->comboBoxBGPH, &QComboBox::currentTextChanged, this, [=](const QString& name) {
        SettingManager::getInstance()->setBGPH(bgphMap.value(name));
        emit changeBKPH(bgphMap.value(name));
    });
    connect(ui->comboBoxBGM, &QComboBox::currentTextChanged, this, [=](const QString& name) {
        SettingManager::getInstance()->setBGM(bgmMap.value(name));
        emit changeBGM(bgmMap.value(name));
    });


    QString savedBGPH = keyByValue(bgphMap, SettingManager::getInstance()->getBGPH());
    ui->comboBoxBGPH->setCurrentText(savedBGPH);

    QString savedBGM = keyByValue(bgmMap, SettingManager::getInstance()->getBGM());
    ui->comboBoxBGM->setCurrentText(savedBGM);
}

void SettingPage::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    // 创建绘制对象
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿

    // 绘制背景（黑色，带圆角）
    painter.setBrush(QColor("#1e1e1e"));  // 设置背景色
    painter.setPen(Qt::NoPen);  // 不绘制边框
    painter.drawRoundedRect(rect(), 15, 15);  // 绘制圆角背景

    QPen pen(QColor(246, 246, 246));  // 蓝色边框

    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);  // 不填充，只有边框
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 15, 15); // 圆角边框
}


SettingPage::~SettingPage()
{
    delete ui;
}
