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
    for (int i = 0; i < w * h ;++i) {
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

bool Level::initForCustom(const int& w, const int& h, const QString& s)
{
    int realElementCount = 0;  // 真正的元素数

    // 预统计元素个数
    for (int i = 0; i < s.size(); )
    {
        if (s[i] == '{')
        {
            int end = s.indexOf('}', i);
            if (end != -1)
            {
                realElementCount++;
                i = end + 1; // 跳到 '}' 后
            }
            else
            {
                // 格式错误
                return false;
            }
        }
        else
        {
            realElementCount++;
            ++i;
        }
    }

    // 必须恰好是 (w * h - 1) 个有效元素
    if (realElementCount != w * h - 1)
    {
        return false;
    }

    // 通过了校验，正式解析
    this->w = w;
    this->h = h;
    element.clear();

    for (int i = 0; i < s.size(); )
    {
        if (s[i] == '{')
        {
            int end = s.indexOf('}', i);
            QString token = s.mid(i + 1, end - i - 1);
            element.append(token);
            i = end + 1;
        }
        else
        {
            element.append(QString(s[i]));
            ++i;
        }
    }

    element.append("");  // 补上空格
    return true;
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
