#ifndef CURRENTINFO_H
#define CURRENTINFO_H

#include <QObject>

typedef enum _RUNTYPE{
    RUN_POOL = 1,
    RUN_NODE = 2, 
    RUN_BOTH = 3,
    RUN_NOTH = 4； 
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
    bool setCurrentNodeInfo(QString nodeinfo,QString threads);
    void deleteFile(int type);
    int  getCurrentType(); 

private:
    QString filepool;
    QString filenode;
    void createFile(QString filePath,QString fileName);


};

#endif // CURRENTINFO_H
