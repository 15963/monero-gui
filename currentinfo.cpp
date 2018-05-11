#include "currentinfo.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#include "misc_log_ex.h"

CurrentInfo::CurrentInfo(QObject *parent) : QObject(parent)
{
    filepool = "pool";
    filenode = "node";
    fileselpool = "selPool";
    fileselnode = "selNode";

}

QString CurrentInfo::getCurrentPoolInfo()
{
    QString currentPool="";
    QFile data(path + filepool);
    if (!data.exists())
    {
        return currentPool;
    }
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        int ans = 0;
        in >> currentPool >> ans;
    }
    return currentPool;
}

QString CurrentInfo::getCurrentNodeInfo()
{
    QString currentNode="";
    QFile data(path + filenode);
     if (!data.exists())
    {
        return currentNode;
    }
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        in >> currentNode;
    }
    return currentNode;
}

bool CurrentInfo::setCurrentPoolInfo(QString pool_address,QString pool_port,QString wallet_address,QString threads)
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
    QTextStream write(&file);
    write<<pool_address<<":"<<pool_port<<":"<<wallet_address<<":"<<threads;
    write.flush();
    file.close();
}

bool CurrentInfo::setCurrentNodeInfo(QString nodeinfo,QString wallet_address,QString threads)
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
    QTextStream write(&file);
    write<<nodeinfo<<":"<<wallet_address<<":"<<threads;
    write.flush();
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

 void CurrentInfo::deleteFile(int type)
 {
     switch (type)
     {
         case  1 : //pool
         {
                QFile pool_file(path + filepool);
                if (pool_file.exists()) {
                    pool_file.remove();
                }
         }
         break;
         case  2 : //node
         {
                QFile node_file(path + filenode);
                if (node_file.exists()) {
                    node_file.remove();
                }
         }
         break;
     }
 }

int CurrentInfo::getCurrentType()
{
       Runtype run_type = RUN_NOTH;
       QString miningInfor = getSelectMinInfo(); 
       MGINFO("RRNC auto starting xmrig type"<< std::string((const char *)miningInfor.toLocal8Bit()));
       QRegExp tagExp(":");
       QStringList paramList = miningInfor.split(tagExp);
       QString m_node_address = ""; 
      if (paramList.length() > 0) {    
          m_node_address = paramList.at(0);
          MGINFO("RRNC auto starting user select index:"<< std::string((const char *)m_node_address.toLocal8Bit()));
          if (m_node_address == "0") {
               run_type = RUN_NODE;
          } else {
               run_type = RUN_POOL;
          }
       }      
       return (int)run_type;
}

 bool CurrentInfo::isBackgroundMining()
 {
      bool bret = false; 
      QString miningInfor = getSelectMinInfo(); 
      MGINFO("RRNC auto starting xmrig type"<< std::string((const char *)miningInfor.toLocal8Bit()));
      QRegExp tagExp(":");
      QStringList paramList = miningInfor.split(tagExp);
      QString m_background_token = ""; 
      if (paramList.length() > 1) {    
          m_background_token = paramList.at(1);
          MGINFO("RRNC auto starting user select index:"<< std::string((const char *)m_background_token.toLocal8Bit()));
          if (m_background_token == "true") {
               bret = true; 
          } else {
              bret = false; 
          }
       }      
      
       return bret;
 }

void CurrentInfo::setSelectMinInfo(QString mining,QString  back_ming,QString threads)
{
    createFile( path,fileselpool);
    QFile file(path + fileselpool);
    file.open(QIODevice::ReadOnly);
    QTextStream io(&file);
    int b;
    io>>b;
    file.close();
    file.open(QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    QTextStream write(&file);
    write<<mining<<":"<<back_ming<<":"<<threads;
    write.flush();
    file.close();
}

QString  CurrentInfo::getSelectMinInfo()
{
    QString currentMinInfo ="";
    QFile data(path + fileselpool);
     if (!data.exists())
    {
        return currentMinInfo;
    }
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        in >> currentMinInfo;
    }
    return currentMinInfo;
}

void  CurrentInfo::setSelectNodeInfo(QString node,QString port, QString name,QString password)
{
    createFile( path,fileselnode);
    QFile file(path + fileselnode);
    file.open(QIODevice::ReadOnly);
    QTextStream io(&file);
    int b;
    io>>b;
    file.close();
    file.open(QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    QTextStream write(&file);
    write<<node<<":"<<port<<":"<<name<<":"<<password;
    write.flush();
    file.close();
}


QString  CurrentInfo::getSelectNodeInfo()
{
    QString currentNodeInfo ="";
    QFile data(path + fileselnode);
     if (!data.exists())
    {
        return currentNodeInfo;
    }
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        in >> currentNodeInfo;
    }
    return currentNodeInfo;
}
