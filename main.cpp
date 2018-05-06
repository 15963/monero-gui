// Copyright (c) 2014-2015, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QStandardPaths>
#include <QDebug>
#include <QObject>
#include <boost/program_options.hpp>
#include "clipboardAdapter.h"
#include "filter.h"
#include "oscursor.h"
#include "oshelper.h"
#include "WalletManager.h"
#include "Wallet.h"
#include "QRCodeImageProvider.h"
#include "PendingTransaction.h"
#include "UnsignedTransaction.h"
#include "TranslationManager.h"
#include "TransactionInfo.h"
#include "TransactionHistory.h"
#include "model/TransactionHistoryModel.h"
#include "model/TransactionHistorySortFilterModel.h"
#include "AddressBook.h"
#include "model/AddressBookModel.h"
#include "wallet/wallet2_api.h"
#include "MainApp.h"
#include "dohttp.h"
#include "systemtray.h"
#include "autostart.h"
#include "currentinfo.h"
#include "rpcmanager.h"
#include "autorunmanager.h"
#include "misc_log_ex.h"


#include <QThread>

// IOS exclusions
#ifndef Q_OS_IOS
#include "daemon/DaemonManager.h"
#endif

#ifdef WITH_SCANNER
#include "QrCodeScanner.h"
#endif

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Send all message types to logger
    Monero::Wallet::debug(msg.toStdString());
}

