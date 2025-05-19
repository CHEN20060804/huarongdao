#include "gamesessiondata.h"
#include <cmath>

GameSessionData::GameSessionData(int LevelId)
    :levelId(LevelId), steps(0), seconds(0.0), Difficulty(0.0)
{
}


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
void GameSessionData::stop() {
    seconds = timer.elapsed() / 1000.0;
}
double GameSessionData::getElapsedSeconds() const
{
    return timer.elapsed() / 1000.0;  // 转为秒
}
double GameSessionData::getSeconds() const
{
    return std::round(seconds * 10) / 10.0;
}
int GameSessionData::getId() const
{
    return levelId;
}

QString GameSessionData::getBeijingTime() const
{
    return BeijingTime;
}
double GameSessionData::getDifficulty() const
{
    return Difficulty;
}
void GameSessionData::setDifficult(double diff)
{
    Difficulty = diff;
}
void GameSessionData::setDate(QString s)
{
    BeijingTime = s;
}
QJsonObject GameSessionData::toJson() const
{
    QJsonObject obj;
    obj["levelId"] = levelId;
    obj["seconds"] = seconds;
    obj["steps"] = steps;
    obj["BeijingTime"] = BeijingTime;
    obj["Difficulty"] = Difficulty;
    return obj;
}

GameSessionData GameSessionData::fromJson(const QJsonObject& obj)
{
    GameSessionData data;
    data.levelId = obj["levelId"].toInt();
    data.seconds = obj["seconds"].toDouble();
    data.steps = obj["steps"].toInt();
    data.BeijingTime = obj["BeijingTime"].toString();
    data.Difficulty = obj["Difficulty"].toDouble();
    return data;
}
