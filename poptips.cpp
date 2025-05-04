#include "poptips.h"
#include <QTimer>

QPointer<QLabel> PopTips::currentTip = nullptr;

void PopTips::showTip(QWidget* parent, QWidget* anchorWidget, const QString& message, const QString& borderColor)
{
    // 如果前一个 Tip 存在，手动销毁
    if (currentTip) {
        currentTip->deleteLater();
        currentTip = nullptr;
    }

    QLabel* tip = new QLabel(parent);
    tip->setText(message);
    tip->setStyleSheet(QString(
                           "background-color: black; color: white; border: 1px solid %1; "
                           "padding: 5px; border-radius: 6px;").arg(borderColor));
    tip->adjustSize();
    tip->move(anchorWidget->x() + (anchorWidget->width() - tip->width()) / 2,
              anchorWidget->y() + anchorWidget->height() + 5);
    tip->show();

    currentTip = tip;

    // 定时自动销毁
    QTimer::singleShot(3000, tip, [tip]() {
        tip->deleteLater();
        if (PopTips::currentTip == tip)
            PopTips::currentTip = nullptr;
    });
}

void PopTips::Good(QWidget* parent, QWidget* anchorWidget, const QString& message)
{
    showTip(parent, anchorWidget, message, "#35DE27");
}

void PopTips::Bad(QWidget* parent, QWidget* anchorWidget, const QString& message)
{
    showTip(parent, anchorWidget, message, "#ffa07a");
}
