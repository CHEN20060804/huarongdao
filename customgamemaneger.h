#ifndef CUSTOMGAMEMANEGER_H
#define CUSTOMGAMEMANEGER_H
#include <QFile>
class CustomGameManeger
{
public:
    static CustomGameManeger* getInstance();

    bool saveGameToFile(const QString& title, const QString& content);

    QStringList loadGameFromFile(const QString& title) const;

    bool deleteGameFromFile(const QString& title) ;

    const QString& getTitle(int i) const;

    const QStringList& getallTitles() const;

private:
    CustomGameManeger();
    QStringList titles;
    QFile listFile;
    QString dirPath;

    // 禁止拷贝构造和赋值操作
    CustomGameManeger(const CustomGameManeger&) = delete;
    void operator=(const CustomGameManeger&) = delete;
};


#endif // CUSTOMGAMEMANEGER_H
