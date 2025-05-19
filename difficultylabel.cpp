#include "difficultylabel.h"
#include <QFont>
DifficultyLabel::DifficultyLabel(QWidget* parent, int x, int y) : QLabel(parent)
{
    QFont font;
    setAlignment(Qt::AlignCenter);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    setMinimumSize(120, 50);
    font.setPixelSize(40);
    font.setBold(true);
    font.setFamily("楷体");
    setFont(font);
    move(x,y);
}

void DifficultyLabel::setDifficulty(double difficulty)
{
    // 设置文本，保留三位小数
    setText(QString("难度系数: %1").arg(difficulty, 0, 'f', 3));

    // 设置对应颜色
    QString color;
    if (difficulty < 0.30) {
        color = "#6FCF97"; // 绿色
    } else if (difficulty >= 0.45) {
        color = "#EB5757"; // 红色
    } else {
        color = "#F2C94C"; // 橙黄色
    }

    // 应用样式
    setStyleSheet(QString(R"(
        color: black;
        background-color: %1;
        border-radius: 8px;
        padding: 6px 12px;
        font-size: 16px;
    )").arg(color));
}
