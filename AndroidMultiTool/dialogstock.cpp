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

    // Enable the button upon selection only!
    ui->flashButton->setEnabled(false);

    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

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

    // Let's check if we have something
    if(ui->tableWidget->rowCount() == 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no stock packages available!");
        msgBox.setInformativeText("You can manually add one or wait for a new release.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        this->hide();
    }
}

DialogStock::~DialogStock()
{
    delete ui;
}

void DialogStock::processOutput()
{
    // Do nothing
}

void DialogStock::processFinished(int exitCode)
{
    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/stock.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Oups! Something went wrong...");
        msgBox.setInformativeText("Apparently the flashing exited with code "+ QString(exitCode) +".");
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
    if(ui->tableWidget->currentItem() != NULL)
    {
        QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
        // Because apple likes it's application folders
        temp_path.cdUp();
        temp_path.cdUp();
        temp_path.cdUp();
#endif

        QProcess* process_flash = new QProcess(this);
        QString temp_folder = QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/StockPackages/"+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text()+"/");

        qDebug() << temp_folder;

        process_flash->setProcessChannelMode(QProcess::MergedChannels);
        connect( process_flash, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
        connect( process_flash, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

        // Restrict from closing while flashing
        *busy = true;

        QDirIterator dirit(temp_folder,QDirIterator::Subdirectories);

        process_flash->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data"));

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

            qDebug() << "reached while" + dirit.fileName();

            // We need the bootloader
            if ((QFileInfo(dirit.filePath()).suffix() == "img") && (dirit.fileName().contains("bootloader")))
            {
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe flash bootloader \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("fastboot.exe reboot-bootloader\n");
                process_flash->write("ping -n 5 localhost >nul\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac flash bootloader \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_mac reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux flash bootloader \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_linux reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#endif
            }
            // We need the radio
            if ((QFileInfo(dirit.filePath()).suffix() == "img") && (dirit.fileName().contains("radio")))
            {
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe flash radio \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("fastboot.exe reboot-bootloader\n");
                process_flash->write("ping -n 5 localhost >nul\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac flash radio \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_mac reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux flash radio \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_linux reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#endif
            }
            // We need the update zip
            if ((QFileInfo(dirit.filePath()).suffix() == "zip") && (dirit.fileName().contains("image-")))
            {
#ifdef Q_OS_WIN
                // Windows code here
                process_flash->write("fastboot.exe -w update \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("fastboot.exe reboot-bootloader\n");
                process_flash->write("ping -n 5 localhost >nul\n");
#elif defined(Q_OS_MACX)
                // MAC code here
                process_flash->write("./fastboot_mac -w update \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_mac reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#else
                // Linux code here
                process_flash->write("./fastboot_linux -w update \""+dirit.filePath().toLatin1()+"\"\n");
                process_flash->write("./fastboot_linux reboot-bootloader\n");
                process_flash->write("sleep 5\n");
#endif
            }
        }

        process_flash->waitForStarted();
        process_flash->write("exit\n");

        // UI restrictions
        ui->tableWidget->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        ui->flashButton->setEnabled(false);
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
