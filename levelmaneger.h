#ifndef LEVELMANEGER_H
#define LEVELMANEGER_H
#include "leveldata.h"
class LevelManeger
{
public:
    LevelManeger();
    const Level& getLevel(const int& index) const;
    int getSize() const;

private:
    QVector<Level> levels;
    int size;
};

#endif // LEVELMANEGER_H
