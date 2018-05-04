#include "systemtray.h"
#include <QDebug>
#include <QMenu>

SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    // create context menu
    QMenu *trayIconMenu = new QMenu();

    viewshow = new QAction(tr("viewshow"), this);
    viewhide = new QAction(tr("viewhide"), this);
    quit = new QAction(tr("quit"), this);

    // connect signals menu to signals QML
    connect(viewshow, &QAction::triggered, this, &SystemTray::signalShow);
    connect(viewhide, &QAction::triggered, this, &SystemTray::signalHide);
    connect(quit, &QAction::triggered, this, &SystemTray::signalQuit);

    trayIconMenu->addAction(viewshow);
    trayIconMenu->addAction(viewhide);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quit);

    // init tray icon and init show hidden message
    m_trayIcon = new QSystemTrayIcon();
    m_trayIcon->setContextMenu(trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/images/trash.png"));
    m_trayIcon->show();

    // connect tray icon click signal to slot
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

SystemTray::~SystemTray()
{
    qDebug() << "System tray destroy";
}

void SystemTray::updateTrans(){
    viewshow->setText(QObject::tr("viewshow"));
    viewhide->setText(QObject::tr("viewhide"));
    quit->setText(tr("quit"));
}
// click on icon in tray
void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){

    case QSystemTrayIcon::Trigger:
        // icon clicked, send to QML layout
        emit signalIconActivated();
        break;
    default:
        break;
    }
}


 void SystemTray::qmlOut(QString strInfo)
 {
     updateTrans();
 }


void SystemTray::hideIconTray()
{
    m_trayIcon->hide();
}
