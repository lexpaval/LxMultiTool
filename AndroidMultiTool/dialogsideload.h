#ifndef DIALOGSIDELOAD_H
#define DIALOGSIDELOAD_H

#include <QDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDebug>

namespace Ui {
class DialogSideload;
}

class DialogSideload : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSideload(QWidget *parent = 0);
    ~DialogSideload();

private slots:
    void on_sideloadButton_clicked();

    void on_exploreButton_clicked();

    void on_tableWidget_itemClicked();

    void processOutput();

    void processFinished(int exitCode);
private:
    Ui::DialogSideload *ui;
    bool *busy;
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGSIDELOAD_H
