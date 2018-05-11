#include "qsingleapplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT                (500)    // 500ms

SingleApplication::SingleApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , _isRunning(false)
    , _localServer(NULL) {
    _serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

    _initLocalConnection();
}
/*
SingleApplication::~SingleApplication()
{
    if(_localServer != NULL)
    {
        delete _localServer;
        _localServer = NULL;
    }
}
*/
////////////////////////////////////////////////////////////////////////////////
// note:
// check the app , true - has false - none
////////////////////////////////////////////////////////////////////////////////
bool SingleApplication::isRunning() {
    return _isRunning;
}

////////////////////////////////////////////////////////////////////////////////
// note
// check it by localsocket
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_newLocalConnection() {
    QLocalSocket *socket = _localServer->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;
        socket = NULL;

    }
}

////////////////////////////////////////////////////////////////////////////////
// note:
// Single instance running through the socket communication program,
// Initialize local connection, if the connection is not on the server, create, or quit
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_initLocalConnection() {
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

////////////////////////////////////////////////////////////////////////////////
// note
// create the LocalServer
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::_newLocalServer() {
    _localServer =  new QLocalServer(this);
    connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
    if(!_localServer->listen(_serverName)) {
        // The failure monitoring, may be when the program crashes, caused by residual process service, remove it
        if(_localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(_serverName); // <-- important
            _localServer->listen(_serverName); // listen again
        }
    }
}
