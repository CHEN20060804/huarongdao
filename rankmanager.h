#ifndef RANKMANAGER_H
#define RANKMANAGER_H

#include <QFile>
#include <QVector>
#include "gamesessiondata.h"
class RankManager
{
public:

    static RankManager* getInstance();

    bool writeRecord(GameSessionData& data);


    QVector<GameSessionData> getLevelBest();


private:

    RankManager();
    ~RankManager(){};

    bool readRecord();


    QVector<GameSessionData> LevelBest;
    QString dirPath;
    QFile file;

    RankManager(const RankManager&) = delete;
    void operator=(const RankManager&) = delete;
};

#endif // RANKMANAGER_H
