#ifndef DIFFICULTYLABEL_H
#define DIFFICULTYLABEL_H

#include <QLabel>

class DifficultyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DifficultyLabel(QWidget* parent = nullptr, int x = 0, int y = 0);

    void setDifficulty(double difficulty);
};
#endif // DIFFICULTYLABEL_H
