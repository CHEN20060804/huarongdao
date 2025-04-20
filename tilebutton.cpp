#include "tilebutton.h"
#include <QGraphicsDropShadowEffect>
#include <QFont>

TileButton::TileButton(QWidget* parent, int w, int h)
    : QPushButton(parent), w(w), h(h)
{
    setFixedSize(w, h);
    QFont font;
    font.setPixelSize(h/2);
    font.setFamily("楷体");
    font.setBold(true);
    font.setStyleStrategy(QFont::PreferAntialias);
    setFont(font);

    //设置边框
    setStyleSheet(R"(
    background-color: #FFFFE0 ;
    border: 1px solid black;
    color: #333333;
    border-radius: 3px;
)");
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
    qDebug() << "Hovering!";
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(50);
    shadow->setColor(QColor(0, 0, 100));
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);

    QPushButton::enterEvent(event);
}

void TileButton::leaveEvent(QEvent* event)
{
    setGraphicsEffect(nullptr); // 移除阴影
    QPushButton::leaveEvent(event);
}
