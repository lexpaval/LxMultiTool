#include "dialogbackups.h"
#include "ui_dialogbackups.h"

DialogBackups::DialogBackups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBackups)
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

    getFiles();

    // Let's check if we have something
    if(ui->tableWidget->rowCount() == 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/backup.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("There are no backups available!");
        msgBox.setInformativeText("I guess it's time you make one.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

DialogBackups::~DialogBackups()
{
    delete ui;
}

void DialogBackups::getFiles()
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
    QDirIterator dirit(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/Backup/"),QDirIterator::Subdirectories);

    while(dirit.hasNext())
    {
        dirit.next();
        if (QFileInfo(dirit.filePath()).suffix() == "ab")
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

void DialogBackups::processFinishedBackup(int exitCode)
{
    QProcess *p = dynamic_cast<QProcess *>( sender() );

    QString error(p->readAllStandardError());
    error.remove("\n");
    error.remove("\r");

    qDebug() <<  exitCode;

    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/backup.png");
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
        QPixmap icon(":/Icons/backup.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Backup finished!");
        msgBox.setInformativeText("The selected backup has been completed.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->tableWidget->setEnabled(true);
    ui->buttonBox_2->setEnabled(true);
    ui->backupButton->setEnabled(true);
    ui->restoreButton->setEnabled(true);

    getFiles();
}

void DialogBackups::processFinishedRestore(int exitCode)
{
    QProcess *p = dynamic_cast<QProcess *>( sender() );

    QString error(p->readAllStandardError());
    error.remove("\n");
    error.remove("\r");

    qDebug() <<  exitCode;

    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/restore.png");
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
        QPixmap icon(":/Icons/restore.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Restore finished!");
        msgBox.setInformativeText("The selected backup has been restored successfully.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->tableWidget->setEnabled(true);
    ui->buttonBox_2->setEnabled(true);
    ui->backupButton->setEnabled(true);
    ui->restoreButton->setEnabled(true);
}

void DialogBackups::on_tableWidget_itemClicked()
{
    ui->restoreButton->setEnabled(true);
}

void DialogBackups::on_actionRefresh_triggered()
{
    getFiles();
}

void DialogBackups::on_actionDelete_triggered()
{
    if(ui->tableWidget->currentItem() != NULL)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/backup.png");
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

            QDir backup_dir(temp_path.absolutePath()+"/Data/Backup/"+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text());
            backup_dir.removeRecursively();

            getFiles();
        }
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/backup.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Nothing selected!");
        msgBox.setInformativeText("You need to select something in order to delete...");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogBackups::on_backupButton_clicked()
{
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    QString fileName = QFileDialog::getSaveFileName(this, tr("Select backup name and folder"), QDir::toNativeSeparators(QString(temp_path.absolutePath()+"/Data/Backup")), tr("Android Backup Files (*.ab)"));
    if(fileName != "")
    {
        QProcess* process_backup = new QProcess(this);

        connect( process_backup, SIGNAL(finished(int)), this, SLOT(processFinishedBackup(int)));

        if(!fileName.contains(".ab"))
        {
            // Append the ab extension if it's not available - linux limitation
            fileName = fileName+".ab";
        }

        // Prepare the file name for insertion in the process
        fileName = fileName+"\"\n";

        process_backup->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
        // Windows code here
        process_backup->start("cmd");
        fileName = "adb.exe backup -apk -shared -all -f \""+QDir::toNativeSeparators(fileName);
#elif defined(Q_OS_MACX)
        // MAC code here
        process_backup->start("sh");
        fileName = "./adb_mac backup -apk -shared -all -f \""+fileName;
#else
        // Linux code here
        process_backup->start("sh");
        fileName = "./adb_linux backup -apk -shared -all -f \""+fileName;
#endif
        process_backup->write(fileName.toLatin1());
        process_backup->write("exit\n");
    }
    else
    {
        // Prepare a messagebox
        QMessageBox msgBox(this);
        QPixmap backup(":/Icons/backup.png");
        msgBox.setIconPixmap(backup);
        msgBox.setText("No backup name selected!");
        msgBox.setInformativeText("You really need to put a name to your backup.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    qDebug() << fileName << " file_dialog";
}

void DialogBackups::on_restoreButton_clicked()
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

        QProcess* process_restore = new QProcess(this);
        QString temp_cmd = temp_path.absolutePath()+QDir::toNativeSeparators("/Data/Backup/")+ui->tableWidget->item(ui->tableWidget->currentRow() ,0)->text()+"\"\n";
        QString restore;

        connect( process_restore, SIGNAL(finished(int)), this, SLOT(processFinishedRestore(int)));

        // Restrict from closing while flashing
        *busy = true;

        process_restore->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
        // Windows code here
        process_restore->start("cmd");
        restore = "adb.exe restore \"" + temp_cmd;
#elif defined(Q_OS_MACX)
        // MAC code here
        process_restore->start("sh");
        restore = "./adb restore \"" + temp_cmd;
#else
        // Linux code here
        process_restore->start("sh");
        restore = "./adb restore \"" + temp_cmd;
#endif
        process_restore->waitForStarted();
        qDebug() << restore.toLatin1();
        process_restore->write(restore.toLatin1());
        process_restore->write("exit\n");

        // UI restrictions
        ui->tableWidget->setEnabled(false);
        ui->buttonBox_2->setEnabled(false);
        ui->backupButton->setEnabled(false);
        ui->restoreButton->setEnabled(false);
    }
}

void DialogBackups::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}
