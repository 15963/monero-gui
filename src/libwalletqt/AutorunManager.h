#ifndef AUTORUNMANAGER_H
#define AUTORUNMANAGER_H

#include "WalletManager.h"
#include "rpcmanager.h"

class AutoRunManager : public QObject
{
    Q_OBJECT

public:
    static AutoRunManager * instance();
    static void* start_mining(void* p);
    static void* get_mining_hashrate(void*p);
    Q_INVOKABLE void setMiningParam(QVector<QString>& params,int runType);
    Q_INVOKABLE bool start();
    Q_INVOKABLE bool stop();
    QString& getWalletAddress(){ return m_wallet_address; }
    int getThreadCount(){ return m_threads; }
    int getAutoRunType(){ return m_runType; }
private:
    explicit AutoRunManager(QObject *parent = 0);
    bool startXmrigMining(); 
    bool startDaemonMining(); 
    void parsePoolInfor(QString& infor); 
    void parseNodeInfor(QString& infor); 
    static AutoRunManager * m_instance;
    QString m_parameter; 
    QString m_node_address;
    QString m_wallet_address;
    QString m_pool_config; 
    int     m_threads;
    int     m_runType; 
};

#endif // AUTORUNMANAGER_H
