#include "RpcManager.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <sstream>

RpcManager * RpcManager::m_instance = nullptr;

RpcManager *RpcManager::instance()
{
    if (!m_instance) {
        m_instance = new RpcManager;
    }

    return m_instance;
}

bool RpcManager::startXmrig(int port)
{
    QString program = "";
    QStringList arguments;
    arguments<< "--rpc-bind-port" << QString("%1").arg(port) << "--disable-rpc-login";
#ifdef Q_OS_WIN
    program = QApplication::applicationDirPath() + "/xmrig.exe"; // edited
#elif defined(Q_OS_UNIX)
    program = QApplication::applicationDirPath() + "/xmrig";   // edited
#endif

    qDebug()<<program;
    QProcess* m_daemon = new QProcess();
    // Start monerod
    bool started = m_daemon->startDetached(program, arguments);
    if (started == true)
    {
        setRpcAddress("127.0.0.1",port);
        return true;
    }

    return false;
}

bool RpcManager::stopXmrig()
{
    // Check if daemon is running every 2 seconds. Kill if still running after 10 seconds
    int counter = 0;
    QThread::sleep(2);
    counter++;
    qDebug() << "Daemon still running.  " << counter;
    qDebug() << "Killing it! ";
#ifdef Q_OS_WIN
                QProcess::execute("taskkill /F /IM xmrig.exe");   // edited monerod
#else
                QProcess::execute("pkill xmrig");
#endif

    return true;
}


bool RpcManager::setRpcAddress(const std::string& ip,int port)
{
    PoolMiningManager *ptr = m_miningManager.get();
    if (!ptr)
    {
        return false;
    }
    ptr->setMiningRpcAddress(ip,port);

    return true;
}

double RpcManager::miningHashRate() const
{
    PoolMiningManager *ptr = m_miningManager.get();
    if (!ptr)
    {
        return 0;
    }

    return ptr->miningHashRate();
}

bool RpcManager::isMining() const
{
    PoolMiningManager *ptr = m_miningManager.get();
    if (!ptr)
    {
        return false;
    }

    return ptr->isMining();
}

bool RpcManager::startMining(const QString &config, quint32 threads)
{
    MINING_CONFIG mreq;
    if (threads == 0) {
        threads = 1;
    }
    mreq.threads_count = threads;
    QJsonParseError json_error;
    QByteArray jsonstr(qPrintable(config));
    QJsonDocument doc = QJsonDocument::fromJson(jsonstr, &json_error);
    if (!doc.isNull() && (json_error.error == QJsonParseError::NoError)) {
        if (doc.isObject()) {
           QJsonObject object = doc.object();
           if (object.contains("algorithm")) {
               QJsonValue algo_value = object.value("algorithm");
               if (algo_value.isString()) {
                   mreq.mining_algorithm = algo_value.toString().toUtf8().constData();
               }
           }
           if (object.contains("pool")) {
               QJsonValue pool_value = object.value("pool");
               if (pool_value.isString()) {
                   mreq.pool_server_address = pool_value.toString().toUtf8().constData();
               }
           }
           if (object.contains("port")) {
                QJsonValue svr_port_value = object.value("port");
                if (svr_port_value.isDouble()) {
                    int portnum = svr_port_value.toDouble();
                    mreq.pool_port = portnum;
                }
           }
           if (object.contains("user")) {
               QJsonValue usr_value = object.value("user");
               if (usr_value.isString()) {
                   mreq.wallet_address = usr_value.toString().toUtf8().constData();
               }
           }
           if (object.contains("password")) {
               QJsonValue passwd_value = object.value("password");
               if (passwd_value.isString()) {
                   mreq.wallet_password = passwd_value.toString().toUtf8().constData();
               }
           }
        }
    }

    PoolMiningManager *ptr = m_miningManager.get();
    if (!ptr)
    {
        return false;
    }

    return ptr->startMining(mreq);
}

bool RpcManager::stopMining()
{
    PoolMiningManager *ptr = m_miningManager.get();
    if (!ptr)
    {
        return false;
    }

    return ptr->stopMining();
}

bool RpcManager::connected() const {

    return true;
}

bool RpcManager::running() {
   
   std::string status="CANCEL"; 
   PoolMiningManager *ptr = m_miningManager.get();
   if (!ptr)
    {
        return false;
    }

    return ptr->isRuning();
}

RpcManager::RpcManager(QObject *parent) : QObject(parent)
{
    if (!m_miningManager.get()) {
         m_miningManager = (std::shared_ptr<PoolMiningManager>) std::make_shared<PoolMiningManager>();
    }
}
