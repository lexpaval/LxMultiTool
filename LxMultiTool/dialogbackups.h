#ifndef DIALOGBACKUPS_H
#define DIALOGBACKUPS_H

#include <QDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMovie>
#include "deviceconnection.h"

namespace Ui {
class DialogBackups;
}

class DialogBackups : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBackups(QWidget *parent = 0);
    ~DialogBackups();

private slots:
    void processFinishedBackup(int exitCode);

    void processFinishedRestore(int exitCode);

    void on_tableWidget_itemClicked();

    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

    void on_backupButton_clicked();

    void on_restoreButton_clicked();

    void on_exploreButton_clicked();

private:
    Ui::DialogBackups *ui;
    bool *busy;
    void getFiles();
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGBACKUPS_H
