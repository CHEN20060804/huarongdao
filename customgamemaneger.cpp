#include "customgamemaneger.h"
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QStringList>
CustomGameManeger::CustomGameManeger()
    :listFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/yourCustomGame/yourGameList.txt"),
    dirPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/yourCustomGame/")
{
    if (listFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&listFile);
        while (!in.atEnd()) {
            QString line = in.readLine();  // 读取一行
            titles.append(line);
        }
        listFile.close();
    }
}
CustomGameManeger* CustomGameManeger::getInstance()
{
    static CustomGameManeger maneger;
    return &maneger;
}

bool CustomGameManeger::saveGameToFile(const QString& title, const QString& content)
{
    QDir dir;
    if (!dir.mkpath(dirPath)) {
        qDebug() << "Failed to create directory:" << dirPath;
        return false;
    }
    qDebug() << "Directory Path: " << dirPath;
    // 创建并打开文件以保存内容
    QString filePath = dirPath + title + ".txt";  // 使用 title 作为文件名
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return false;
    }

    QTextStream out(&file);
    out << content;
    file.close();    // 关闭文件

    //写入列表文件
    listFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
    QTextStream outList(&listFile);
    outList << title << Qt::endl;
    listFile.close();

    qDebug() << "File saved at:" << filePath;

    titles.append(title);

    return true;
}

QStringList CustomGameManeger::loadGameFromFile(const QString& title) const
{
    QString filePath = dirPath + title + ".txt";
    QFile file(filePath);
    QStringList lines;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "无法打开文件:" << filePath;
        return lines;  // 返回空的 QStringList
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();  // 读一行，并去掉前后空格
        if (!line.isEmpty())
        {
            lines.append(line);
        }
    }

    file.close();
    return lines;

}

bool CustomGameManeger::deleteGameFromFile(const QString& title)
{
    QString filePath = dirPath + title + ".txt";
    if (QFile::exists(filePath)) {  // 先检查文件是否存在
        if (QFile::remove(filePath)) {
            qDebug() << "文件删除成功:" << filePath;
            titles.removeOne(title);
            if (!listFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                qDebug() << "无法打开文件用于重写:" << filePath;
                return false;
            }

            QTextStream out(&listFile);
            for (const QString &title : titles) {
                out << title << Qt::endl;  // 一行一个标题
            }

            listFile.close();
            qDebug() << "标题列表文件重写完成:" << filePath;
            return true;

        } else {
            qDebug() << "文件删除失败:" << filePath;
            return false;
        }
    } else {
        qDebug() << "文件不存在:" << filePath;
        return false;
    }
}


const QString& CustomGameManeger::getTitle(int i) const
{
    return titles[i];
}

const QStringList& CustomGameManeger::getallTitles() const
{
    return titles;

}
 CustomGameManeger::~CustomGameManeger() {};
