#ifndef DEVICECONNECTION_H
#define DEVICECONNECTION_H

#include <QProcess>
#include <QCoreApplication>
#include <QDir>

#define NONE 0
#define ADB 1
#define FASTBOOT 2

class DeviceConnection
{
public:
    static int getConnection();
};

#endif // DEVICECONNECTION_H
