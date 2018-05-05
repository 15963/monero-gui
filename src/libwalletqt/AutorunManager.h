#ifndef AUTORUNMANAGER_H
#define AUTORUNMANAGER_H

#include "WalletManager.h"
#include "rpcmanager.h"

class AutoRunManager : public QObject
{
    Q_OBJECT

public:

    static AutoRunManager * instance();
   // Q_INVOKABLE bool start();
   // Q_INVOKABLE bool stop();

private:
    explicit AutoRunManager(QObject *parent = 0);
    static AutoRunManager * m_instance;

};

#endif // AUTORUNMANAGER_H
