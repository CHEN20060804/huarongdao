#ifndef LEVELDATA_H
#define LEVELDATA_H
#include <QVector>
#include <QString>
struct Level
{
    int w;
    int h;
    QVector<QString> element;
    Level(){};
    Level(int w, int h, QVector<QString> level);
    void initLevel(const int& w, const int& h, const QString& s);
};

#endif // LEVELDATA_H
