#include "dialogerase.h"
#include "ui_dialogerase.h"

DialogErase::DialogErase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogErase)
{
    ui->setupUi(this);
    busy = new bool(false);
}

DialogErase::~DialogErase()
{
    delete ui;
}

void DialogErase::on_eraseButton_clicked()
{
    // Prepare a messagebox
    QMessageBox msgBox(this->parentWidget());
    QPixmap icon(":/Icons/erase.png");
    msgBox.setIconPixmap(icon);

    QProcess* process_erase = new QProcess(this);
    process_erase->setProcessChannelMode(QProcess::ForwardedChannels);
    connect( process_erase, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    QDir temp_path(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MACX
    // Because apple likes it's application folders
    temp_path.cdUp();
    temp_path.cdUp();
    temp_path.cdUp();
#endif

    if(ui->boot_radioButton->isChecked())
    {
        ui->groupBox->setEnabled(false);
        ui->eraseButton->setEnabled(false);

        msgBox.setText("You are about to erase your boot partition.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            process_erase->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_erase->start("cmd");
            process_erase->write("fastboot erase boot\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_erase->start("sh");
            process_erase->write("./fastboot_mac erase boot\n");
#else
            // Linux code here
            process_erase->start("sh");
            process_erase->write("./fastboot_linux erase boot\n");
#endif
            process_erase->write("exit\n");
        }
    }
    else if (ui->cache_radioButton->isChecked())
    {
        ui->groupBox->setEnabled(false);
        ui->eraseButton->setEnabled(false);

        msgBox.setText("You are about to erase your cache partition.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            process_erase->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_erase->start("cmd");
            process_erase->write("fastboot.exe erase cache\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_erase->start("sh");
            process_erase->write("./fastboot_mac erase cache\n");
#else
            // Linux code here
            process_erase->start("sh");
            process_erase->write("./fastboot_linux erase cache\n");
#endif

            process_erase->write("exit\n");
        }
    }
    else if (ui->recovery_radioButton->isChecked())
    {
        ui->groupBox->setEnabled(false);
        ui->eraseButton->setEnabled(false);

        msgBox.setText("You are about to erase your recovery partition.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            process_erase->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_erase->start("cmd");
            process_erase->write("fastboot.exe erase recovery\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_erase->start("sh");
            process_erase->write("./fastboot_mac erase recovery\n");
#else
            // Linux code here
            process_erase->start("sh");
            process_erase->write("./fastboot_linux erase recovery\n");
#endif

            process_erase->write("exit\n");
        }
    }
    else if (ui->data_radioButton->isChecked())
    {
        ui->groupBox->setEnabled(false);
        ui->eraseButton->setEnabled(false);

        msgBox.setText("You are about to erase your data partition.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            process_erase->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_erase->start("cmd");
            process_erase->write("fastboot.exe erase userdata\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_erase->start("sh");
            process_erase->write("./fastboot_mac erase userdata\n");
#else
            // Linux code here
            process_erase->start("sh");
            process_erase->write("./fastboot_linux erase userdata\n");
#endif

            process_erase->write("exit\n");
        }
    }
    else if (ui->system_radioButton->isChecked())
    {
        ui->groupBox->setEnabled(false);
        ui->eraseButton->setEnabled(false);

        msgBox.setText("You are about to erase your system partition.");
        msgBox.setInformativeText("Are you sure you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Yes)
        {
            process_erase->setWorkingDirectory(QDir::toNativeSeparators(temp_path.absolutePath()+"/Data/"));
#ifdef Q_OS_WIN
            // Windows code here
            process_erase->start("cmd");
            process_erase->write("fastboot.exe erase system\n");
#elif defined(Q_OS_MACX)
            // MAC code here
            process_erase->start("sh");
            process_erase->write("./fastboot_mac erase system\n");
#else
            // Linux code here
            process_erase->start("sh");
            process_erase->write("./fastboot_linux erase system\n");
#endif

            process_erase->write("exit\n");
        }
    }
    else
    {
        msgBox.setText("Nothing selected!");
        msgBox.setInformativeText("You really need to select an erase option before erasing.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

void DialogErase::processFinished(int exitCode)
{
    if(exitCode != 0)
    {
        // Prepare a messagebox
        QMessageBox msgBox(this->parentWidget());
        QPixmap icon(":/Icons/erase.png");
        msgBox.setIconPixmap(icon);
        msgBox.setText("Oups! Something went wrong...");
        msgBox.setInformativeText("Apparently the flashing exited with code "+ QString(exitCode) +".");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }

    *busy = false;

    ui->groupBox->setEnabled(true);
    ui->eraseButton->setEnabled(true);
}

void DialogErase::closeEvent(QCloseEvent *event)
{
    // Let's decide if it's safe to exit or not
    if(*busy == true)
    {
        event->ignore();
    }
    else
        event->accept();
}
