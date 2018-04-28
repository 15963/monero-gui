#ifndef COMMONDATA_H
#define COMMONDATA_H

#include<QString>
#include<QMap>
#include <memory>


const QString behe_url = "http://work.rrnc.io/";        // web url
const QString pools_url = "getpools";                   // get pools
const QString remote_nodes_url = "getnodes";            // get remote nodes


class CommonData
{

public:
   static QString MakeMD5Key(QString strIn)
    {
         QString md5;
        QByteArray ba,bb;
        QCryptographicHash md(QCryptographicHash::Md5);
        ba.append(strIn);
        md.addData(ba);
        bb = md.result();
        md5.append(bb.toHex());
        return md5;
    }

  static  QString GetLongTime()
    {
        qint64 time = QDateTime::currentMSecsSinceEpoch();
        QString strTime = QString::number(time/1000,10);
        return strTime;
    }


};



#endif // COMMONDATA_H
