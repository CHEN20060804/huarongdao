#ifndef TILEBUTTON_H
#define TILEBUTTON_H

#include <QPushButton>

class TileButton : public QPushButton {
    Q_OBJECT
private:
    int w;
    int h;
    int m_row;//所在行
    int m_col;//所在列
    bool m_selected;
    bool m_empty;
protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent*event) override;
public:
    TileButton(QWidget* parent = nullptr, int w = 0, int h = 0);
    void setRowCol(int row, int col);
    int row();
    int col();
    //void setSelected(bool selected);

    void setSelected(bool sel) { m_selected = sel; update(); }
    void setEmpty   (bool emp) { m_empty = emp;     update(); }
signals:
    void triggered();
};
#endif // TILEBUTTON_H
