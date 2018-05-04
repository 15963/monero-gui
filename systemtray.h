#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>

class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);
    ~SystemTray();
    void updateTrans();
signals:
    void signalIconActivated();
    void signalShow();
    void signalHide();
    void signalQuit();
    void signalShowActivated(QString strInfo);

private slots:
    // click on icon in tray
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    void hideIconTray();
    void qmlOut(QString strInfo);

private:
    // object for icon in tray
    QSystemTrayIcon *m_trayIcon;
    QAction *viewshow;
    QAction *viewhide;
    QAction *quit;
};

#endif // SYSTEMTRAY_H
