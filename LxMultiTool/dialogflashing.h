#ifndef DIALOGFLASHING_H
#define DIALOGFLASHING_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include <QDirIterator>
#include <QMovie>
#include "deviceconnection.h"

namespace Ui {
class DialogFlashing;
}

class DialogFlashing : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFlashing(QWidget *parent = 0);
    ~DialogFlashing();

private slots:
    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

    void on_flashButton_clicked();

    void on_exploreButton_clicked();

    void on_tableWidget_itemClicked();

    void processFinished(int exitCode);

    void on_tableWidget_currentItemChanged();

private:
    Ui::DialogFlashing *ui;
    bool busy = false;
    void closeEvent(QCloseEvent *event);
    void getFiles();
};

#endif // DIALOGFLASHING_H
