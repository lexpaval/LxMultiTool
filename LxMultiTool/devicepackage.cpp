#include "devicepackage.h"

// Define our static variables
QString DevicePackage::deviceName;
QString DevicePackage::download_link;
QString DevicePackage::xda_link;
QString DevicePackage::unlock_cmd;
QString DevicePackage::lock_cmd;

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

    // Open the temporary version file
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "File Error", file.errorString());
    }

    // Read the file as XML format
    QXmlStreamReader xml(&file);

    // Start parsing the XML
    while (!xml.atEnd()) {
        xml.readNext();

        // We store all the stuff in variables
        if(xml.name() == "device")
        {
            deviceName = xml.readElementText();
        }
        else if (xml.name() == "downloads_link")
        {
            download_link = xml.readElementText();
        }
        else if (xml.name() == "xda_link")
        {
            xda_link = xml.readElementText();
        }
        else if (xml.name() == "unlock")
        {
            unlock_cmd = xml.readElementText();
        }
        else if (xml.name() == "lock")
        {
            lock_cmd = xml.readElementText();
        }
    }
    if (xml.hasError()) {
        QMessageBox::critical(0, "XML Error", xml.errorString());
    }

    // Close the reader
    xml.clear();
    // Close the file
    file.close();
}

QString DevicePackage::getDevicePackage()
{
    return deviceName;
}

QString DevicePackage::getDownloadLink()
{
    return download_link;
}

QString DevicePackage::getXdaLink()
{
    return xda_link;
}

QString DevicePackage::getUnlockCommand()
{
    return unlock_cmd;
}

QString DevicePackage::getLockCommand()
{
    return lock_cmd;
}
