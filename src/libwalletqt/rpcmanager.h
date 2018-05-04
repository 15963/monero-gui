#ifndef RPCMANAGER_H
#define RPCMANAGER_H

#include <QObject>
#include <QPointer>

#include "pool_mining_manager.h"

class RpcManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected)

public:

    static RpcManager * instance();

    Q_INVOKABLE bool connected() const;
    Q_INVOKABLE double miningHashRate() const;
    Q_INVOKABLE bool isMining() const;
    Q_INVOKABLE bool startMining(const QString &config, quint32 threads);
    Q_INVOKABLE bool stopMining();
    Q_INVOKABLE bool setRpcAddress(const std::string& ip,int port);
    Q_INVOKABLE bool startXmrig(int port);
    Q_INVOKABLE bool stopXmrig();

private:
    explicit RpcManager(QObject *parent = 0);
    static RpcManager * m_instance;
    std::shared_ptr<PoolMiningManager> m_miningManager;
};

#endif // RPCMANAGER_H
