#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QString>
#include <QFile>
class SettingManager
{
public:

    static SettingManager* getInstance();


private:

    SettingManager();
    ~SettingManager(){};

    QString dirPath;
    QFile file;

    QString bgm;
    QString bgph;
    int bgmVol;
    int soundVol;

    SettingManager(const SettingManager&) = delete;
    void operator=(const SettingManager&) = delete;
};

#endif // SETTINGMANAGER_H
