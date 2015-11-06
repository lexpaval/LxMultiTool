#include "dialogrecovery.h"
#include "ui_dialogrecovery.h"

DialogRecovery::DialogRecovery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRecovery)
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

    // We don't want to see the empty progress bar
    ui->progressBar->setVisible(false);
    // Enable the button upon selection only!
    ui->flashButton->setEnabled(false);

    // Let's populate the list
    QDirIterator dirit(QDir::toNativeSeparators(QDir::currentPath()+"/Data/Recoveries/"),QDirIterator::Subdirectories);

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

    // Let's check if we have something
    if(ui->tableWidget->rowCount() == 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/recovery.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no recoveries available!");
        msgBox.setInformativeText("You can manually add one or wait for a new release.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        this->hide();
    }
}

DialogRecovery::~DialogRecovery()
{
    delete ui;
}

// Not used right now
void DialogRecovery::processOutput()
{
    QProcess *p = dynamic_cast<QProcess *>( sender() );
    QString progress = p->readAllStandardOutput();

    if(progress.contains("sending"))
        ui->progressBar->setValue(33);

    if(progress.contains("writing"))
        ui->progressBar->setValue(66);

    if(progress.contains("finished"))
        ui->progressBar->setValue(100);
}

void DialogRecovery::processFinished(int exitCode)
{
    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/recovery.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Oups! Something went wrong...");
        msgBox.setInformativeText("Apparently the flashing exited with code "+ QString(exitCode) +".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->progressBar->setValue(100);
    ui->tableWidget->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->flashButton->setEnabled(true);
}

void DialogRecovery::on_flashButton_clicked()
{
    if(ui->tableWidget->currentItem() != NULL)
    {
        QProcess* process_flash = new QProcess(this);
        QString temp_cmd = QDir::currentPath()+QDir::toNativeSeparators("/Data/Recoveries/")+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text()+"\"\n";
        QString recovery;

        process_flash->setProcessChannelMode(QProcess::ForwardedChannels);
        connect( process_flash, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
        connect( process_flash, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

        // Restrict from closing while flashing
        *busy = true;

        process_flash->setWorkingDirectory(QDir::toNativeSeparators(QDir::currentPath()+"/Data"));
#ifdef Q_OS_WIN
        // Windows code here
        process_flash->start("cmd");
        recovery = "fastboot.exe flash recovery \"" + temp_cmd;
#elif Q_OS_MAC
        // MAC code here
        process_flash->start("sh");
        recovery = "./fastboot_mac flash recovery \"" + temp_cmd;
#else
        // Linux code here
        process_flash->start("sh");
        recovery = "./fastboot_linux flash recovery \"" + temp_cmd;
#endif
        process_flash->waitForStarted();
        process_flash->write(recovery.toLatin1());
        process_flash->write("exit\n");

        // UI restrictions
        ui->tableWidget->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        ui->flashButton->setEnabled(false);
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);
    }
}

void DialogRecovery::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}

void DialogRecovery::on_exploreButton_clicked()
{
    QString path = QDir::toNativeSeparators(QDir::currentPath()+"/Data/Recoveries");
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void DialogRecovery::on_tableWidget_itemClicked()
{
    ui->flashButton->setEnabled(true);
}
