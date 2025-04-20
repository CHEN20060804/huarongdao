#pragma once
#include <QElapsedTimer>
class GameSessionData {
public:
    GameSessionData(int levelId);
    GameSessionData(){};
    void start();
    void addStep();
    void reset();                       // 重置（用于开始新游戏）

    int getSteps() const;
    double getElapsedSeconds() const;

private:
    int levelId;
    int steps;
    QElapsedTimer timer;
    double seconds;
};
