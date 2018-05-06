#include "autostart.h"
#include <QSettings>
#include <QApplication>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

AutoStart::AutoStart(QObject *parent) : QObject(parent)
{

}
void AutoStart::init(int type,QString path)
{
    if(type == 0) // only win
    {
        QString application_name = QApplication::applicationName();
        QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
        QString application_path = QApplication::applicationFilePath();

        QVariant exepath = settings->value(application_name); //读取注册表值
         if  (exepath.toString().length() <= 0)
         {
             QString application_path = QApplication::applicationFilePath();
             settings->setValue("\""+ application_name, application_path.replace("/", "\\")  + "\"" + " --start --config " + path);
         }


        delete settings;
    }

}
