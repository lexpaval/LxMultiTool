#ifndef DIALOGLOGGING_H
#define DIALOGLOGGING_H

#include <QDialog>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDirIterator>
#include <QFileDialog>
#include <QProcess>
#include <QCloseEvent>

namespace Ui {
class DialogLogging;
}

class DialogLogging : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogging(QWidget *parent = 0);
    ~DialogLogging();

private slots:
    void processFinished(int exitCode);

    void on_getLogButton_clicked();

    void on_openFolderButton_clicked();

private:
    Ui::DialogLogging *ui;
    bool *busy;
    void closeEvent(QCloseEvent *event);
    void getFiles();
};

#endif // DIALOGLOGGING_H
