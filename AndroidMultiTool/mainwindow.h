#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include "dialogrecovery.h"
#include "dialogsideload.h"
#include "dialogkernel.h"
#include "dialoglogging.h"
#include "dialogstock.h"
#include "dialogscreenshot.h"
#include "dialogerase.h"
#include "filedownloader.h"
#include "aboutdialog.h"
#include "version.h"

#define DEBUG

#ifdef DEBUG
#warning "YOU ARE IN DEBUG MODE"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_UnlockButton_clicked();

    void on_RelockButton_clicked();

    void on_RecoveryButton_clicked();

    void on_refreshButton_clicked();

    void on_SideloadButton_clicked();

    void on_BackupButton_clicked();

    void on_RestoreButton_clicked();

    void on_rebootFastbootButton_clicked();

    void on_rebootAdbButton_clicked();

    void on_versionButton_clicked();

    void on_kernelButton_clicked();

    void on_actionGo_to_XDA_Thread_triggered();

    void on_actionCheck_for_updates_triggered();

    void on_actionAbout_triggered();

    void on_loggingButton_clicked();

    void on_StockButton_clicked();

    void on_screenshotButton_clicked();

    void on_eraseButton_clicked();

    void checkUpdate();

private:
    void checkOptions();
    void getDeviceName();
    Ui::MainWindow *ui;
    FileDownloader* mgr;
};

#endif // MAINWINDOW_H
