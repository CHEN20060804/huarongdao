#ifndef LEVELORIGINANDAUTOR_H
#define LEVELORIGINANDAUTOR_H

#include <QString>
#include <QVector>

class LevelOriginAndAutor {
public:
    static QString getTargetText(int index);

private:

    static const QVector<QString> targetText;

};


#endif // LEVELORIGINANDAUTOR_H
