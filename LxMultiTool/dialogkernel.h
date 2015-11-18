#ifndef DIALOGKERNEL_H
#define DIALOGKERNEL_H

#include <QDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include "deviceconnection.h"

namespace Ui {
class DialogKernel;
}

class DialogKernel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKernel(QWidget *parent = 0);
    ~DialogKernel();

private slots:
    void processOutput();

    void processFinished(int exitCode);

    void on_flashButton_clicked();

    void on_exploreButton_clicked();

    void on_tableWidget_itemClicked();

    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

private:
    Ui::DialogKernel *ui;
    bool *busy;
    void getFiles();
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGKERNEL_H
