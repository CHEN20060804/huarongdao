#include "customgamemaneger.h"
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QStringList>
CustomGameManeger::CustomGameManeger()
{
    QFile file(QCoreApplication::applicationDirPath() + "/yourCustomGame/yourGameList.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();  // 读取一行
            titles.append(line);
        }
        file.close();
    }
}
CustomGameManeger* CustomGameManeger::getInstance()
{
    static CustomGameManeger maneger;
    return &maneger;
}
bool CustomGameManeger::saveGameToFile(const QString& title, const QString& content)
{
    QString dirPath = QCoreApplication::applicationDirPath() + "/yourCustomGame/";  // 获取当前 exe 文件所在的目录
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


    QString listFilePath = QCoreApplication::applicationDirPath() + "/yourCustomGame/yourGameList.txt";  // 获取当前 exe 文件所在的目录
    QFile listFile(listFilePath);
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
    QString filePath = QCoreApplication::applicationDirPath() + "/yourCustomGame/" + title + ".txt";  // 获取当前 exe 文件所在的目录
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

const QString& CustomGameManeger::getTitle(int i) const
{
    return titles[i];
}

QVector<QString> CustomGameManeger::getallTitles() const
{
    return titles;

}

