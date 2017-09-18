#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Parse the device package file at the start
    DevicePackage::parseDevicePackage();

    // Check the options according to our device package
    parseDeviceOptions();

    // Specify our device package
    ui->statusBar->addPermanentWidget(new QLabel(QString("Package: "+DevicePackage::getDeviceName().toLatin1())));

    // Set our version in the status bar
    ui->statusBar->addPermanentWidget(new QLabel(QString("Version: %1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH)));

    // Check our reboot options
    checkOptions(DeviceConnection::getConnection(FAST_TIMEOUT));

    // Get the name of the device
    getDeviceName();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDeviceName()
{
    int connection = DeviceConnection::getConnection(DEFAULT_TIMEOUT);

    if(connection == ADB)
    {
        QProcess process;
        QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
        // Because apple likes it's application folders
        temp_path.cdUp();
        temp_path.cdUp();
        temp_path.cdUp();
#endif

        process.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
        // Windows code here
        process.start("cmd");
        process.write("adb.exe shell getprop\n");
#elif defined(Q_OS_MACX)
        // MAC code here
        process.start("sh");
        process.write("./adb_mac shell getprop\n");
#else
        // Linux code here
        process.start("sh");
        process.write("./adb_linux shell getprop\n");
#endif

        process.write("exit\n");
        process.waitForFinished(); // sets current thread to sleep and waits for process end

        QString output(process.readAllStandardOutput());

        QString deviceName = "None";

        // Here we parse the chunk of text for the brand
        QRegularExpression rxlen("\\[ro.product.brand\\].*\\[(?<brand>[\\w\\d\\s]*)\\]");
        QRegularExpressionMatch match = rxlen.match(output);

        if(match.hasMatch())
        {
            deviceName = match.captured("brand")+" ";
        }

        // Here we parse the chunk of text for device
        rxlen.setPattern("\\[ro.product.device\\].*\\[(?<device>[\\w\\d\\s]*)\\]");
        match = rxlen.match(output);

        if(match.hasMatch())
        {
            deviceName += match.captured("device");
        }

        ui->label_device->setText(deviceName);
    }
    else if(connection == ADB_RECOVERY)
    {
        ui->label_device->setText("Recovery Device");
    }
    else if(connection == ADB_SIDELOAD)
    {
        ui->label_device->setText("Sideload Device");
    }
    else if(connection == FASTBOOT)
    {
        ui->label_device->setText("Fastboot Device");
    }
    else
    {
        ui->label_device->setText("None");
    }
}

void MainWindow::parseDeviceOptions()
{
    int option = DevicePackage::getOptions().toInt();

    switch(option)
    {
    case 0: // Not used
        break;

    case 1: // Default Nexus features
        break;

    case 2: // No lock/relock or return to stock
        ui->StockButton->setVisible(false);
        ui->UnlockButton->setVisible(false);
        ui->RelockButton->setVisible(false);
        break;

    default:
        break;

    }
}

void MainWindow::on_UnlockButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Prepare a messagebox
    QMessageBox msgBox(this);
    QPixmap unlock(":/Icons/unlock.png");
    msgBox.setIconPixmap(unlock);

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        msgBox.setText("You are about to unlock your bootloader.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            QProcess process;
            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            process.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process.start("cmd");
            process.write("fastboot.exe "+DevicePackage::getUnlockCmd().toLatin1()+"\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process.start("sh");
            process.write("./fastboot_mac "+DevicePackage::getUnlockCmd().toLatin1()+"\n");
#else
            // Linux code here
            process.start("sh");
            process.write("./fastboot_linux "+DevicePackage::getUnlockCmd().toLatin1()+"\n");
#endif

            process.write("exit\n");
            process.waitForFinished(); // sets current thread to sleep and waits for process end

            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Unlocking command performed.");
        }
    }
    else
    {
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_RelockButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Prepare a messagebox
    QMessageBox msgBox(this);
    QPixmap unlock(":/Icons/lock.png");
    msgBox.setIconPixmap(unlock);

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        msgBox.setText("You are about to Lock your bootloader.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            QProcess process;
            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            process.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process.start("cmd");
            process.write("fastboot.exe "+DevicePackage::getLockCmd().toLatin1()+"\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process.start("sh");
            process.write("./fastboot_mac "+DevicePackage::getLockCmd().toLatin1()+"\n");
#else
            // Linux code here
            process.start("sh");
            process.write("./fastboot_linux "+DevicePackage::getLockCmd().toLatin1()+"\n");
#endif

            process.write("exit\n");
            process.waitForFinished(); // sets current thread to sleep and waits for process end

            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Locking command performed.");
        }
    }
    else
    {
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_RecoveryButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        DialogRecovery* recovery_ui = new DialogRecovery(this);
        recovery_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap recovery(":/Icons/recovery.png");
        msgBox.setIconPixmap(recovery);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_refreshButton_clicked()
{
    // Check our device connection
    int connection = DeviceConnection::getConnection(DEFAULT_TIMEOUT);

    switch(connection)
    {
    case ADB:
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        break;

    case ADB_RECOVERY:
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        break;

    case ADB_SIDELOAD:
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        break;

    case FASTBOOT:
        ui->checkBox_ADB->setChecked(false);
        ui->checkBox_Fastboot->setChecked(true);
        break;

    default:
        ui->checkBox_ADB->setChecked(false);
        ui->checkBox_Fastboot->setChecked(false);
        break;

    }

    // Check again our reboot options
    checkOptions(connection);

    // Get the name of the device
    getDeviceName();

    // Use the sttusbar
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Refresh successfull!");
}

void MainWindow::on_SideloadButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have adb connection
    if(ui->checkBox_ADB->checkState())
    {
        DialogSideload* sideload_ui = new DialogSideload(this);
        sideload_ui->exec();
    }
    else
    {    // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap sideload(":/Icons/sideload_2.png");
        msgBox.setIconPixmap(sideload);
        msgBox.setText("You need to be in adb mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_BackupButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_ADB->checkState())
    {
        DialogBackups* backup_ui = new DialogBackups(this);
        backup_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap backup(":/Icons/backup.png");
        msgBox.setIconPixmap(backup);
        msgBox.setText("You need to be in adb mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::checkOptions(int connection)
{
    switch(connection)
    {
    case ADB:
        // Enable ADB only commands
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        ui->normalRebootButton->setEnabled(true);
        ui->bootloaderRebootButton->setEnabled(true);
        ui->recoveryRebootButton->setEnabled(true);
        ui->adbGroup->setEnabled(true);
        ui->fasbootGroup->setEnabled(false);
        ui->SideloadButton->setEnabled(false);
        ui->screenshotButton->setEnabled(true);
        ui->loggingButton->setEnabled(true);
        ui->installApkButton->setEnabled(true);
        ui->BackupButton->setEnabled(true);
        ui->rebootAdbButton->setEnabled(true);
        break;

    case ADB_RECOVERY:
        // Enable ADB only commands from recovery
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        ui->adbGroup->setEnabled(true);
        ui->fasbootGroup->setEnabled(false);
        ui->normalRebootButton->setEnabled(true);
        ui->bootloaderRebootButton->setEnabled(true);
        ui->recoveryRebootButton->setEnabled(true);
        ui->SideloadButton->setEnabled(false);
        ui->screenshotButton->setEnabled(false);
        ui->loggingButton->setEnabled(false);
        ui->installApkButton->setEnabled(false);
        ui->BackupButton->setEnabled(false);
        ui->rebootAdbButton->setEnabled(true);
        break;

    case ADB_SIDELOAD:
        // Enable ADB only commands for sideload
        ui->checkBox_ADB->setChecked(true);
        ui->checkBox_Fastboot->setChecked(false);
        ui->adbGroup->setEnabled(true);
        ui->fasbootGroup->setEnabled(false);
        ui->normalRebootButton->setEnabled(false);
        ui->bootloaderRebootButton->setEnabled(false);
        ui->recoveryRebootButton->setEnabled(false);
        ui->SideloadButton->setEnabled(true);
        ui->screenshotButton->setEnabled(false);
        ui->loggingButton->setEnabled(false);
        ui->installApkButton->setEnabled(false);
        ui->BackupButton->setEnabled(false);
        ui->rebootAdbButton->setEnabled(false);
        break;

    case FASTBOOT:
        // Enable fastboot only commands
        ui->checkBox_ADB->setChecked(false);
        ui->checkBox_Fastboot->setChecked(true);
        ui->normalRebootButton->setEnabled(true);
        ui->bootloaderRebootButton->setEnabled(true);
        ui->recoveryRebootButton->setEnabled(false);
        ui->adbGroup->setEnabled(false);
        ui->fasbootGroup->setEnabled(true);
        ui->SideloadButton->setEnabled(false);
        break;

    default:
        // Disable ALL!!!
        ui->checkBox_ADB->setChecked(false);
        ui->checkBox_Fastboot->setChecked(false);
        ui->normalRebootButton->setEnabled(false);
        ui->bootloaderRebootButton->setEnabled(false);
        ui->recoveryRebootButton->setEnabled(false);
        ui->adbGroup->setEnabled(false);
        ui->fasbootGroup->setEnabled(false);
        break;
    }

#ifdef QT_DEBUG
    ui->normalRebootButton->setEnabled(true);
    ui->bootloaderRebootButton->setEnabled(true);
    ui->recoveryRebootButton->setEnabled(true);
    ui->adbGroup->setEnabled(true);
    ui->fasbootGroup->setEnabled(true);
    ui->checkBox_ADB->setChecked(true);
    ui->checkBox_Fastboot->setChecked(true);
#endif
}

void MainWindow::on_rebootFastbootButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fasboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        if(ui->normalRebootButton->isChecked())
        {
            QProcess process_fastboot;
            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            process_fastboot.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_fastboot.start("cmd");
            process_fastboot.write("fastboot.exe reboot\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_fastboot.start("sh");
            process_fastboot.write("./fastboot_mac reboot\n");
#else
            // Linux code here
            process_fastboot.start("sh");
            process_fastboot.write("./fastboot_linux reboot\n");
#endif
            process_fastboot.write("exit\n");
            process_fastboot.waitForFinished(); // sets current thread to sleep and waits for process end

            // Use the statusbar
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Reboot to system successfull!");
        }
        else if(ui->bootloaderRebootButton->isChecked())
        {
            QProcess process_fastboot;
            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            process_fastboot.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_fastboot.start("cmd");
            process_fastboot.write("fastboot.exe reboot-bootloader\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_fastboot.start("sh");
            process_fastboot.write("./fastboot_mac reboot-bootloader\n");
#else
            // Linux code here
            process_fastboot.start("sh");
            process_fastboot.write("./fastboot_linux reboot-bootloader\n");
#endif

            process_fastboot.write("exit\n");
            process_fastboot.waitForFinished(); // sets current thread to sleep and waits for process end

            // Use the statusbar
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Reboot to bootloader successfull!");
        }
        else
        {
            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap reboot(":/Icons/reboot.png");
            msgBox.setIconPixmap(reboot);
            msgBox.setText("Reboot to what?");
            msgBox.setInformativeText("You really need to choose where you want to reboot...");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap reboot(":/Icons/reboot.png");
        msgBox.setIconPixmap(reboot);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    // Disable stuff after commands
    ui->checkBox_Fastboot->setChecked(false);
    checkOptions(DeviceConnection::getConnection(DEFAULT_TIMEOUT));
}

void MainWindow::on_rebootAdbButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have adb connection
    if(ui->checkBox_ADB->checkState())
    {
        if(ui->normalRebootButton->isChecked())
        {
            QProcess process_adb;
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
            process_adb.write("adb.exe reboot\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_adb.start("sh");
            process_adb.write("./adb_mac reboot\n");
#else
            // Linux code here
            process_adb.start("sh");
            process_adb.write("./adb_linux reboot\n");
#endif

            process_adb.write("exit\n");
            process_adb.waitForFinished(); // sets current thread to sleep and waits for process end

            // Use the statusbar
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Reboot to system successfull!");
        }
        else if(ui->bootloaderRebootButton->isChecked())
        {
            QProcess process_adb;
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
            process_adb.write("adb.exe reboot bootloader\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_adb.start("sh");
            process_adb.write("./adb_mac reboot bootloader\n");
#else
            // Linux code here
            process_adb.start("sh");
            process_adb.write("./adb_linux reboot bootloader\n");
#endif

            process_adb.write("exit\n");
            process_adb.waitForFinished(); // sets current thread to sleep and waits for process end

            // Use the statusbar
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Reboot to bootloader successfull!");
        }
        else if (ui->recoveryRebootButton->isChecked())
        {
            QProcess process_adb;
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
            process_adb.write("adb.exe reboot recovery\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_adb.start("sh");
            process_adb.write("./adb_mac reboot recovery\n");
#else
            // Linux code here
            process_adb.start("sh");
            process_adb.write("./adb_linux reboot recovery\n");
#endif

            process_adb.write("exit\n");
            process_adb.waitForFinished(); // sets current thread to sleep and waits for process end

            // Use the statusbar
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Reboot to recovery successfull!");
        }
        else
        {
            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap reboot(":/Icons/reboot.png");
            msgBox.setIconPixmap(reboot);
            msgBox.setText("Reboot to what?");
            msgBox.setInformativeText("You really need to choose where you want to reboot...");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap reboot(":/Icons/reboot.png");
        msgBox.setIconPixmap(reboot);
        msgBox.setText("You need to be in adb mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    // Disable stuff after commands
    ui->checkBox_ADB->setChecked(false);
    checkOptions(DeviceConnection::getConnection(DEFAULT_TIMEOUT));
}

void MainWindow::on_versionButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have adb connection
    if(ui->checkBox_ADB->checkState())
    {
        QProcess process;
        QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
        // Because apple likes it's application folders
        temp_path.cdUp();
        temp_path.cdUp();
        temp_path.cdUp();
#endif

        process.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
        // Windows code here
        process.start("cmd");
        process.write("adb.exe version\n");
#elif defined(Q_OS_MACX)
        // MAC code here
        process.start("sh");
        process.write("./adb_mac version\n");
#else
        // Linux code here
        process.start("sh");
        process.write("./adb_linux version\n");
#endif

        process.write("exit\n");
        process.waitForFinished(); // sets current thread to sleep and waits for process end
        QString output(process.readAllStandardOutput());

#ifdef Q_OS_WIN
        // Clean the version output on windows
        int aux = output.indexOf("Android Debug");
        output = output.remove(0, aux);
        aux = output.indexOf("-android");
        output = output.remove(aux,output.count());
#endif

        ui->statusBar->clearMessage();
        ui->statusBar->showMessage(output);
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap version(":/Icons/version.png");
        msgBox.setIconPixmap(version);
        msgBox.setText("You need to be in ADB mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_actionGo_to_XDA_Thread_triggered()
{
    QUrl link_to_xda(DevicePackage::getXdaLink().toLatin1());

    if(!link_to_xda.isEmpty())
    {
        QDesktopServices::openUrl (link_to_xda);
    }
    else
    {
        QMessageBox::critical(0, "Thread link not found!", "The link for the thread has not been found, please check your device package!");
    }
}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    QUrl version(DevicePackage::getVersionLink());

    mgr = new FileDownloader(version, this);
    mgr->downloadedData();

    connect(mgr, SIGNAL (downloaded()), this, SLOT (checkUpdate()));
}

void MainWindow::checkUpdate()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    if(!mgr->downloadedData().isEmpty())
    {

        QFile file(temp_path.absolutePath()+"/version.txt");

        // Open the file to write the data to it
        file.open(QFile::WriteOnly);
        file.write(mgr->downloadedData());
        file.close();

        // Open the temporary version file
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Error", file.errorString());
        }

        QString output = file.readLine();

        // Here we parse the version file
        QRegularExpression rxlen("(?<major>\\d).(?<minor>\\d).(?<patch>\\d)");
        QRegularExpressionMatch match = rxlen.match(output);
        bool newVersion = false;
        int newVerMajor = QString(match.captured("major")).toInt();
        int newVerMinor = QString(match.captured("minor")).toInt();
        int newVerPatch = QString(match.captured("patch")).toInt();

        if(match.hasMatch())
        {
            if (newVerPatch > VERSION_PATCH)
            {
                newVersion = true;
            }
            else if (newVerMinor > VERSION_MINOR)
            {
                newVersion = true;
            }
            else if (newVerMajor > VERSION_MAJOR)
            {
                newVersion = true;
            }
        }

        // It was temporary so delete it at this point
        file.remove();

        if(newVersion)
        {
            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap unlock(":/Icons/updates.png");
            msgBox.setIconPixmap(unlock);
            msgBox.setText("A new version has been found!");
            msgBox.setInformativeText(QString("Would you like to open the link for version %1.%2.%3").arg(newVerMajor).arg(newVerMinor).arg(newVerPatch));
            msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int exec = msgBox.exec();

            if (exec == QMessageBox::Yes)
            {
                QUrl link_to_download(DevicePackage::getDownloadLink().toLatin1());
                QDesktopServices::openUrl (link_to_download);
            }
        }
        else
        {
            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap unlock(":/Icons/updates.png");
            msgBox.setIconPixmap(unlock);
            msgBox.setText("You are up to date!");
            msgBox.setInformativeText("Nothing new under the sun, check later maybe.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox::critical(0, "Version check error!", "Unable to check for updates, please check your device package!");
    }
}

void MainWindow::on_actionAbout_triggered()
{
    // Show the about dialog
    AboutDialog* about_ui = new AboutDialog(this);
    about_ui->exec();
}

void MainWindow::on_loggingButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have adb connection
    if(ui->checkBox_ADB->checkState())
    {
        DialogLogging *logging_ui = new DialogLogging(this);
        logging_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap unlock(":/Icons/log.png");
        msgBox.setIconPixmap(unlock);
        msgBox.setText("You need to be in ADB mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_StockButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        DialogStock* stock_ui = new DialogStock(this);
        stock_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap stock(":/Icons/stock.png");
        msgBox.setIconPixmap(stock);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_screenshotButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_ADB->checkState())
    {
        DialogScreenshot* screen_ui = new DialogScreenshot(this);
        screen_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon(":/Icons/screenshot.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_eraseButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        DialogErase* erase_ui = new DialogErase(this);
        erase_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon(":/Icons/erase.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_installApkButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_ADB->checkState())
    {
        DialogInstallApk* instapk_ui = new DialogInstallApk(this);
        instapk_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap backup(":/Icons/application.png");
        msgBox.setIconPixmap(backup);
        msgBox.setText("You need to be in adb mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_flashingButton_clicked()
{
    // Refresh before trying to do stuff
    on_refreshButton_clicked();

    // Check if we have fastboot connection
    if(ui->checkBox_Fastboot->checkState())
    {
        DialogFlashing* flashing_ui = new DialogFlashing(this);
        flashing_ui->exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap backup(":/Icons/flashing.png");
        msgBox.setIconPixmap(backup);
        msgBox.setText("You need to be in fastboot mode!");
        msgBox.setInformativeText("Please refresh the connection before you try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWindow::on_actionCMD_triggered()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    // Set our current path to where we want to start our terminal
    QDir::setCurrent(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data"));

#ifdef Q_OS_WIN
    // Windows code here
    QProcess::startDetached("cmd");
#elif defined(Q_OS_MACX)
    // MAC code here
    QProcess::startDetached("open -a Terminal .");
#else
    // Linux code here
    QProcess::startDetached("gnome-terminal");
#endif

    // Set our current path back
    QDir::setCurrent(temp_path.absolutePath());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QProcess process;
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    process.setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
    // Windows code here
    process.start("cmd");
    process.write("adb.exe kill-server\n");
#elif defined(Q_OS_MACX)
    // MAC code here
    process.start("sh");
    process.write("./adb_mac kill-server\n");
#else
    // Linux code here
    process.start("sh");
    process.write("./adb_linux kill-server\n");
#endif

    process.write("exit\n");
    process.waitForFinished(); // sets current thread to sleep and waits for process end

    event->accept();
}
