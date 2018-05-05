#include "autostart.h"
#include <QSettings>
#include <QApplication>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

AutoStart::AutoStart(QObject *parent) : QObject(parent)
{

}
void AutoStart::init(int type)
{
    if(type == 0) // only win
    {
        QString application_name = QApplication::applicationName();
        QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(application_name+ " -start -config", application_path.replace("/", "\\"));

        delete settings;
    }

}
