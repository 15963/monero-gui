#include "autorunmanager.h"

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
#include <boost/thread.hpp>

#include "daemon/DaemonManager.h"
#include "misc_log_ex.h"

AutoRunManager * AutoRunManager::m_instance = nullptr;

AutoRunManager *AutoRunManager::instance()
{
    if (!m_instance) {
        m_instance = new AutoRunManager;
    }

    return m_instance;
}



AutoRunManager::AutoRunManager(QObject *parent) : QObject(parent),
m_parameter(""),
m_runType(0)
{

}


void AutoRunManager::parsePoolInfor(QString& infor) {
    QString infor_cont =infor;
    QRegExp tagExp(":");
    QStringList paramList = infor_cont.split(tagExp);
    if (paramList.length() > 0) {    
        QString pool_address = paramList.at(0);
        QString pool_port = paramList.at(1);
        QString wallet_address = paramList.at(2);
        QString threads = paramList.at(3);
        m_pool_config = ""; 
        m_pool_config = "{\"algorithm\":\"cryptonight\","; 
        m_pool_config += "\"pool\":";
        m_pool_config += "\""; 
        m_pool_config += pool_address;
        m_pool_config += "\",";
        m_pool_config += "\"port\":";
        m_pool_config += pool_port;
        m_pool_config += ",\"user\":\"";
        m_pool_config +=  wallet_address;
        m_pool_config += "\",\"password\":\"x\"}";

        qDebug()<<m_pool_config;

        if (threads.isEmpty()) {
            m_threads = 1; 
        } else {
            m_threads = atoi(qPrintable(threads));
        }            
    }      
}

void AutoRunManager::parseNodeInfor(QString& infor) {
    QString infor_cont =infor;
    QRegExp tagExp(":");
    QStringList paramList = infor_cont.split(tagExp);
    if (paramList.length() > 0) {    
        m_node_address = paramList.at(0);
        m_node_address += ":";
        m_node_address += paramList.at(1);
        m_wallet_address = paramList.at(2);
        QString threads = paramList.at(3);
        if (threads.isEmpty()) {
            m_threads = 1; 
        } else {
            m_threads = atoi(qPrintable(threads));
        }                    
    }      
}

void AutoRunManager::setMiningParam(QVector<QString>& params,int runType) {
    m_runType = runType;
    if (runType == 1){
       m_parameter = params[0];       
       parsePoolInfor(m_parameter); 
    } else if (runType == 2) {
       m_parameter = params[0];
       parseNodeInfor(m_parameter); 
    } else if (runType == 3) {
       QString pool_info = params[0]; 
       parsePoolInfor(pool_info); 
       QString node_info = params[1]; 
       parseNodeInfor(node_info); 
    }
}

bool AutoRunManager::startXmrigMining() {
    MGINFO("Rcssp auto starting xmrig ...");
    bool bret = false; 
    if (RpcManager::instance()->isMining())     
    {
        return true; 
    }

    RpcManager::instance()->stopXmrig();
    if (RpcManager::instance()->startXmrig(7777)) {
        MGINFO("Rcssp auto xmrig started ok.");
        QThread::sleep(2);
        bret = RpcManager::instance()->startMining(m_pool_config,(quint32)m_threads);
        if (bret == false) {
             RpcManager::instance()->stopXmrig();
             QThread::sleep(2);
             if (RpcManager::instance()->startXmrig(7777)) {
                 QThread::sleep(2);
                 bret = RpcManager::instance()->startMining(m_pool_config,(quint32)m_threads);
             }
        } else {
            bret = true;
        }

    } else {
        bret = false; 
    }
   
    if (bret==true) {
        MGINFO("Rcssp auto xmrig start mining ok.");
        boost::thread thr (boost::bind(&AutoRunManager::get_mining_hashrate,this));
        thr.detach();
    }

    return bret;      
}

void* AutoRunManager::start_mining(void* p) {
    AutoRunManager *ptr = (AutoRunManager*)p;
    bool bret = false;
    for (;;) {
      MGINFO("Rcssp auto start rrncd mining thread try to connect daemon.");
      bret = WalletManager::instance()->startMining(ptr->getWalletAddress(),(quint32)ptr->getThreadCount(),true,true);
      if (bret) {
          MGINFO("Rcssp auto start rrncd mining ok.");
          break;
      }
      QThread::sleep(30); // waiting 30 seconds
    }

    return NULL;
}

void* AutoRunManager::get_mining_hashrate(void*p)
{
    AutoRunManager *ptr = (AutoRunManager*)p;
    int runtype = ptr->getAutoRunType(); 
    char buff[5] = {0}; 
    double nrate = 0.0;
    for(;;) {
       if (runtype == 1) {
           nrate = RpcManager::instance()->miningHashRate();
           printf("hashrate: %3.1f H/\s\n",nrate);
           sprintf(buff,"%3.1f H/s", nrate); 
           MGINFO("Rcssp auto xmrig hash rate:\n"<< buff);
        } else {
           nrate = WalletManager::instance()->miningHashRate();
           printf("hashrate: %3.1f H/\s\n",nrate);
           sprintf(buff,"%3.1f H/s", nrate); 
           MGINFO("Rcssp auto daemon hash rate:\n"<< buff);
        }
      
       QThread::sleep(10);
    }

    return NULL;
}

bool AutoRunManager::startDaemonMining() {
    bool bret = false; 
    MGINFO("Rcssp auto start daemon ");
    RpcManager::instance()->stopRrncd();
    //bool daem_started = RpcManager::instance()->startRrncd(); 
     QStringList argument_autostart; 
     argument_autostart << "Rsscp";    
     bool daem_started = DaemonManager::instance(&argument_autostart)->start("",false,""); 
    if (daem_started) {
        MGINFO("Rcssp auto start daemon ok.");
        WalletManager::instance()->setDaemonAddress(m_node_address);
        MGINFO("Rcssp auto start rrncd mining thread.");
        QThread::sleep(20);
        boost::thread thr (boost::bind(&AutoRunManager::start_mining,this));
        thr.detach();
    } else {
        MGINFO("Rcssp auto start daemon failed.");
    }
   
    return bret; 
}

bool AutoRunManager::start() {

   bool bothStarted = false; 
   if (m_runType == 4) //noth
   {
       MGINFO("Rcssp auto run start xmring and daemon mining.");
       return false; 
   } 

   if (m_runType == 3) //both
   {
        MGINFO("Rcssp auto run start xmring and daemon mining.");
        bothStarted = startXmrigMining();
        if (bothStarted == true) {           
            bothStarted = startDaemonMining();
            if (!bothStarted) {
                bothStarted = RpcManager::instance()->stopXmrig();
            }
        }

        return bothStarted; 
   } 
   else if (m_runType == 2)//node
   {
      MGINFO("Rcssp auto run start daemon mining for rrncd rpc.");
      return startDaemonMining();
   } 
   else if (m_runType == 1) //pool
   {
     MGINFO("Rcssp auto run start xmring mining for xmrig rpc.");
     return startXmrigMining();      
   }

   return true; 
}

bool AutoRunManager::stop()
{
   bool bothstoped =false; 
   if (m_runType == 3) //both
   {
        bothstoped = RpcManager::instance()->stopXmrig();
        if (bothstoped) {            
            bothstoped = RpcManager::instance()->stopRrncd();
        }
       
        return bothstoped;  
   } 
   else if (m_runType == 2)//node
   {
      return RpcManager::instance()->stopRrncd();
   } 
   else if (m_runType == 1) //pool
   {
      return  RpcManager::instance()->stopXmrig();    
   }

   return true; 
}
