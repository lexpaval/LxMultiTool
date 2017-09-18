#ifndef DIALOGERASE_H
#define DIALOGERASE_H

#include <QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include "deviceconnection.h"

namespace Ui {
class DialogErase;
}

class DialogErase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogErase(QWidget *parent = 0);
    ~DialogErase();

private slots:
    void on_eraseButton_clicked();

    void processFinished(int exitCode);

private:
    Ui::DialogErase *ui;
    bool busy = false;
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGERASE_H
