#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QSettings>
#include <QString>
#include <QMap>
#include "RGB.h"
class SettingManager
{
public:
    static SettingManager* getInstance();

    void loadSettings();     // 读取设置
    void saveSettings();     // 保存设置

    void setBGM(const QString& name) { bgm = name; }
    void setBGPH(const QString& name) { bgph = name; }
    void setBGMVolume(int vol) { bgmVol = vol; }
    void setSoundVolume(int vol) { soundVol = vol; }
    void setAll(const QString& bgm, const QString& bgph, int bgmVol, int soundVol);

    QString getBGM() const { return bgm; }
    QString getBGPH() const { return bgph; }
    int getBGMVolume() const { return bgmVol; }
    int getSoundVolume() const { return soundVol; }
    RGB getRGB() const{return btnMap.value(bgph); }

private:
    SettingManager();
    ~SettingManager() {}

    QString bgm;
    QString bgph;
    int bgmVol;
    int soundVol;
    QMap<QString, RGB> btnMap;

    SettingManager(const SettingManager&) = delete;
    void operator=(const SettingManager&) = delete;
};

#endif // SETTINGMANAGER_H
