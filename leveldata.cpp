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
