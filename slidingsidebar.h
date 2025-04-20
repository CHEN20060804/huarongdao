#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QPainter>
#include <QMouseEvent>

class SlidingSidebar : public QWidget
{
    Q_OBJECT

public:
    enum SlideDirection { SlideLeft, SlideRight };

    explicit SlidingSidebar(QWidget* parent = nullptr,
                            int sidebarWidth = 200,
                            int sidebarHeight = 300,
                            QPoint topLeft = QPoint(0, 0),
                            const QString& backgroundColor = "#FAFAD2",
                            const QString& fontFamily = "楷体",
                            int fontSize = 14,
                            int maxline = 2,
                            SlideDirection direction = SlideRight);

    void setLineText(int index, const QString& text);
    void toggle();
    void open();
    void close();
    bool isOpen() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;
    QPropertyAnimation* animation;
    QPushButton* toggleButton;
    int sidebarWidth;
    int sidebarHeight;
    QPoint position;
    bool opened = true;
    QString bgColor;
    int radius = 12;
    SlideDirection slideDirection;
    QVector<QLabel*> lineLabels;  // 每行一个 QLabel
    int maxline;


    void setupToggleButton();
};
