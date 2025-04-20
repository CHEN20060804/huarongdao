#ifndef LEVELORIGINANDAUTOR_H
#define LEVELORIGINANDAUTOR_H

#include <QString>
#include <QVector>

class LevelOriginAndAutor {
public:
    // 静态成员函数，不依赖对象
    static QString getTargetText(int index);
    static QString getOrigin(int index);
    static QString getAutor(int index);

private:
    // 静态成员数据
    static const QVector<QString> targetText;
    static const QVector<QString> origins;
    static const QVector<QString> autors;
};


#endif // LEVELORIGINANDAUTOR_H
