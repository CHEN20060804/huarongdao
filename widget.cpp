#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("活字华容道");//设置标题
    setFixedSize(850, 600);

    connect(ui->bntExit, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btnReturn, &QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
}

Widget::~Widget()
{
    delete ui;
}
