#include "rankmanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
RankManager::RankManager()
    :dirPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/levelBest/"),
    file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/levelBest/Data.json")
{
    LevelBest.resize(20);
    for(int i = 0; i < LevelBest.size(); i++)
    {
        LevelBest[i] = GameSessionData(i);
    }
    readRecord();
}


RankManager* RankManager::getInstance()
{
    static RankManager manager;
    return &manager;
}


bool RankManager::writeRecord(GameSessionData& data)
{
    int Id = data.getId();
    double newSeconds = data.getSeconds();
    double oldSeconds = LevelBest[Id].getSeconds();
    if( oldSeconds < newSeconds && oldSeconds > 1e-3) return false;
    LevelBest[Id] = data;
    QDate date = QDate::currentDate();
    LevelBest[Id].setDate(date.toString("yyyy-MM-dd"));
    QDir dir;
    if(!dir.mkpath(dirPath))
    {
        return false;
    }
    qDebug() << "Directory Path: " << dirPath;

    QJsonArray array;
    for (const auto& d : LevelBest) {
        array.append(d.toJson());
    }

    // 写入 JSON 文件
    QJsonDocument doc(array);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }
}

bool RankManager::readRecord()
{
    if (!file.exists()) return false;

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isArray()) return false;

        QJsonArray array = doc.array();
        int count = std::min(array.size(), LevelBest.size());
        for (int i = 0; i < count; ++i) {
            QJsonObject obj = array[i].toObject();
            LevelBest[i] = GameSessionData::fromJson(obj);
        }
        return true;
    }
    return false;
}

QVector<GameSessionData> RankManager::getLevelBest()
{
    return LevelBest;
}
