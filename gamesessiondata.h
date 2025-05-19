#pragma once
#include <QElapsedTimer>
#include <QString>
#include <QJsonObject>
class GameSessionData {
public:
    GameSessionData(int levelId);
    GameSessionData(): steps(0){};
    void start();
    void addStep();
    void reset();                       // 重置

    int getSteps() const;
    double getElapsedSeconds() const;
    QJsonObject toJson() const;
    static GameSessionData fromJson(const QJsonObject& obj);
    int getId() const;
    double getSeconds() const;
    void stop();
    QString getBeijingTime() const;
    double getDifficulty() const;
    void setDifficult(double diff);
    void setDate(QString s);


private:
    int levelId;
    int steps;
    QElapsedTimer timer;
    double seconds;
    QString BeijingTime;
    double Difficulty;
};
