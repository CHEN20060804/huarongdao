#ifndef LEVELDATA_H
#define LEVELDATA_H
#include <QVector>
#include <QString>
class Level
{
private:
    int Id;
    int w;
    int h;
    QVector<QString> element;
public:
    Level(){};
    Level(int w, int h, QVector<QString> level);
    void initLevel(const int& w, const int& h, const QString& s);
    bool initForCustom(const int& w, const int& h, const QString& s);
    int getId() const;
    int getw() const;
    int geth() const;
    void setId(const int& Id);
    QVector<QString> getElement() const;
};

#endif // LEVELDATA_H
