#pragma once

#include <QTableWidget>

class FancyTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit FancyTableWidget(QWidget *parent = nullptr);
};
