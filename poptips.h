#ifndef POPTIPS_H
#define POPTIPS_H

#include <QLabel>
#include <QPointer>
#include <QWidget>

class PopTips {
public:
    static void Good(QWidget* parent, QWidget* anchorWidget, const QString& message, bool isTemp = true);
    static void Bad(QWidget* parent, QWidget* anchorWidget, const QString& message, bool isTemp = true);
    static void GoodCenter(QWidget* parent, const QString& message, bool isTemp = true);

private:
    static void showTip(QWidget* parent, QWidget* anchorWidget, const QString& message, const QString& borderColor, bool isTemp = true);
    static QPointer<QLabel> currentTip;  // 当前正在显示的 Tip

};

#endif // POPTIPS_H
