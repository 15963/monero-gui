#ifndef CURRENTINFO_H
#define CURRENTINFO_H

#include <QObject>

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
    bool setCurrentPoolInfo(QString poolinfo);
    bool setCurrentNodeInfo(QString nodeinfo);
private:
    QString filepool;
    QString filenode;
    void createFile(QString filePath,QString fileName);


};

#endif // CURRENTINFO_H
