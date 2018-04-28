#ifndef DOHTTP_H
#define DOHTTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class Dohttp : public QObject
{
    Q_OBJECT
public:
    explicit Dohttp(QObject *parent = nullptr);

signals:
    void get_pools_message( QString strinfo);
    void get_nodes_message( QString strinfo);

public slots:
    void finished_slotget_pools(QNetworkReply*);
    void finished_slotget_nodes(QNetworkReply*);

private:
    QNetworkAccessManager *m_NetMangerPools;
    QNetworkAccessManager *m_NetMangerNodes;
    QNetworkReply* m_ReplyPools;
    QNetworkReply* m_ReplyNodes;
    QString pools_info;
    QString nodes_info;
public:
    Q_INVOKABLE QString get_pools_info();
    Q_INVOKABLE QString get_nodes_info();

    void fetch_pools();
    void fetch_nodes();

};

#endif // DOHTTP_H

