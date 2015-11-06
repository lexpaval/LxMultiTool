#include "dialoglogging.h"
#include "ui_dialoglogging.h"

DialogLogging::DialogLogging(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogging)
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

    // Populate the list
    getFiles();

    // Let's check if we have something
    if(ui->tableWidget->rowCount() == 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/log.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no logfiles available!");
        msgBox.setInformativeText("I guess it's time you make one.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

DialogLogging::~DialogLogging()
{
    delete ui;
}

void DialogLogging::getFiles()
{
    // Reset our row count
    ui->tableWidget->setRowCount(0);

    // Let's populate the list
    QDirIterator dirit(QDir::toNativeSeparators(QDir::currentPath()+"/Logfiles/"),QDirIterator::Subdirectories);

    while(dirit.hasNext())
    {
        dirit.next();
        if (QFileInfo(dirit.filePath()).suffix() == "txt")
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

void DialogLogging::processFinished(int exitCode)
{
    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/log.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Oups! Something went wrong...");
        msgBox.setInformativeText("Apparently the procedure exited with code "+ QString(exitCode) +".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->tableWidget->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->getLogButton->setEnabled(true);

    // Refresh the list
    getFiles();
}

void DialogLogging::on_getLogButton_clicked()
{
    if(ui->radioLogcat->isChecked())
    {
        // Restrict from closing while getting the log
        *busy = true;

        // UI restrictions
        ui->tableWidget->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        ui->getLogButton->setEnabled(false);

        QString fileName = QFileDialog::getSaveFileName(this, tr("Select logfile name or a custom folder"), QDir::toNativeSeparators(QString(QDir::currentPath()+"/Logfiles")), tr("Text File (*.txt)"));
        QProcess* process_log = new QProcess(this);
        QString logCommand;

        if(fileName != "")
        {
            if(!fileName.contains(".txt"))
            {
                // Append the txt extension if it's not available - linux limitation
                fileName = fileName+".txt";
            }

            // Prepare the file name for insertion in the process
            fileName = fileName+"\"\n";

            process_log->setProcessChannelMode(QProcess::ForwardedChannels);
            connect( process_log, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

            // Restrict from closing while flashing
            *busy = true;

            process_log->setWorkingDirectory(QDir::toNativeSeparators(QDir::currentPath()+"/Data"));
#ifdef Q_OS_WIN
            // Windows code here
            process_log->start("cmd");
            logCommand = "adb.exe logcat -d > \"" + fileName;
#elif Q_OS_MAC
            // MAC code here
            process_log->start("sh");
            logCommand = "./adb_mac logcat -d > \"" + fileName;
#else
            // Linux code here
            process_log->start("sh");
            logCommand = "./adb_linux logcat -d > \"" + fileName;
#endif
            process_log->waitForStarted();
            process_log->write(logCommand.toLatin1());
            process_log->write("exit\n");
        }
        else
        {
            // UI restrictions
            ui->tableWidget->setEnabled(true);
            ui->buttonBox->setEnabled(true);
            ui->getLogButton->setEnabled(true);

            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap recovery("../Icons/log.png");
            msgBox.setIconPixmap(recovery);
            msgBox.setText("No name selected for the log!");
            msgBox.setInformativeText("You really need to input a name for your log.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else if(ui->radioDmesg->isChecked())
    {
        // Restrict from closing while getting the log
        *busy = true;

        // UI restrictions
        ui->tableWidget->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        ui->getLogButton->setEnabled(false);

        QString fileName = QFileDialog::getSaveFileName(this, tr("Select logfile name or a custom folder"), QDir::toNativeSeparators(QString(QDir::currentPath()+"/Logfiles")), tr("Text File (*.txt)"));
        QProcess* process_log = new QProcess(this);
        QString logCommand;

        if(fileName != "")
        {
            if(!fileName.contains(".txt"))
            {
                // Append the txt extension if it's not available - linux limitation
                fileName = fileName+".txt";
            }

            // Prepare the file name for insertion in the process
            fileName = fileName+"\"\n";

            process_log->setProcessChannelMode(QProcess::ForwardedChannels);
            connect( process_log, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

            // Restrict from closing while flashing
            *busy = true;

            process_log->setWorkingDirectory(QDir::toNativeSeparators(QDir::currentPath()+"/Data"));
#ifdef Q_OS_WIN
            // Windows code here
            process_log->start("cmd");
            logCommand = "adb.exe shell dmesg > \"" + fileName;
#elif Q_OS_MAC
            // MAC code here
            process_log->start("sh");
            logCommand = "./adb_mac shell dmesg > \"" + fileName;
#else
            // Linux code here
            process_log->start("sh");
            logCommand = "./adb_linux shell dmesg > \"" + fileName;
#endif
            process_log->write(logCommand.toLatin1());
            process_log->write("exit\n");
        }
        else
        {
            // UI restrictions
            ui->tableWidget->setEnabled(true);
            ui->buttonBox->setEnabled(true);
            ui->getLogButton->setEnabled(true);

            // Prepare a messagebox
            QMessageBox msgBox(this);
            QPixmap icon("../Icons/log.png");
            msgBox.setIconPixmap(icon);
            msgBox.setText("No name selected for the log!");
            msgBox.setInformativeText("You really need to input a name for your log.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap icon("../Icons/log.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("No log type selected!");
        msgBox.setInformativeText("You really need to check Logcat or Dmesg.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogLogging::on_openFolderButton_clicked()
{
    QString path = QDir::toNativeSeparators(QDir::currentPath()+"/Logfiles");
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void DialogLogging::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}
