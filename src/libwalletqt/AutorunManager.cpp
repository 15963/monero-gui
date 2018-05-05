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

AutoRunManager * AutoRunManager::m_instance = nullptr;

AutoRunManager *AutoRunManager::instance()
{
    if (!m_instance) {
        m_instance = new AutoRunManager;
    }

    return m_instance;
}



RpcManager::RpcManager(QObject *parent) : QObject(parent)
{

}
