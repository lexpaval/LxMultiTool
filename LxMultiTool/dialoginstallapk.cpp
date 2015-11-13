#include "dialoginstallapk.h"
#include "ui_dialoginstallapk.h"

DialogInstallApk::DialogInstallApk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInstallApk)
{
    ui->setupUi(this);
}

DialogInstallApk::~DialogInstallApk()
{
    delete ui;
}