int main(int argc, char *argv[])
{
//    // Enable high DPI scaling on windows & linux
//#if !defined(Q_OS_ANDROID) && QT_VERSION >= 0x050600
//    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    qDebug() << "High DPI auto scaling - enabled";
//#endif

  //mlog_configure(mlog_get_default_log_path("rsscp.log"), true);
  mlog_set_log_level(2);
    
  if (argc == 1) {
        MGINFO("param argv[0]: " << argv[0]);
  }

  MGINFO("Rcssp starting ...");

  bool isAutoStart = false; 
  std::string configPath;
  if ( argc > 2 ) {
        MGINFO("Rcssp is auto starting ...");
        namespace po = boost::program_options;
        po::options_description desc_params_help("Rsscp options");
        desc_params_help.add_options()
        ("help","produce help message")
        ("start", "set Rsscp auto startup")
        ("config", po::value<string>(&configPath),"set progame config path")
        ("s", "set Rsscp auto startup")
        ("c", po::value<string>(&configPath),"set progame config path");
        po::variables_map vm;          
        po::store(po::parse_command_line(argc, argv, desc_params_help), vm);  
        po::notify(vm);  
            
        if (vm.count("help"))
        {
            std::cout<< desc_params_help <<std::endl;
            return 1; 
        }
        if (vm.count("start") || vm.count("s"))
        {
                isAutoStart = true;
        }
        if (isAutoStart) {
            MGINFO("Rcssp param --start is true");
        } else {
            MGINFO("Rcssp param --start is false");
        }
        
        MGINFO("Rcssp param --config :"<<configPath);

  }
  //isAutoStart = true;
  //configPath = "/Users/axis/Rcssp/currentInfo/";
  
  if (isAutoStart) {

      MainApp app(argc, argv);
      qDebug() << "app auto start startd";
      CurrentInfo currentInfo;
      currentInfo.path = QString::fromLocal8Bit(configPath.c_str()); 
      int miningType = currentInfo.getCurrentType(); 
       QVector<QString> params(3);
      if (miningType == RUN_POOL) {
         params[0]=currentInfo.getCurrentPoolInfo(); 
         MGINFO("Rcssp auto start pool mining:"<<string((const char *)params[0].toLocal8Bit())); 
         AutoRunManager::instance()->setMiningParam(params, miningType);          
      } else if (miningType == RUN_NODE) {
         params[0]=currentInfo.getCurrentNodeInfo(); 
         MGINFO("Rcssp auto start node mining:"<<string((const char *)params[0].toLocal8Bit())); 
         AutoRunManager::instance()->setMiningParam(params, miningType);  
      } else if (miningType == RUN_BOTH) {
         params[0]=currentInfo.getCurrentPoolInfo(); 
         MGINFO("Rcssp auto start pool mining:"<<string((const char *)params[0].toLocal8Bit())); 
         params[1]=currentInfo.getCurrentNodeInfo(); 
         MGINFO("Rcssp auto start node mining:"<<string((const char *)params[1].toLocal8Bit())); 
         AutoRunManager::instance()->setMiningParam(params, miningType);    
      }
      if (miningType != RUN_NOTH) {
          MGINFO("Rcssp AutoRunManager::instance()->start");
          AutoRunManager::instance()->start();
      }

     // return app.exec();
  }


    // Log settings
    Monero::Wallet::init(argv[0], "，Rcssp");
//    qInstallMessageHandler(messageHandler);

    MainApp app(argc, argv);

    qDebug() << "app startd";

    // app.setApplicationName("，monero-core");
    // app.setOrganizationDomain("getmonero.org");
    // app.setOrganizationName("，monero-project");

    filter *eventFilter = new filter;
    app.installEventFilter(eventFilter);

    // registering types for QML
    qmlRegisterType<clipboardAdapter>("moneroComponents.Clipboard", 1, 0, "Clipboard");

    qmlRegisterUncreatableType<Wallet>("moneroComponents.Wallet", 1, 0, "Wallet", "Wallet can't be instantiated directly");


    qmlRegisterUncreatableType<PendingTransaction>("moneroComponents.PendingTransaction", 1, 0, "PendingTransaction",
                                                   "PendingTransaction can't be instantiated directly");

    qmlRegisterUncreatableType<UnsignedTransaction>("moneroComponents.UnsignedTransaction", 1, 0, "UnsignedTransaction",
                                                   "UnsignedTransaction can't be instantiated directly");

    qmlRegisterUncreatableType<WalletManager>("moneroComponents.WalletManager", 1, 0, "WalletManager",
                                                   "WalletManager can't be instantiated directly");

    qmlRegisterUncreatableType<RpcManager>("moneroComponents.RpcManager", 1, 0, "RpcManager",
                                                   "RpcManager can't be instantiated directly");

    qmlRegisterUncreatableType<TranslationManager>("moneroComponents.TranslationManager", 1, 0, "TranslationManager",
                                                   "TranslationManager can't be instantiated directly");



    qmlRegisterUncreatableType<TransactionHistoryModel>("moneroComponents.TransactionHistoryModel", 1, 0, "TransactionHistoryModel",
                                                        "TransactionHistoryModel can't be instantiated directly");

    qmlRegisterUncreatableType<TransactionHistorySortFilterModel>("moneroComponents.TransactionHistorySortFilterModel", 1, 0, "TransactionHistorySortFilterModel",
                                                        "TransactionHistorySortFilterModel can't be instantiated directly");

    qmlRegisterUncreatableType<TransactionHistory>("moneroComponents.TransactionHistory", 1, 0, "TransactionHistory",
                                                        "TransactionHistory can't be instantiated directly");

    qmlRegisterUncreatableType<TransactionInfo>("moneroComponents.TransactionInfo", 1, 0, "TransactionInfo",
                                                        "TransactionHistory can't be instantiated directly");
#ifndef Q_OS_IOS
    qmlRegisterUncreatableType<DaemonManager>("moneroComponents.DaemonManager", 1, 0, "DaemonManager",
                                                   "DaemonManager can't be instantiated directly");
#endif
    qmlRegisterUncreatableType<AddressBookModel>("moneroComponents.AddressBookModel", 1, 0, "AddressBookModel",
                                                        "AddressBookModel can't be instantiated directly");

    qmlRegisterUncreatableType<AddressBook>("moneroComponents.AddressBook", 1, 0, "AddressBook",
                                                        "AddressBook can't be instantiated directly");

    qRegisterMetaType<PendingTransaction::Priority>();
    qRegisterMetaType<TransactionInfo::Direction>();
    qRegisterMetaType<TransactionHistoryModel::TransactionInfoRole>();

#ifdef WITH_SCANNER
    qmlRegisterType<QrCodeScanner>("moneroComponents.QRCodeScanner", 1, 0, "QRCodeScanner");
#endif

    QQmlApplicationEngine engine;

    // http get pools and nodes
    Dohttp dohttp;
    engine.rootContext()->setContextProperty("dohttp", &dohttp);
    dohttp.fetch_nodes();
    dohttp.fetch_pools();

    // system tray
    SystemTray systemTray;
    engine.rootContext()->setContextProperty("systemTray", &systemTray);

    OSCursor cursor;
    engine.rootContext()->setContextProperty("globalCursor", &cursor);
    OSHelper osHelper;
    engine.rootContext()->setContextProperty("oshelper", &osHelper);

    engine.rootContext()->setContextProperty("walletManager", WalletManager::instance());

    engine.rootContext()->setContextProperty("rpcManager", RpcManager::instance());

    engine.rootContext()->setContextProperty("translationManager", TranslationManager::instance());

    engine.addImageProvider(QLatin1String("qrcode"), new QRCodeImageProvider());
    const QStringList arguments = QCoreApplication::arguments();

    engine.rootContext()->setContextProperty("mainApp", &app);

// Exclude daemon manager from IOS
#ifndef Q_OS_IOS
    DaemonManager * daemonManager = DaemonManager::instance(&arguments);
    engine.rootContext()->setContextProperty("daemonManager", daemonManager);
#endif

//  export to QML monero accounts root directory
//  wizard is talking about where
//  to save the wallet file (.keys, .bin), they have to be user-accessible for
//  backups - I reckon we save that in My Documents\Monero Accounts\ on
//  Windows, ~/Monero Accounts/ on nix / osx
    bool isWindows = false;
    bool isIOS = false;
    bool isMac = false;
    int type = -1;
#ifdef Q_OS_WIN
    isWindows = true;
    type = 0;
    QStringList moneroAccountsRootDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
#elif defined(Q_OS_IOS)
    isIOS = true;
    type = 1;
    QStringList moneroAccountsRootDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
#elif defined(Q_OS_UNIX)
    type = 2;
    QStringList moneroAccountsRootDir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
#endif
#ifdef Q_OS_MAC
    type = 3;
    isMac = true;
#endif

    bool is32 = false;
#ifdef Q_OS_DARWIN32 | Q_OS_WIN32
    is32 = true;
#endif

    engine.rootContext()->setContextProperty("isAutoStart", isAutoStart);
    engine.rootContext()->setContextProperty("isWindows", isWindows);
    engine.rootContext()->setContextProperty("isIOS", isIOS);
    engine.rootContext()->setContextProperty("is32", is32);

    if (!moneroAccountsRootDir.empty()) {
        QString moneroAccountsDir = moneroAccountsRootDir.at(0) + "/Rcssp/wallets";
        engine.rootContext()->setContextProperty("moneroAccountsDir", moneroAccountsDir);
    }

    QString path =  moneroAccountsRootDir.at(0) + "/Rcssp/currentInfo/";
    CurrentInfo currentInfo;
    currentInfo.path = path;
    engine.rootContext()->setContextProperty("currentInfo", &currentInfo);


    AutoStart autoStart;
    autoStart.init(type,path);

    // Get default account name
    QString accountName = qgetenv("USER"); // mac/linux
    if (accountName.isEmpty()){
        accountName = qgetenv("USERNAME"); // Windows
    }
    if (accountName.isEmpty()) {
        accountName = "My monero Account";
    }

    engine.rootContext()->setContextProperty("defaultAccountName", accountName);
    engine.rootContext()->setContextProperty("applicationDirectory", QApplication::applicationDirPath());

    bool builtWithScanner = false;
#ifdef WITH_SCANNER
    builtWithScanner = true;
#endif
    engine.rootContext()->setContextProperty("builtWithScanner", builtWithScanner);

    // Load main window (context properties needs to be defined obove this line)
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    QObject *rootObject = engine.rootObjects().first();

#ifdef WITH_SCANNER
    QObject *qmlCamera = rootObject->findChild<QObject*>("qrCameraQML");
    if( qmlCamera ){
        qDebug() << "QrCodeScanner : object found";
        QCamera *camera_ = qvariant_cast<QCamera*>(qmlCamera->property("mediaObject"));
        QObject *qmlFinder = rootObject->findChild<QObject*>("QrFinder");
        qobject_cast<QrCodeScanner*>(qmlFinder)->setSource(camera_);
    } else {
        qDebug() << "QrCodeScanner : something went wrong !";
    }
#endif

    QObject::connect(eventFilter, SIGNAL(sequencePressed(QVariant,QVariant)), rootObject, SLOT(sequencePressed(QVariant,QVariant)));
    QObject::connect(eventFilter, SIGNAL(sequenceReleased(QVariant,QVariant)), rootObject, SLOT(sequenceReleased(QVariant,QVariant)));
    QObject::connect(eventFilter, SIGNAL(mousePressed(QVariant,QVariant,QVariant)), rootObject, SLOT(mousePressed(QVariant,QVariant,QVariant)));
    QObject::connect(eventFilter, SIGNAL(mouseReleased(QVariant,QVariant,QVariant)), rootObject, SLOT(mouseReleased(QVariant,QVariant,QVariant)));


    return app.exec();
}
