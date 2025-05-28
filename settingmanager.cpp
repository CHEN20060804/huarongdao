#include "settingmanager.h"

SettingManager::SettingManager()
{
    loadSettings();
    btnMap = {
        { "bk7", {50, 200, 220} },
        { "bk_tree", {50, 250, 150} },
        { "bk_sunset", {150, 100, 100} },
        { "bk_snow", {150, 200, 150} },
    };
}

SettingManager* SettingManager::getInstance()
{
    static SettingManager instance;
    return &instance;
}

void SettingManager::loadSettings()
{
    QSettings settings("SCUT", "huarongdao");
    bgm = settings.value("bgm", "wu").toString();
    bgph = settings.value("bgph", "bk7").toString();
    bgmVol = settings.value("bgmVol", 50).toInt();
    soundVol = settings.value("soundVol", 50).toInt();
}

void SettingManager::saveSettings()
{
    QSettings settings("SCUT", "huarongdao");
    settings.setValue("bgm", bgm);
    settings.setValue("bgph", bgph);
    settings.setValue("bgmVol", bgmVol);
    settings.setValue("soundVol", soundVol);
    qDebug() << bgph;
}

void SettingManager::setAll(const QString& bgm, const QString& bgph, int bgmVol, int soundVol)
{
    setBGM(bgm);
    setBGPH(bgph);
    setBGMVolume(bgmVol);
    setSoundVolume(soundVol);
}
