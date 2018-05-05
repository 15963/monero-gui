#ifndef AUTOSTART_H
#define AUTOSTART_H

#include <QObject>

class AutoStart : public QObject
{
    Q_OBJECT
public:
    explicit AutoStart(QObject *parent = nullptr);
    void init(int type,QString path);
signals:

public slots:
private:
    bool is_auto_start;
};

#endif // AUTOSTART_H
