#include "gamesessiondata.h"

GameSessionData::GameSessionData(int LevelId)
    :steps(0), levelId(LevelId)
{}


void GameSessionData::start()
{
    timer.start();
}

void GameSessionData::reset()
{
    steps = 0;
    timer.invalidate();
}

void GameSessionData::addStep()
{
    ++steps;
}

int GameSessionData::getSteps() const
{
    return steps;
}

double GameSessionData::getElapsedSeconds() const
{
    return timer.elapsed() / 1000.0;  // 转为秒
}

