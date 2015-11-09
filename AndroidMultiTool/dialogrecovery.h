#ifndef DIALOGRECOVERY_H
#define DIALOGRECOVERY_H

#include <QDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QListWidgetItem>
#include <QCloseEvent>
#include <QDesktopServices>

namespace Ui {
class DialogRecovery;
}

class DialogRecovery : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRecovery(QWidget *parent = 0);
    ~DialogRecovery();

private slots:
    void processOutput();

    void processFinished(int exitCode);

    void on_flashButton_clicked();

    void on_exploreButton_clicked();

    void on_tableWidget_itemClicked();

    void on_actionRefresh_triggered();

    void on_actionDelete_triggered();

private:
    Ui::DialogRecovery *ui;
    bool *busy;
    void getFiles();
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGRECOVERY_H
