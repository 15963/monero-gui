#include "currentinfo.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>

CurrentInfo::CurrentInfo(QObject *parent) : QObject(parent)
{
    filepool = "pool";
    filenode = "node";
}

QString CurrentInfo::getCurrentPoolInfo()
{
    QString currentPool;
    QFile data(path + filepool);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        int ans = 0;
        in >> currentPool >> ans;
    }
    return currentPool;
}

QString CurrentInfo::getCurrentNodeInfo()
{
    QString currentNode;
    QFile data(path + filenode);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        in >> currentNode;
    }
    return currentNode;
}

bool CurrentInfo::setCurrentPoolInfo(QString poolinfo)
{
    createFile( path,filepool);
    QFile file(path + filepool);
    file.open(QIODevice::ReadOnly);
    QTextStream io(&file);
    int b;
    io>>b;
    file.close();
    file.open(QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    io<<poolinfo;
    file.close();
}

bool CurrentInfo::setCurrentNodeInfo(QString nodeinfo)
{
    createFile( path,filenode);
    QFile file(path + filenode);
    file.open(QIODevice::ReadOnly);
    QTextStream io(&file);
    int b;
    io>>b;
    file.close();
    file.open(QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    io<<nodeinfo;
    file.close();
}

void CurrentInfo::createFile(QString filePath,QString fileName)
{
    QDir tempDir;
    //临时保存程序当前路径
    QString currentDir = tempDir.currentPath();
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(filePath))
    {
        qDebug()<<"不存在该路径"<<endl;
        tempDir.mkpath(filePath);
    }
    QFile *tempFile = new QFile;
    //将程序的执行路径设置到filePath下
    tempDir.setCurrent(filePath);
    qDebug()<<tempDir.currentPath();
    //检查filePath路径下是否存在文件fileName,如果停止操作。
    if(tempFile->exists(fileName))
    {
        qDebug()<<"文件存在";
        return ;
    }
    //此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
    tempFile->setFileName(fileName);
    if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"打开失败";
    }
    tempFile->close();
    //将程序当前路径设置为原来的路径
    tempDir.setCurrent(currentDir);
    qDebug()<<tempDir.currentPath();
}
