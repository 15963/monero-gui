#ifndef MAINAPP_H
#define MAINAPP_H

#include <QObject>
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QWidget>


class MainApp : public QApplication
{
    Q_OBJECT
public:
    MainApp(int &argc, char** argv);

    bool isRunning();                // 是否已經有实例在运行

private slots:
    // 有新连接时触发
    void _newLocalConnection();

private:
    // 初始化本地连接
    void _initLocalConnection();
    // 创建服务端
    void _newLocalServer();

    bool _isRunning;                // 是否已經有实例在运行
    QLocalServer *_localServer;     // 本地socket Server
    QString _serverName;            // 服务名称
    bool event(QEvent *e);
signals:
    void closing();
};

#endif // MAINAPP_H


