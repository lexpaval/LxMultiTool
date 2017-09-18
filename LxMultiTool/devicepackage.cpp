#include "devicepackage.h"

// Define our static variables
QString DevicePackage::deviceName   = "None";
QString DevicePackage::options      = "2";
QString DevicePackage::downloadLink;
QString DevicePackage::xdaLink;
QString DevicePackage::versionLink;
QString DevicePackage::unlockCmd;
QString DevicePackage::lockCmd;

QString DevicePackage::getDeviceName()
{
    return deviceName;
}

QString DevicePackage::getDownloadLink()
{
    return downloadLink;
}

QString DevicePackage::getXdaLink()
{
    return xdaLink;
}

QString DevicePackage::getVersionLink()
{
    return versionLink;
}

QString DevicePackage::getUnlockCmd()
{
    return unlockCmd;
}

QString DevicePackage::getLockCmd()
{
    return lockCmd;
}

QString DevicePackage::getOptions()
{
    return options;
}

void DevicePackage::parseDevicePackage()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    QFile file(temp_path.absolutePath()+"/Data/DevicePackage.xml");

    // Open the temporary version file if possible
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Device package file Error", file.errorString());
    }
    else
    {
        // Read the file as XML format
        QXmlStreamReader xml(&file);

        // Start parsing the XML
        while (!xml.atEnd()) {
            xml.readNext();

            // We store all the stuff in variables
            if (xml.name() == "device")
                DevicePackage::deviceName = xml.readElementText();
            else if (xml.name() == "options")
                DevicePackage::options = xml.readElementText();
            else if (xml.name() == "downloads_link")
                DevicePackage::downloadLink = xml.readElementText();
            else if (xml.name() == "xda_link")
                DevicePackage::xdaLink = xml.readElementText();
            else if (xml.name() == "version_link")
                DevicePackage::versionLink = xml.readElementText();
            else if (xml.name() == "unlock")
                DevicePackage::unlockCmd = xml.readElementText();
            else if (xml.name() == "lock")
                DevicePackage::lockCmd = xml.readElementText();

        }
        if (xml.hasError())
        {
            QMessageBox::critical(0, "XML Error", xml.errorString());
        }

        // Close the reader
        xml.clear();
        // Close the file
        file.close();
    }
}
