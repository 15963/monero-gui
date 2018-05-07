#ifndef CURRENTINFO_H
#define CURRENTINFO_H

#include <QObject>

typedef enum _RUNTYPE{
    RUN_POOL = 1,
    RUN_NODE = 2, 
    RUN_BOTH = 3,
    RUN_NOTH = 4
} Runtype; 

class CurrentInfo : public QObject
{
    Q_OBJECT
public:
    explicit CurrentInfo(QObject *parent = nullptr);

    QString path;

signals:

public slots:

    QString getCurrentPoolInfo();
    QString getCurrentNodeInfo();
    bool setCurrentPoolInfo(QString pool_address,QString pool_port,QString wallet_address,QString threads);
    bool setCurrentNodeInfo(QString nodeinfo,QString wallet_address,QString threads);
    void setSelectMinInfo(QString mining,QString  back_ming,QString threads);
    QString getSelectMinInfo();
    void setSelectNodeInfo(QString node,QString port, QString name,QString password);
    QString getSelectNodeInfo();
    void deleteFile(int type);
    int  getCurrentType(); 
    bool isBackgroundMining(); 

private:
    QString filepool;
    QString filenode;
    QString fileselpool;
    QString fileselnode;
    void createFile(QString filePath,QString fileName);


};

#endif // CURRENTINFO_H
