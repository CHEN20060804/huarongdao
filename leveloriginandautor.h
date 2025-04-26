#ifndef LEVELORIGINANDAUTOR_H
#define LEVELORIGINANDAUTOR_H

#include <QString>
#include <QVector>

class LevelOriginAndAutor {
public:
    // 静态成员函数，不依赖对象
    static QString getTargetText(int index);

private:
    // 静态成员数据
    static const QVector<QString> targetText;

};


#endif // LEVELORIGINANDAUTOR_H
