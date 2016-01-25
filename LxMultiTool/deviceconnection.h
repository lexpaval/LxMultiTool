#ifndef DEVICECONNECTION_H
#define DEVICECONNECTION_H

#include <QProcess>
#include <QCoreApplication>
#include <QDir>

#define NONE 0
#define ADB 1
#define FASTBOOT 2
#define ADB_RECOVERY 3
#define ADB_SIDELOAD 4

#define DEFAULT_TIMEOUT 30000
#define FAST_TIMEOUT 100

class DeviceConnection
{
public:
    static int getConnection(int timeout);
};

#endif // DEVICECONNECTION_H
