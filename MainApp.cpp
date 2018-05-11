#include "MainApp.h"
#include <QCloseEvent>
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>



#define TIME_OUT                (500)    // 500ms

MainApp::MainApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , _isRunning(false)
    , _localServer(NULL) {
     _serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

    _initLocalConnection();
}

bool MainApp::isRunning() {
    return _isRunning;
}

void MainApp::_newLocalConnection() {
    QLocalSocket *socket = _localServer->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;
        socket = NULL;
    }
}

void MainApp::_initLocalConnection() {
    _isRunning = false;

    QLocalSocket socket;
    socket.connectToServer(_serverName);
    if(socket.waitForConnected(TIME_OUT)) {
        fprintf(stderr, "%s already running.\n",
                _serverName.toLocal8Bit().constData());
        _isRunning = true;
         return;
    }

     _newLocalServer();
}

void MainApp::_newLocalServer() {
    _localServer =  new QLocalServer(this);
    connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
    if(!_localServer->listen(_serverName)) {
         if(_localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(_serverName);
            _localServer->listen(_serverName);
        }
    }
}

bool MainApp::event (QEvent *event)
{
    // Catch application exit event and signal to qml app to handle exit
    if(event->type() == QEvent::Close) {
        event->ignore();
        emit closing();
        return true;
    }

    return false;
}
