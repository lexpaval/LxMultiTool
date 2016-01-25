#include "deviceconnection.h"

int DeviceConnection::getConnection(int timeout)
{
    int connection = NONE;
    QProcess process_adb;
    QProcess process_fastboot;
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    process_adb.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));

#ifdef Q_OS_WIN
    // Windows code here
    process_adb.start("cmd");
    process_adb.write("adb.exe devices\n");
#elif defined(Q_OS_MACX)
    // MAC code here
    process_adb.start("sh");
    process_adb.write("./adb_mac devices\n");
#else
    // Linux code here
    process_adb.start("sh");
    process_adb.write("./adb_linux devices\n");
#endif

    process_adb.write("exit\n");
    process_adb.waitForFinished(timeout); // sets current thread to sleep and waits for process end

    QString adb(process_adb.readAllStandardOutput());

    if(adb.contains("\tdevice"))
    {
        connection = ADB;
    }
    else if (adb.contains("\trecovery"))
    {
        connection = ADB_RECOVERY;
    }
    else if (adb.contains("\tsideload"))
    {
        connection = ADB_SIDELOAD;
    }
    else
    {

        process_fastboot.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));

#ifdef Q_OS_WIN
        // Windows code here
        process_fastboot.start("cmd");
        process_fastboot.write("fastboot.exe devices\n");
#elif defined(Q_OS_MACX)
        // MAC code here
        process_fastboot.start("sh");
        process_fastboot.write("./fastboot_mac devices\n");
#else
        // Linux code here
        process_fastboot.start("sh");
        process_fastboot.write("./fastboot_linux devices\n");
#endif

        process_fastboot.write("exit\n");
        process_fastboot.waitForFinished(timeout);

        QString fastboot(process_fastboot.readAllStandardOutput());

        if(fastboot.contains("\tfastboot"))
        {
            connection = FASTBOOT;
        }
    }

    return connection;
}
