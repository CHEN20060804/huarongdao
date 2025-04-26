#ifndef CUSTOMGAMEMANEGER_H
#define CUSTOMGAMEMANEGER_H
#include <QString>
#include <QVector>
class CustomGameManeger
{
public:
    static CustomGameManeger* getInstance();

    bool saveGameToFile(const QString& title, const QString& content);

    QVector<QString> loadGameFromFile(const QString& title) const;

    const QString& getTitle(int i) const;

    QStringList getallTitles() const;

private:
    CustomGameManeger();  // 私有化构造函数，确保只能通过 getInstance 获取实例
    QVector<QString> titles;
    // 禁止拷贝构造和赋值操作
    CustomGameManeger(const CustomGameManeger&) = delete;
    void operator=(const CustomGameManeger&) = delete;
};


#endif // CUSTOMGAMEMANEGER_H
