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
        if(is_auto_start)
        {
            QString application_path = QApplication::applicationFilePath();
            settings->setValue(application_name, application_path.replace("/", "\\"));
        }
        else
        {
            settings->remove(application_name);
        }
        delete settings;
    }
}
