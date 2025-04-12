#ifndef LEVELMANEGER_H
#define LEVELMANEGER_H
#include "leveldata.h"
class LevelManeger
{
public:
    LevelManeger();
    const Level& getLevel(const int& index) const;
private:
    QVector<Level> levels;
};

#endif // LEVELMANEGER_H
