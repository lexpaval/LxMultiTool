#ifndef DIALOGINSTALLAPK_H
#define DIALOGINSTALLAPK_H

#include <QDialog>
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

private:
    Ui::DialogInstallApk *ui;
};

#endif // DIALOGINSTALLAPK_H
