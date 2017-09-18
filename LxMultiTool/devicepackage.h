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
    static QString options;
    static QString downloadLink;
    static QString xdaLink;
    static QString versionLink;
    static QString unlockCmd;
    static QString lockCmd;
public:
    static void parseDevicePackage();
    static QString getDeviceName();
    static QString getDownloadLink();
    static QString getXdaLink();
    static QString getVersionLink();
    static QString getUnlockCmd();
    static QString getLockCmd();
    static QString getOptions();
};

#endif // DEVICEPACKAGE_H
