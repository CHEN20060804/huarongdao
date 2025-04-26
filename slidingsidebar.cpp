#include "slidingsidebar.h"

SlidingSidebar::SlidingSidebar(QWidget* parent,
                               int sidebarWidth,
                               int sidebarHeight,
                               QPoint topLeft,
                               const QString& backgroundColor,
                               const QString& fontFamily,
                               int fontSize,
                               int maxline,
                               SlideDirection direction)
    : QWidget(parent),
    sidebarWidth(sidebarWidth),
    sidebarHeight(sidebarHeight),
    position(topLeft),
    bgColor(backgroundColor),
    maxline(maxline),
    slideDirection(direction)
{
    setGeometry(QRect(position, QSize(sidebarWidth, sidebarHeight)));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TranslucentBackground);

    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InOutCubic);

    contentWidget = new QWidget(this);
    contentWidget->setFixedWidth(sidebarWidth);
    contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setAlignment(Qt::AlignTop);
    contentLayout->setSpacing(40); // 行间距为10像素

    contentLayout->setContentsMargins(30, 13, 20, 13);


    int wrapWidth = sidebarWidth
                    - contentLayout->contentsMargins().left()
                    - contentLayout->contentsMargins().right() + 10;

    for (int i = 0; i < maxline; ++i) {
        QLabel* line = new QLabel(contentWidget);
        line->setText("");
        line->setWordWrap(true);
        line->setAlignment(Qt::AlignLeft | Qt::AlignVCenter |Qt::AlignTop);
        QFont font(fontFamily, fontSize);
        font.setBold(true);
        line->setFont(font);
        line->setStyleSheet("color: black;");
        line->setMinimumHeight(fontSize * 1.5);
        line->setFixedWidth(wrapWidth);
        line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        contentLayout->addWidget(line);
        lineLabels.append(line);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(contentWidget);
    setLayout(mainLayout);

    setupToggleButton();
}

void SlidingSidebar::setupToggleButton()
{
    toggleButton = new QPushButton(this);
    toggleButton->setFixedSize(20, 60);
    if (slideDirection == SlideRight)
        toggleButton->move(0, height() / 2 - 30);
    else
        toggleButton->move(width() - 20, height() / 2 - 30);
    toggleButton->setText(slideDirection == SlideRight ? ">" : "<");
    toggleButton->setStyleSheet("background-color: rgba(0,0,0,60); color: white; border: none; border-radius: 5px;");
    connect(toggleButton, &QPushButton::clicked, this, &SlidingSidebar::toggle);
}

void SlidingSidebar::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor bg(bgColor);
    bg.setAlpha(220); // 半透明
    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), radius, radius);
    QWidget::paintEvent(event);
}

// void SlidingSidebar::setLineText(int lineIndex, const QString& text)
// {
//     if (lineIndex >= 0 && lineIndex < lineLabels.size()) {
//         lineLabels[lineIndex]->setText(text);
//         contentLayout->activate();
//     }
// }

void SlidingSidebar::setLineText(int lineIndex, const QString& text)
{
    if (lineIndex >= 0 && lineIndex < lineLabels.size()) {
        QLabel* label = lineLabels[lineIndex];
        label->setText(text);

        // 自动计算文本高度
        QFontMetrics fm(label->font());
        int wrapWidth = label->width(); // 或者提前保存的 wrapWidth
        QRect bound = fm.boundingRect(0, 0, wrapWidth, 9999, Qt::TextWordWrap, text);

        int finalHeight = bound.height();

        // 限制最大高度，比如最多 6 行
        int maxHeight = 20 * 1.6 * 8;  // 乘 1.6 留余地防止上下裁剪
        finalHeight = std::min(finalHeight, maxHeight);

        label->setMinimumHeight(finalHeight);
        label->setMaximumHeight(finalHeight);
        label->updateGeometry();
        contentLayout->invalidate();
    }
}

void SlidingSidebar::toggle()
{
    if (opened) close();
    else open();
}

void SlidingSidebar::open()
{
    animation->stop();
    QRect startRect = geometry();
    QRect endRect(position, QSize(sidebarWidth, sidebarHeight));
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start();
    toggleButton->setText(slideDirection == SlideRight ? ">" : "<");
    opened = true;
}


void SlidingSidebar::close()
{
    animation->stop();
    QRect startRect = geometry();
    QRect endRect;

    if (slideDirection == SlideRight)
        endRect = QRect(QPoint(position.x() + sidebarWidth - 20, position.y()), QSize(20, sidebarHeight));
    else
    {
        endRect = QRect(QPoint(position.x(), position.y()), QSize(20, sidebarHeight));
    }

    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start();
    toggleButton->setText(slideDirection == SlideRight ? "<" : ">");
    opened = false;
}


void SlidingSidebar::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if(slideDirection == SlideRight) {
        toggleButton->move(0, height()/2 - 30);
    } else {
        toggleButton->move(width() - 20, height()/2 - 30);
    }
}

