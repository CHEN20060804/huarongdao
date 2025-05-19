#include "tilebutton.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QStyle>
#include <QPainter>
TileButton::TileButton(QWidget* parent, int w, int h)
    : QPushButton(parent), w(w), h(h), m_selected(false), m_empty(false)
{
    setFixedSize(w, h);
    QFont font;
    font.setPixelSize(h/2);
    font.setFamily("楷体");
    font.setBold(true);
    font.setStyleStrategy(QFont::PreferAntialias);
    setFont(font);
}
void TileButton::setRowCol(int row, int col)
{
    m_row = row;
    m_col = col;
}

int TileButton::row()
{
    return m_row;
}

int TileButton::col()
{
    return m_col;
}

void TileButton::enterEvent(QEnterEvent* event)
{
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(60);
    shadow->setColor(QColor(0, 0, 100));
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);

    QPushButton::enterEvent(event);
}

void TileButton::paintEvent(QPaintEvent* e) {
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);

    // 1) 自己画背景
    QColor bg;
    if      (m_selected)  bg = QColor("#D0EFFF");
    else if (!m_empty)    bg = QColor("#FFFFE0");
    else                  bg = Qt::transparent;
    p.fillRect(rect(), bg);

    // 2) 画边框
    QPen pen;
    if      (m_selected)  { pen.setWidth(2); pen.setColor(QColor("#3399FF")); }
    else if (m_empty)     { pen.setStyle(Qt::NoPen); }
    else                  { pen.setWidth(2); pen.setColor(Qt::black); }
    p.setPen(pen);
    p.drawRect(rect());

    // 3) 画文字（居中）
    if (!m_empty) {
        p.setPen(Qt::black);
        p.setFont(font());
        p.drawText(rect(), Qt::AlignCenter, text());
    }
}


void TileButton::leaveEvent(QEvent* event)
{
    setGraphicsEffect(nullptr); // 移除阴影
    QPushButton::leaveEvent(event);
}
