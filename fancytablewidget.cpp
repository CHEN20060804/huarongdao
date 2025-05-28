#include <QScrollBar>
#include <QWheelEvent>
#include "FancyTableWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QHeaderView>

FancyTableWidget::FancyTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true); // 让样式表背景生效
    setFrameStyle(QFrame::NoFrame);

    // 关闭选择、编辑
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    verticalHeader()->setVisible(false);

    setAlternatingRowColors(true);

    setFocusPolicy(Qt::NoFocus);
    setCurrentItem(nullptr);

    setStyleSheet(R"(
    QTableWidget {
        background-color: #2e2e2e;
        color: white;
        font-family: '楷体';
        font-size: 12pt;
        border: 2px solid white;
        border-radius: 5px;
    }
    QHeaderView::section {
        background-color: #1e1e1e;
        color: white;
        font-family: '楷体';
        font-size: 14pt;
        font-weight: bold;
        padding: 6px;
        border: none;
    }
)");
}
