#include "dohttp.h"
#include "commondata.h"

Dohttp::Dohttp(QObject *parent) : QObject(parent)
{
    m_NetMangerPools = new QNetworkAccessManager;
    m_NetMangerNodes = new QNetworkAccessManager;
}

void Dohttp::fetch_pools()
{
    QUrl url(behe_url+pools_url);
    QNetworkRequest request;

    request.setUrl(url);
    m_ReplyPools = m_NetMangerPools->get(request);
    QObject::connect(m_NetMangerPools, SIGNAL(finished(QNetworkReply*)),
           this, SLOT(finished_slotget_pools(QNetworkReply*)));
 }

QString Dohttp::get_pools_info()
{
     return pools_info;
}

QString Dohttp::get_nodes_info()
{
     return nodes_info;
}

void Dohttp::finished_slotget_pools(QNetworkReply *)
{
    m_ReplyPools->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    m_ReplyPools->attribute(QNetworkRequest::RedirectionTargetAttribute);

    QString data ;
    if (m_ReplyPools->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_ReplyPools->readAll();
        data = QString::fromUtf8(bytes);
        qDebug()<<data;
    }
    else
    {
        qDebug()<< m_ReplyPools->errorString();
        data =  m_ReplyPools->errorString();
    }
    pools_info = data;
    emit this->get_pools_message(data);
    m_ReplyPools->deleteLater();
    m_NetMangerPools->disconnect(SIGNAL(finished(QNetworkReply*)));
}

void Dohttp::fetch_nodes()
{
    QUrl url(behe_url+remote_nodes_url);
    QNetworkRequest request;

    request.setUrl(url);
    m_ReplyNodes = m_NetMangerNodes->get(request);
    QObject::connect(m_NetMangerNodes, SIGNAL(finished(QNetworkReply*)),
           this, SLOT(finished_slotget_nodes(QNetworkReply*)));
}

void Dohttp::finished_slotget_nodes(QNetworkReply *)
{
    m_ReplyNodes->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    m_ReplyNodes->attribute(QNetworkRequest::RedirectionTargetAttribute);

    QString data ;
    if (m_ReplyNodes->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_ReplyNodes->readAll();
        data = QString::fromUtf8(bytes);
        qDebug()<<data;
    }
    else
    {
        qDebug()<< m_ReplyNodes->errorString();
        data =  m_ReplyNodes->errorString();
    }
    nodes_info = data;
    emit this->get_nodes_message(data);
    m_ReplyNodes->deleteLater();
    m_NetMangerNodes->disconnect(SIGNAL(finished(QNetworkReply*)));
}



