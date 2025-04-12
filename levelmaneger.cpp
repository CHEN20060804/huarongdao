#include "levelmaneger.h"

LevelManeger::LevelManeger() {
    levels.resize(16);
    levels[1].initLevel(3, 3, "青青子衿悠悠我心");

}

const Level& LevelManeger::getLevel(const int& index) const
{
    return levels[index];
}
