#include "leveldata.h"

Level::Level(int w, int h, QVector<QString> level)
    :w(w), h(h), element(level)
{

}
void Level::initLevel(const int& w, const int& h, const QString& s)
{
    this->w = w;
    this->h = h;
    element =  QVector<QString>(h*w);
    int k = 0;
    for (int i = 0; i < w*h ;++i) {
        if(i<s.size())
        {
            element[i] = s.mid(i, 1);
        }
        else
        {
            element[i]="";
        }
    }

}
int Level::getId() const
{
    return Id;
}
int Level::getw() const
{
    return w;
}
int Level::geth() const
{
    return h;
}
QVector<QString> Level::getElement() const
{
    return element;
}
void Level::setId(const int& Id)
{
    this->Id = Id;
}
