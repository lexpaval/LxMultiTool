#ifndef DIALOGINSTALLAPK_H
#define DIALOGINSTALLAPK_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDirIterator>
#include "deviceconnection.h"

namespace Ui {
class DialogInstallApk;
}

class DialogInstallApk : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInstallApk(QWidget *parent = 0);
    ~DialogInstallApk();

private slots:
    void processFinished(int exitCode);

    void on_exploreButton_clicked();

    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

    void on_installButton_clicked();

private:
    Ui::DialogInstallApk *ui;
    bool *busy;
    void closeEvent(QCloseEvent *event);
    void getFiles();
};

#endif // DIALOGINSTALLAPK_H
