#include "dialogflashing.h"
#include "ui_dialogflashing.h"

DialogFlashing::DialogFlashing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFlashing)
{
    ui->setupUi(this);

    busy = new bool(false);

    // Configure the table
    ui->tableWidget->setColumnCount(2);
    QStringList m_TableHeader;
    m_TableHeader << "Name" << "Size";
    ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
    // Beautification
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Get our context menu up and running
    ui->tableWidget->insertAction(NULL,ui->actionDelete);
    ui->tableWidget->insertAction(ui->actionDelete,ui->actionRefresh);

    // Enable the button upon selection only!
    ui->flashButton->setEnabled(false);

    // Set our loading animation
    ui->processLabel->setVisible(false);

    getFiles();

    // Let's check if we have something
    if(ui->tableWidget->rowCount() == 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no flashable images available!");
        msgBox.setInformativeText("You can manually add one or wait for a new release.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

DialogFlashing::~DialogFlashing()
{
    delete ui;
}

void DialogFlashing::getFiles()
{
    // Reset our row count
    ui->tableWidget->setRowCount(0);

    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    // Let's populate the list
    QDirIterator dirit(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/Flashable/"),QDirIterator::Subdirectories);

    while(dirit.hasNext())
    {
        dirit.next();
        if (QFileInfo(dirit.filePath()).suffix() == "img")
        {
            qint64 size_to_convert = QFileInfo(dirit.filePath()).size();
            float size_converted_float;
            QString size_converted_string;

            // Convert to B, KB or MB
            if (size_to_convert < 1025)
            {
                size_converted_string = QString::number(size_to_convert) + " B";
            }
            else if (size_to_convert < 1048577)
            {
                size_converted_float = size_to_convert / 1024.f;
                size_converted_string = QString::number(size_converted_float, 'f', 3) + " KB";
            }
            else
            {
                size_converted_float = size_to_convert / 1048576.f;
                size_converted_string = QString::number(size_converted_float, 'f', 2) + " MB";
            }

            // Add our stuff to the table
            ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(dirit.fileName()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(size_converted_string));
        }
    }
}

void DialogFlashing::on_actionRefresh_triggered()
{
    getFiles();
}

void DialogFlashing::on_tableWidget_itemClicked()
{
    ui->flashButton->setEnabled(true);

    // Automatically propose partition flashing based on the naming of the selected file
    if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("bootloader"))
    {
        ui->bootloaderRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("radio"))
    {
        ui->radioRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("recovery"))
    {
        ui->recoveryRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("system"))
    {
        ui->systemRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("userdata"))
    {
        ui->userdataRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("kernel") || ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("boot"))
    {
        ui->kernelRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("vendor"))
    {
        ui->vendorRadio->setChecked(true);
    }
    else if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("cache"))
    {
        ui->cacheRadio->setChecked(true);
    }
}

void DialogFlashing::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}

void DialogFlashing::on_actionDelete_triggered()
{
    if(ui->tableWidget->currentItem() != NULL)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Are you sure you want to delete?");
        msgBox.setInformativeText("The file will be PERMANENTLY deleted from you hard drive.");
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int option = msgBox.exec();

        if(option == QMessageBox::Yes)
        {
            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            QFile file(temp_path.absolutePath()+"/Data/Flashable/"+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text());
            file.remove();

            getFiles();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Nothing selected!");
        msgBox.setInformativeText("You need to select something in order to delete...");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogFlashing::processFinished(int exitCode)
{
    QProcess *p = dynamic_cast<QProcess *>( sender() );

    QString error(p->readAllStandardError());
    error.remove("\n");
    error.remove("\r");

    // Hide our loading animation
    ui->processLabel->setVisible(false);

    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Oups! Something went wrong...");
        msgBox.setInformativeText(error);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Image flashing successful!");
        msgBox.setInformativeText("Your selected image has been successfully flashed!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->tableWidget->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->flashButton->setEnabled(true);
}

void DialogFlashing::on_flashButton_clicked()
{
    if (DeviceConnection::getConnection() == FASTBOOT)
    {
        if(ui->tableWidget->currentItem() != NULL)
        {
            // Show our loading animation
            ui->processLabel->setVisible(true);
            QMovie *movie = new QMovie(":/Others/loader.gif");
            ui->processLabel->setMovie(movie);
            movie->start();

            QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
            // Because apple likes it's application folders
            temp_path.cdUp();
            temp_path.cdUp();
            temp_path.cdUp();
#endif

            QProcess* process_flash = new QProcess(this);
            QString temp_cmd = temp_path.absolutePath()+QDir::toNativeSeparators("/Data/Flashable/")+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text()+"\"\n";

            connect( process_flash, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

            // Restrict from closing while flashing
            *busy = true;

            if(ui->bootloaderRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("bootloader"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"bootloader\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash bootloader \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash bootloader \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash bootloader \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->radioRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("radio"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"radio\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash radio \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash radio \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash radio \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->recoveryRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("recovery"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"recovery\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash recovery \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash recovery \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash recovery \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->systemRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("system"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"system\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash system \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash system \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash system \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->userdataRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("userdata"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"userdata\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash userdata \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash userdata \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash userdata \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->kernelRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("kernel") || ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("boot"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"kernel\" or \"boot\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash boot \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash boot \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash boot \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->vendorRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("vendor"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"vendor\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash vendor \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash vendor \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash vendor \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else if(ui->cacheRadio->isChecked())
            {
                bool accepted = false;

                if(ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text().contains("cache"))
                {
                    accepted = true;
                }
                else
                {
                    // Prepare a messagebox for warning
                    QMessageBox msgBox(this);
                    QPixmap unlock(":/Icons/flashing.png");
                    msgBox.setIconPixmap(unlock);
                    msgBox.setText("The image might be wrong...");
                    msgBox.setInformativeText("The selected image does not contain \"cache\" in it's name, are you REALLY sure you want to flash it anyway?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int ret = msgBox.exec();

                    if (ret == QMessageBox::Yes)
                    {
                        accepted = true;
                    }
                }

                if(accepted)
                {
                    // start the flashing
                    QString flash_cmd;
                    process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
                    // Windows code here
                    process_flash->start("cmd");
                    flash_cmd = "fastboot.exe flash cache \"" + temp_cmd;
#elif defined(Q_OS_MACX)
                    // MAC code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_mac flash cache \"" + temp_cmd;
#else
                    // Linux code here
                    process_flash->start("sh");
                    flash_cmd = "./fastboot_linux flash cache \"" + temp_cmd;
#endif
                    process_flash->waitForStarted();
                    process_flash->write(flash_cmd.toLatin1());
                    process_flash->write("exit\n");

                    // UI restrictions
                    ui->tableWidget->setEnabled(false);
                    ui->buttonBox->setEnabled(false);
                    ui->flashButton->setEnabled(false);
                }
            }
            else
            {
                // Prepare a messagebox
                QMessageBox msgBox(this->parentWidget());
                QPixmap icon(":/Icons/flash.png");
                msgBox.setIconPixmap(icon);
                msgBox.setText("No partition selected!");
                msgBox.setInformativeText("Select a partition to flash and try again.");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }

        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/flash.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("No connection!");
        msgBox.setInformativeText("Check your phone connection and try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogFlashing::on_exploreButton_clicked()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    QString path = QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/Flashable");
    QDesktopServices::openUrl(QUrl("file:///" + path));
}
