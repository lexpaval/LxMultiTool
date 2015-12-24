#include "dialogstock.h"
#include "ui_dialogstock.h"

DialogStock::DialogStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStock)
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
        QPixmap icon(":/Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no stock packages available!");
        msgBox.setInformativeText("You can manually add one or wait for a new release.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

DialogStock::~DialogStock()
{
    delete ui;
}

void DialogStock::getFiles()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    // Reset our row count
    ui->tableWidget->setRowCount(0);

    // Let's populate the list
    QDirIterator dirit(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/StockPackages/"),QDirIterator::NoIteratorFlags);

    while(dirit.hasNext())
    {

        dirit.next();
        if (QFileInfo(dirit.filePath()).isDir() && dirit.fileName() != "." && dirit.fileName()!= "..")
        {

            qint64 size_to_convert = 0;
            QDir dir = dirit.filePath();

            // Get the total size of the folder
            QFileInfoList list = dir.entryInfoList();
            for (int i = 0; i < list.size(); ++i) {

                QFileInfo fileInfo = list.at(i);
                size_to_convert += fileInfo.size();

            }

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

void DialogStock::processOutput()
{
    // Do nothing
}

void DialogStock::processFinished(int exitCode)
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
        QPixmap icon(":/Icons/stock.png");
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
        QPixmap icon(":/Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Return to stock finished");
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

void DialogStock::on_flashButton_clicked()
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
            // Variables that hold the path to the images to be flashed
            QString bootloader_path;
            QString radio_path;
            QString image_path;

            QProcess* process_flash = new QProcess(this);
            QString temp_folder = QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/StockPackages/"+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text()+"/");

            connect( process_flash, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
            connect( process_flash, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

            // Restrict from closing while flashing
            *busy = true;

            QDirIterator dirit(temp_folder,QDirIterator::Subdirectories);

            process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));

#ifdef Q_OS_WIN
            // Windows code here
            process_flash->start("cmd");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_flash->start("sh");
#else
            // Linux code here
            process_flash->start("sh");
#endif

            while(dirit.hasNext())
            {
                dirit.next();

                // We need the bootloader
                if ((QFileInfo(dirit.filePath()).suffix() == "img") && (dirit.fileName().contains("bootloader")))
                {
                    bootloader_path = dirit.filePath().toLatin1();
                }
                // We need the radio
                if ((QFileInfo(dirit.filePath()).suffix() == "img") && (dirit.fileName().contains("radio")))
                {
                    radio_path = dirit.filePath().toLatin1();
                }
                // We need the update zip
                if ((QFileInfo(dirit.filePath()).suffix() == "zip") && (dirit.fileName().contains("image-")))
                {
                    image_path = dirit.filePath().toLatin1();
                }
            }

            if (bootloader_path.toLatin1() != "")
            {
                // Bootloader flashing
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe flash bootloader \""+bootloader_path.toLatin1()+"\"\n");
                process_flash->write("fastboot.exe reboot-bootloader\n");
                process_flash->write("ping -n 5 localhost >nul\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac flash bootloader \""+bootloader_path.toLatin1()+"\"\n");
                process_flash->write("./fastboot_mac reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux flash bootloader \""+bootloader_path.toLatin1()+"\"\n");
                process_flash->write("./fastboot_linux reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#endif
            }

            if (radio_path.toLatin1() != ""){
                // Radio flashing
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe flash radio \""+radio_path.toLatin1()+"\"\n");
                process_flash->write("fastboot.exe reboot-bootloader\n");
                process_flash->write("ping -n 5 localhost >nul\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac flash radio \""+radio_path.toLatin1()+"\"\n");
                process_flash->write("./fastboot_mac reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux flash radio \""+radio_path.toLatin1()+"\"\n");
                process_flash->write("./fastboot_linux reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#endif
            }

            if(image_path.toLatin1() != "")
            {
                // Update the rest
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe -w update \""+image_path.toLatin1()+"\"\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac -w update \""+image_path.toLatin1()+"\"\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux -w update \""+image_path.toLatin1()+"\"\n");
#endif
            }

            process_flash->waitForStarted();
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
        QPixmap icon(":/Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("No connection!");
        msgBox.setInformativeText("Check your phone connection and try again.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogStock::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}

void DialogStock::on_exploreButton_clicked()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    QString path = QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/StockPackages/"+ui->tableWidget->currentItem()->text());
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void DialogStock::on_tableWidget_itemClicked()
{
    ui->flashButton->setEnabled(true);
}

void DialogStock::on_actionRefresh_triggered()
{
    getFiles();
}

void DialogStock::on_actionDelete_triggered()
{
    if(ui->tableWidget->currentItem() != NULL)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Are you sure you want to delete?");
        msgBox.setInformativeText("The folder will be PERMANENTLY deleted from you hard drive.");
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

            QDir folder(temp_path.absolutePath()+"/Data/StockPackages/"+ui->tableWidget->currentItem()->text());
            folder.removeRecursively();

            getFiles();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Nothing selected!");
        msgBox.setInformativeText("You need to select something in order to delete...");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}
