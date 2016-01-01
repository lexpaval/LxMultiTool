#ifndef DEVICEPACKAGE_H
#define DEVICEPACKAGE_H

#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QDir>
#include <QMessageBox>
#include <QFile>

class DevicePackage
{
private:
    static QString deviceName;
    static QString download_link;
    static QString xda_link;
    static QString unlock_cmd;
    static QString lock_cmd;
public:
    static void parseDevicePackage();
    static QString getDevicePackage();
    static QString getDownloadLink();
    static QString getXdaLink();
    static QString getUnlockCommand();
    static QString getLockCommand();
};

#endif // DEVICEPACKAGE_H
