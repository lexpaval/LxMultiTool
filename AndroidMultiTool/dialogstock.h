#ifndef DIALOGSTOCK_H
#define DIALOGSTOCK_H

#include <QDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QListWidgetItem>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDebug>

namespace Ui {
class DialogStock;
}

class DialogStock : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStock(QWidget *parent = 0);
    ~DialogStock();

private slots:
    void processOutput();

    void processFinished(int exitCode);

    void on_exploreButton_clicked();

    void on_flashButton_clicked();

    void on_tableWidget_itemClicked();

private:
    Ui::DialogStock *ui;
    bool *busy;
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGSTOCK_H
